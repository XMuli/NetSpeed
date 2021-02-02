#include "WinDockNet.h"

#include <QTimer>
#include <QDebug>
#include <QObjectList>
#include <QMessageBox>
#include <QIcon>
#include <QFile>
#include <QDate>
#include <climits>

/*!
 * \brief WinDockNet::WinDockNet
 * \param parent
 *
 * \htmlonly
 * <pre style="font-family: FreeMono, Consolas, Menlo, 'Noto Mono', 'Courier New', Courier, monospace;line-height: 100%;">
 * ==========================================================================
 * ||    上传标签    ||    网速 + 单位    ||    CPU标签    ||    CPU 使用率    ||
 * ==========================================================================
 * ||    下载标签    ||    网速 + 单位    ||    内存标签    ||    内存 使用率   ||
 * ==========================================================================
 * </pre>
 * \endhtmlonly
 */
WinDockNet::WinDockNet(WinDdeDockSetting *winSetting, Qt::Orientation orientation, QWidget *parent)
    : m_winSetting(winSetting)
    , QWidget(parent)
    , m_info(new MonitorInfo_x11())
    , m_timer(new QTimer())
    , m_timerNetTrafficLog(new QTimer())
    , m_modelUnit(Default)
    , m_precision(2)
    , m_vecOverWarningTemp(3, QVariant(false))
    , m_vecOverWarning(3, QVariant(false))
    , m_orientation(Qt::Horizontal)
    , m_gridLayout(new QGridLayout(this))
    , m_vecLabel(8, nullptr)
    , m_hover(true)
{
    init();
    connect(m_timer, &QTimer::timeout, this, &WinDockNet::onNet);
    connect(m_timer, &QTimer::timeout, this, &WinDockNet::onCpu);
    connect(m_timer, &QTimer::timeout, this, &WinDockNet::onMemory);
    connect(m_timer, &QTimer::timeout, this, &WinDockNet::onSystemRunTime);
    connect(m_timer, &QTimer::timeout, this, &WinDockNet::onNetOverWarning);
    m_timer->setInterval(1000);
    m_timer->start();

    connect(m_timerNetTrafficLog, &QTimer::timeout, this, &WinDockNet::onWriteNetworkTraffic);
    m_timerNetTrafficLog->setInterval(1000 * 4);
    m_timerNetTrafficLog->start();
}

WinDockNet::~WinDockNet()
{
}

WinDockNet *WinDockNet::winDockNetObject()
{
    return this;
}

void WinDockNet::init()
{
    for (auto it = m_vecLabel.begin(); it != m_vecLabel.end(); ++it)
        *it = new QLabel();

     m_gridLayout->setContentsMargins(0, 0, 0, 0);
     m_gridLayout->setSpacing(0);

     m_vecOverWarning.reserve(7);
     m_vecOverWarningTemp.reserve(7);
     m_vecOverWarningTemp.push_back(QVariant(0));
     m_vecOverWarningTemp.push_back(QVariant(0));
     m_vecOverWarningTemp.push_back(QVariant(0));
     m_vecOverWarningTemp.push_back(QVariant("MB"));

     initSigConnect();
     m_winSetting->readConfigWinDdeDock();

     setLabWidgetLayout(m_winSetting->isHorizontal());
     m_info->netInfo(m_upload, m_down);
     m_info->cpuInfo(m_vec);
}

void WinDockNet::initSigConnect()
{
    // 响应 WinDdeDockSetting 发射的信号
   connect(m_winSetting, &WinDdeDockSetting::sigCurrentFont, this, &WinDockNet::onCurrentFont);
   connect(m_winSetting, &WinDdeDockSetting::sigFontSize, this, &WinDockNet::onFontSize);
   connect(m_winSetting, &WinDdeDockSetting::sigUnitModel, this, &WinDockNet::onUnitModel);
   connect(m_winSetting, &WinDdeDockSetting::sigShowModel, this, &WinDockNet::onShowModel);
   connect(m_winSetting, &WinDdeDockSetting::sigLabTextColor, this, &WinDockNet::onLabTextColor);
   connect(m_winSetting, &WinDdeDockSetting::sigTextColor, this, &WinDockNet::onTextColor);
   connect(m_winSetting, &WinDdeDockSetting::sigLabUploadText, this, &WinDockNet::onLabUploadText);
   connect(m_winSetting, &WinDdeDockSetting::sigLabDownText, this, &WinDockNet::onLabDownText);
   connect(m_winSetting, &WinDdeDockSetting::sigLabCpuText, this, &WinDockNet::onLabCpuText);
   connect(m_winSetting, &WinDdeDockSetting::sigLabMemoryText, this, &WinDockNet::onLabMemoryText);
//    connect(m_winSetting, &WinDdeDockSetting::sigLabDiskReadText, this, &WinDockNet::onLabDiskReadText);
//    connect(m_winSetting, &WinDdeDockSetting::sigLabDiskWriteText, this, &WinDockNet::onLabDiskWriteText);
   connect(m_winSetting, &WinDdeDockSetting::sigDisolayNet, this, &WinDockNet::onDisolayNet);
   connect(m_winSetting, &WinDdeDockSetting::sigDisolayCPUAndMemory, this, &WinDockNet::onDisolayCPUAndMemory);
//    connect(m_winSetting, &WinDdeDockSetting::sigDisolayDisk, this, &WinDockNet::onDisolayDisk);
   connect(m_winSetting, &WinDdeDockSetting::sigLocationExchangeNet, this, &WinDockNet::onLocationExchangeNet);
   connect(m_winSetting, &WinDdeDockSetting::sigLocationExchangeCPUAndMenory, this, &WinDockNet::onLocationExchangeCPUAndMenory);
//    connect(m_winSetting, &WinDdeDockSetting::sigLocationExchangeDisk, this, &WinDockNet::onLocationExchangeDisk);
   connect(m_winSetting, &WinDdeDockSetting::sigFractionalAccuracy, this, &WinDockNet::onFractionalAccuracy);
   connect(m_winSetting, &WinDdeDockSetting::sigRefreshInterval, this, &WinDockNet::onRefreshInterval);
   connect(m_winSetting, &WinDdeDockSetting::sigHoverDisplay, this, &WinDockNet::onHoverDisplay);

   // 响应 WinMain 发射的信号
   connect(m_winSetting, &WinDdeDockSetting::sigCpuOver, this, &WinDockNet::onCpuOver);
   connect(m_winSetting, &WinDdeDockSetting::sigMemOver, this, &WinDockNet::onMemOver);
   connect(m_winSetting, &WinDdeDockSetting::sigNetOver, this, &WinDockNet::onNetOver);
   connect(m_winSetting, &WinDdeDockSetting::sigCpuOverNum, this, &WinDockNet::onCpuOverNum);
   connect(m_winSetting, &WinDdeDockSetting::sigMemOverNum, this, &WinDockNet::onMemOverNum);
   connect(m_winSetting, &WinDdeDockSetting::sigNetOverNum, this, &WinDockNet::onNetOverNum);
   connect(m_winSetting, &WinDdeDockSetting::sigNetNumUnit, this, &WinDockNet::onNetNumUnit);
   connect(m_winSetting, &WinDdeDockSetting::sigBtnApplyWinMain, this, &WinDockNet::onBtnApplyWinMain);
}

/*!
 * \brief WinDockNet::setLabWidgetLayout 设置 dde 的插件布局（水平 、垂直）
 * \param[in] isHorizontal true 水平； false 垂直
 */
void WinDockNet::setLabWidgetLayout(bool isHorizontal)
{
    if (isHorizontal)
        m_orientation = Qt::Horizontal;
    else
        m_orientation = Qt::Vertical;

    setLabWidgetLayout(m_orientation);
}

/*!
 * \brief WinDockNet::setLabWidgetLayout 设置 dde 的插件布局（水平 、垂直）
 * \param[in] orientation 希望设置为的布局方式
 */
void WinDockNet::setLabWidgetLayout(Qt::Orientation orientation)
{
    if (m_gridLayout == nullptr)
        return;

    const int nNum = m_gridLayout->children().count();
    for (int i = 0; i < nNum; ++i) {
        QLayoutItem *item = m_gridLayout->layout()->itemAt(i);
        m_gridLayout->removeItem(item);
    }

    if (orientation == Qt::Horizontal && m_gridLayout->children().count() == 0) {
        for (int i = 0; i < m_vecLabel.count(); i += 4) {
            int iTo2 = i % 2;
            m_gridLayout->addWidget(m_vecLabel[i], 0, i / 2 + iTo2, Qt::AlignLeft);
            m_gridLayout->addWidget(m_vecLabel[i + 1], 0, (i + 1) / 2 + (i + 1) % 2, Qt::AlignRight);
            m_gridLayout->addWidget(m_vecLabel[i + 2], 1, i / 2 + iTo2, Qt::AlignLeft);
            m_gridLayout->addWidget(m_vecLabel[i + 3], 1, (i + 1) / 2 + (i + 1) % 2, Qt::AlignRight);
        }
    } else {
        for (int i = 0; i < m_vecLabel.count(); ++i) {
            int iTo2 = i % 2;
            m_gridLayout->addWidget(m_vecLabel[i], i / 2, iTo2, iTo2 ? Qt::AlignRight : Qt::AlignLeft);
        }
    }
}

bool WinDockNet::isHoverDisplay()
{
    return m_hover;
}

/*!
 * \brief WinDockNet::DataOverWarning 当 cpu、mem、net 勾选预警提示，并且超过各自指定预警值，弹出提示窗口
 * \param tile 弹窗标题
 * \param text 弹窗内容
 * \param isTransient 是否为临时消息（默认显示 4000 ms 后自动消失）
 * \param ms 临时弹窗消息，默认显示时间毫秒数
 */\
void WinDockNet::DataOverWarning(QString title, QString text, QWidget *parent, bool isTransient, int ms)
{
    if (isTransient) {
        QMessageBox *msg = new QMessageBox(QMessageBox::Information, title, text);
        msg->setWindowFlags(Qt::WindowStaysOnTopHint);
        QTimer::singleShot(ms, msg, &QMessageBox::close);
        msg->exec();
    } else {
        QMessageBox::information(parent, title, text);
    }
}

/*!
 * \brief WinDockNet::writeNetworkTraffic 将网络接收发送网络总字节数字，保存在 .log中；时间最新的永远在
 *                                        最上一行，同一天仅只能有一条流量记录
 * \param[in] log 最新的一行数据，保存在第一行
 * \note QIODevice:: WriteOnly、Append、Truncate，会打开时候就清空、或者调用 .readAll() 为 "" 空；
 *                   ReadWrite 读没问题，写的时候 .readAll() 为 "" 空；故打开和两次，坑死了！！！
 */
void WinDockNet::writeNetworkTraffic(QString &log)
{
    QFile file(":/NetworkTraffic.log");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qErrnoWarning("../dde-dock-plugin/NetworkTraffic.log  [ReadOnly]\"  don't open!");
        return;
    }

    QTextStream stream(&file);
    QString firstLine = stream.readLine();
    QStringList listFirstLine = firstLine.simplified().split(QRegExp("\\s{1,}"));
    QStringList listLog = log.simplified().split(QRegExp("\\s{1,}"));
    stream.seek(0);
    QString dataAll = stream.readAll();

    if (listFirstLine[0] == listLog[0]) {
        dataAll.replace(0, dataAll.indexOf('\n') + 1, log);  // + 1 是因此返回为下标为 0 的序号，而填入参数为 替换字符的个数
    } else {
          dataAll.prepend(log);
    }
    file.close();

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qErrnoWarning("../dde-dock-plugin/NetworkTraffic.log [WriteOnly]\"  don't open!");
        return;
    }

    stream << dataAll;
    file.close();
}

void WinDockNet::readNetworkTraffic(long &net)
{
    QFile file(":/NetworkTraffic.log");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qErrnoWarning("../dde-dock-plugin/NetworkTraffic.log  [ReadOnly]\"  don't open!");
        return;
    }

    QTextStream stream(&file);
    QStringList listDoday = stream.readLine().simplified().split(QRegExp("\\s{1,}"));
    QStringList listYesterday = stream.readLine().simplified().split(QRegExp("\\s{1,}"));

    bool ok = false;
    long doday = listDoday[5].toLong(&ok);
    long yesterday = listYesterday[5].toLong(&ok);
    if (ok) {
        net = doday - yesterday;
    }
    file.close();
}

/*!
 * \brief WinDockNet::netOverNumToByte 将预警提示流量数值转换为字节输出
 * \param[in] net 流量预警输入框输入的数值和单位，换算为对应的字节
 * \return 以 Byte 单位的流量数值
 */
long WinDockNet::netOverNumToByte(long net)
{
    long ret = 0;
    QString strNetUnit =  m_vecOverWarningTemp[6].toString();
    if (strNetUnit == "Byte") {
        ret = net;
    } else if (strNetUnit == "KB") {
        ret = net * 1024;
    } else if (strNetUnit == "MB") {
        ret = net * 1024 * 1024;
    } else if (strNetUnit == "GB") {
        ret = net * 1024 * 1024 * 1024;
    } else if (strNetUnit == "TB") {
        ret = net * 1024 * 1024 * 1024 * 1024;
    }

    if (net > LONG_MAX) {
        QMessageBox::information(this, tr("数值过大"), tr("数值超过 long 型的最大值"));
        return 0;
    } else {
        return ret;
    }
}

/*!
 * \brief WinDockNet::showTest 测试函数
 * \param str 打印自定义字符串
 */
void WinDockNet::showTest(QString str)
{
    qDebug()<<"=================================#" + str << "===>"<<m_vecOverWarning
           <<m_vecOverWarning.size()<<m_vecOverWarning.capacity();

    qDebug()<<"=================================#" + str << "===>"<<m_vecOverWarningTemp
           <<m_vecOverWarningTemp.size()<<m_vecOverWarningTemp.capacity();
}

QString WinDockNet::hoverDisplayText()
{
    double run = 0;
    double idle = 0;
    if (m_hover) {
        m_info->systemRunTime(run, idle);
        return m_info->runTimeUnit(run);
    } else {
        return "";
    }
}

/*!
 * \brief WinDockNet::onNet 定时刷新网速
 */
void WinDockNet::onNet()
{
    long upload = 0;
    long down = 0;
    NetUnit netUnit = Byte;
    QString unit = "";

    m_info->netInfo(upload, down);
    double increaseUpload  = m_info->netShowUnit((upload - m_upload) / (m_timer->interval() /  1000.0), netUnit);
    unit = m_info->netModelUnit(netUnit, m_modelUnit);
    m_vecLabel[1]->setText(QString("%1").arg(increaseUpload, 0, 'f', m_precision, QLatin1Char(' ')) + unit);

    netUnit = Byte;
    double increaseDown = m_info->netShowUnit((down - m_down) / (m_timer->interval() / 1000.0), netUnit);
    unit = m_info->netModelUnit(netUnit, m_modelUnit);
    m_vecLabel[3]->setText(QString("%1").arg(increaseDown, 0, 'f', m_precision, QLatin1Char(' ')) + unit);

    m_upload = upload;
    m_down = down;
}

/*!
 * \brief WinDockNet::onCpu 定时刷新 CPU
 */
void WinDockNet::onCpu()
{
    QVector<CpuInfo> vec;
    m_info->cpuInfo(vec);

    double valCpu = (vec.begin()->cpuWork - m_vec.begin()->cpuWork) * 100.0 / (vec.begin()->cpuAll - m_vec.begin()->cpuAll);
    m_vecLabel[5]->setText(QString("%1%").arg(valCpu, 0, 'f', m_precision, QLatin1Char(' ')));

    // CPU 显示为负数
    if (valCpu < 0) {
         __builtin_trap();
         int n = 0;
         for (auto it : vec) {
             qDebug() << "=====================>【" << n++ <<"】" << it.index << "  " << it.cpuWork << "  " << it.cpuAll;
         }
    }

    if (m_vecOverWarning[0].toBool()) {
        QString title("CPU 提示");
        QString text("");
        bool ok = false;
        int overNum = m_vecOverWarning[3].toInt(&ok);
        if (ok)
            text = "CPU 使用率超过" + QString::number(overNum) + "%";

        if (valCpu >= overNum) {
            m_vecOverWarning[0].setValue(false);
            DataOverWarning(title, text, m_winSetting);
        }
    }
}

/*!
 * \brief WinDockNet::onMemory 定时刷新 内存
 */
void WinDockNet::onMemory()
{
    MemoryInfo info;
    m_info->memoryInfo(info);

    double mem = (info.memoryAll - info.memoryFree) * 100.0 / info.memoryAll;
    m_vecLabel[7]->setText(QString("%1%").arg(mem, 0, 'f', m_precision, QLatin1Char(' ')));
//    ui->lab_34->setText(QString("%1%").arg(swap, 0, 'f', m_precision, QLatin1Char(' ')));

    if (m_vecOverWarning[1].toBool()) {
        QString title("内存提示");
        QString text("");
        bool ok = false;
        int overNum = m_vecOverWarning[4].toInt(&ok);
        if (ok)
            text = "内存使用率超过" + QString::number(overNum) + "%";

        if (mem >= overNum) {
            DataOverWarning(title, text, m_winSetting);
            m_vecOverWarning[1].setValue(false);
        }
    }
}

/*!
 * \brief WinDockNet::onSystemRunTime 定时刷新 系统开机运行时间
 */
void WinDockNet::onSystemRunTime()
{
    double run = 0;
    double idle = 0;
    m_info->systemRunTime(run, idle);
    //    ui->lab_32->setText(m_info->runTimeUnit(run));
}

void WinDockNet::onWriteNetworkTraffic()
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    QString log = time + " upload: " + QString::number(m_upload) + " down: " + QString::number(m_down) + "\n";
    writeNetworkTraffic(log);
}

/*!
 * \brief WinDockNet::onNetOverWarning 槽函数，监测流量超过预警限制
 */
void WinDockNet::onNetOverWarning()
{
    if (m_vecOverWarning[2].toBool()) {
        QString title("流量提示");
        QString text("");
        bool ok = false;
        long overNum = static_cast<long>(m_vecOverWarning[5].toLongLong(&ok));

        NetUnit unit = NetUnit::Byte;
        long autoUnitOverNum = m_info->netShowUnit(overNum, unit);
        if (ok) {
            QString strUnit("");
            if (unit == NetUnit::Byte)
                strUnit = " Byte！";
            else if (unit == NetUnit::Kb)
                strUnit = " KB！";
            else if (unit == NetUnit::Mb)
                strUnit = " MB";
            else if (unit == NetUnit::Gb)
                strUnit = " GB";
            else if (unit == NetUnit::Tb)
                strUnit = " TB";
            text = "今日下载流量使用超过" + QString::number(autoUnitOverNum) + strUnit;
        }

        long net = 0;
        readNetworkTraffic(net);
        if (net >= overNum) {
            m_vecOverWarning[2].setValue(false);
            DataOverWarning(title, text, m_winSetting);
        }
    }
}

void WinDockNet::onCurrentFont(const QFont &font)
{
    int nCount = m_gridLayout->count();
    for (int i = 0; i < nCount; ++i) {
        QLayoutItem *it = m_gridLayout->itemAt(i);
        QLabel * lab = static_cast<QLabel *>(it->widget());
        lab->setFont(font);
    }
}

void WinDockNet::onFontSize(int size)
{
    QFont font;
    font.setPointSize(size);

    int nCount = m_gridLayout->count();
    for (int i = 0; i < nCount; ++i) {
        QLayoutItem *it = m_gridLayout->itemAt(i);
        QLabel * lab = static_cast<QLabel *>(it->widget());
        lab->setFont(font);
    }
}

/*!
 * \brief WinDockNet::onShowModel 插件水平还是垂直
 * \param[in] check 现实模式中水平 (radioHorizontal) 控件是否被选中
 * \line            true 水平； false 垂直
 */
void WinDockNet::onShowModel(bool check)
{
    setLabWidgetLayout(check);
}

/*!
 * \brief WinDockNet::onUnitModel 单位显示模式
 */
void WinDockNet::onUnitModel(const QString &text)
{
    if (text == "Default")
        m_modelUnit = ModelUnit::Default;
    else if (text == "Upper")
        m_modelUnit = ModelUnit::Upper;
    else if (text == "Lower")
        m_modelUnit = ModelUnit::Lower;
    else if (text == "Custom")
        m_modelUnit = ModelUnit::Custom;
}

void WinDockNet::onLabTextColor(const QColor color)
{
    // TODO: 2021-01-08 加一行，颜色相同就返回, 结尾出，后面添加磁盘功能
    QPalette palette;
    palette.setColor(QPalette::WindowText, color);
    m_vecLabel[0]->setPalette(palette);
    m_vecLabel[2]->setPalette(palette);
    m_vecLabel[4]->setPalette(palette);
    m_vecLabel[6]->setPalette(palette);
}

void WinDockNet::onTextColor(const QColor color)
{
    // TODO: 2021-01-08 加一行，颜色相同就返回, 结尾出，后面添加磁盘功能
    QPalette palette;
    palette.setColor(QPalette::WindowText, color);
    m_vecLabel[1]->setPalette(palette);
    m_vecLabel[3]->setPalette(palette);
    m_vecLabel[5]->setPalette(palette);
    m_vecLabel[7]->setPalette(palette);
}

void WinDockNet::onLabUploadText(const QString &text)
{
    if (m_vecLabel[0]->text() != text)
        m_vecLabel[0]->setText(text);
}

void WinDockNet::onLabDownText(const QString &text)
{
    if (m_vecLabel[2]->text() != text)
        m_vecLabel[2]->setText(text);
}

void WinDockNet::onLabCpuText(const QString &text)
{
    if (m_vecLabel[4]->text() != text)
        m_vecLabel[4]->setText(text);
}

void WinDockNet::onLabMemoryText(const QString &text)
{
    if (m_vecLabel[6]->text() != text)
        m_vecLabel[6]->setText(text);
}

void WinDockNet::onDisolayNet(bool check)
{
    if (check) {
        m_vecLabel[0]->show();
        m_vecLabel[1]->show();
        m_vecLabel[2]->show();
        m_vecLabel[3]->show();
    } else {
        m_vecLabel[0]->hide();
        m_vecLabel[1]->hide();
        m_vecLabel[2]->hide();
        m_vecLabel[3]->hide();
    }

}

void WinDockNet::onDisolayCPUAndMemory(bool check)
{
    if (check) {
        m_vecLabel[4]->show();
        m_vecLabel[5]->show();
        m_vecLabel[6]->show();
        m_vecLabel[7]->show();
    } else {
        m_vecLabel[4]->hide();
        m_vecLabel[5]->hide();
        m_vecLabel[6]->hide();
        m_vecLabel[7]->hide();
    }
}

void WinDockNet::onLocationExchangeNet(bool check)
{
    Q_UNUSED(check)
    QLabel labLab(m_vecLabel[0]->text());
    QLabel labText(m_vecLabel[1]->text());

    m_vecLabel[0]->setText(m_vecLabel[2]->text());
    m_vecLabel[1]->setText(m_vecLabel[3]->text());
    m_vecLabel[2]->setText(labLab.text());
    m_vecLabel[3]->setText(labText.text());
}

void WinDockNet::onLocationExchangeCPUAndMenory(bool check)
{
    Q_UNUSED(check)
    QLabel labLab(m_vecLabel[4]->text());
    QLabel labText(m_vecLabel[5]->text());

    m_vecLabel[4]->setText(m_vecLabel[6]->text());
    m_vecLabel[5]->setText(m_vecLabel[7]->text());
    m_vecLabel[6]->setText(labLab.text());
    m_vecLabel[7]->setText(labText.text());
}

void WinDockNet::onFractionalAccuracy(int num)
{
    if (m_precision != num)
        m_precision = num;
}

void WinDockNet::onRefreshInterval(int interval)
{
    if (m_timer->interval() != interval)
        m_timer->setInterval(interval);
}

void WinDockNet::onHoverDisplay(bool check)
{
    if (m_hover != check)
        m_hover = check;
}

void WinDockNet::onCpuOver(bool check)
{
    m_vecOverWarningTemp[0].setValue(check);
    onBtnApplyWinMain();
    showTest("onCpuOver");
}

void WinDockNet::onMemOver(bool check)
{
    m_vecOverWarningTemp[1].setValue(check);
    onBtnApplyWinMain();
    showTest("onMemOver");
}

void WinDockNet::onNetOver(bool check)
{
    m_vecOverWarningTemp[2].setValue(check);
    onBtnApplyWinMain();
    showTest("onNetOver");
}

void WinDockNet::onCpuOverNum(int cpu)
{
    m_vecOverWarningTemp[3].setValue(cpu);
    showTest("onCpuOverNum");
}

void WinDockNet::onMemOverNum(int mem)
{
    m_vecOverWarningTemp[4].setValue(mem);
    showTest("onMemOverNum");
}

void WinDockNet::onNetOverNum(int net)
{
    m_vecOverWarningTemp[5].setValue(netOverNumToByte(net));
    showTest("onNetOverNum");
}

void WinDockNet::onNetNumUnit(const QString &unit)
{
    m_vecOverWarningTemp[6].setValue(unit);
    showTest("onNetNumUnit");
}

void WinDockNet::onBtnApplyWinMain()
{
    m_vecOverWarning = m_vecOverWarningTemp;
}


