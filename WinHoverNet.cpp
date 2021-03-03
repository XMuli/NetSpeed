#include "WinHoverNet.h"

#include <QTimer>
#include <QDebug>
#include <QObjectList>
#include <QMessageBox>
#include <QIcon>
#include <QFile>
#include <QDate>
#include <climits>
#include <QPalette>

/*!
 * \brief WinHoverNet::WinHoverNet
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
 *
 * \htmlonly
 * <pre style="font-family: FreeMono, Consolas, Menlo, 'Noto Mono', 'Courier New', Courier, monospace;line-height: 100%;">
 * =======================================
 * ||    上传标签    ||    网速 + 单位     ||
 * =======================================
 * ||    下载标签    ||    网速 + 单位     ||
 * =======================================
 * ||    CPU标签    ||    CPU 使用率      ||
 * =======================================
 * ||    内存标签    ||    内存 使用率     ||
 * =======================================
 * </pre>
 * \endhtmlonly
 */
WinHoverNet::WinHoverNet(Qt::Orientation orientation, QWidget *parent)
    : WinTransparent(parent)
    , m_winSetting(new WinSetting())  // 没设置父对象， null
    , m_info(new MonitorInfo_x11())
    , m_timer(new QTimer())
    , m_modelUnit(Default)
    , m_precision(2)
    , m_vecOverWarningTemp(3, QVariant(false))
    , m_vecOverWarning(3, QVariant(false))
    , m_orientation(orientation)
    , m_gridLayout(new QGridLayout(this))
    , m_vecLabel(8, nullptr)
//    , m_hover(true)
{
    init();
    connect(m_timer, &QTimer::timeout, this, &WinHoverNet::onNet);
    connect(m_timer, &QTimer::timeout, this, &WinHoverNet::onCpu);
    connect(m_timer, &QTimer::timeout, this, &WinHoverNet::onMemory);
//    connect(m_timer, &QTimer::timeout, this, &WinHoverNet::onSystemRunTime);
    m_timer->setInterval(1000);
    m_timer->start();
}


WinHoverNet::~WinHoverNet()
{
}

//WinHoverNet *WinHoverNet::winHoverNetObject()
//{
//    return this;
//}

void WinHoverNet::init()
{
    for (auto it = m_vecLabel.begin(); it != m_vecLabel.end(); ++it)
        *it = new QLabel();

     m_gridLayout->setContentsMargins(0, 0, 0, 0);
     m_gridLayout->setSpacing(0);

     m_vecOverWarning.reserve(4);
     m_vecOverWarningTemp.reserve(4);
     m_vecOverWarningTemp.push_back(QVariant(0));
     m_vecOverWarningTemp.push_back(QVariant(0));
     m_vecOverWarningTemp.push_back(QVariant(0));
     m_vecOverWarningTemp.push_back(QVariant("MB"));

     initSigConnect();
     m_winSetting->readConfig();
     m_winSetting->onlyFirstEmitSig();
     m_winSetting->show();

     setLabWidgetLayout(m_orientation);
     m_info->netInfo(m_upload, m_down);
     m_info->cpuInfo(m_vec);
}

void WinHoverNet::initSigConnect()
{
    //--------响应 "个性化" 发射的信号--------
    connect(m_winSetting, &WinSetting::sigUnitModel, this, &WinHoverNet::onUnitModel);
    connect(m_winSetting, &WinSetting::sigUnitModelIndex, this, &WinHoverNet::onUnitModelIndex);

    connect(m_winSetting, &WinSetting::sigLabUploadText, this, &WinHoverNet::onLabUploadText);
    connect(m_winSetting, &WinSetting::sigLabDownText, this, &WinHoverNet::onLabDownText);
    connect(m_winSetting, &WinSetting::sigLabCpuText, this, &WinHoverNet::onLabCpuText);
    connect(m_winSetting, &WinSetting::sigLabMemoryText, this, &WinHoverNet::onLabMemoryText);

    connect(m_winSetting, &WinSetting::sigCurrentFont, this, &WinHoverNet::onCurrentFont);
    connect(m_winSetting, &WinSetting::sigFontSize, this, &WinHoverNet::onFontSize);
    // 此处是事件过滤器，修改颜色 3+1

    //--------响应 "常规配置" 发射的信号--------
//    connect(m_winSetting, &WinSetting::sigCurrystemStyle, this, &WinHoverNet::onCurrystemStyle);
//    connect(m_winSetting, &WinSetting::sigCurrystemStyleText, this, &WinHoverNet::onCurrystemStyleText);
    // 悬浮窗口 暂时空
    connect(m_winSetting, &WinSetting::sigDisolayNet, this, &WinHoverNet::onDisolayNet);
    connect(m_winSetting, &WinSetting::sigDisolayCPUAndMemory, this, &WinHoverNet::onDisolayCPUAndMemory);
    connect(m_winSetting, &WinSetting::sigLocationExchangeNet, this, &WinHoverNet::onLocationExchangeNet);
    connect(m_winSetting, &WinSetting::sigLocationExchangeCPUAndMenory, this, &WinHoverNet::onLocationExchangeCPUAndMenory);
    connect(m_winSetting, &WinSetting::sigFractionalAccuracy, this, &WinHoverNet::onFractionalAccuracy);
    connect(m_winSetting, &WinSetting::sigRefreshInterval, this, &WinHoverNet::onRefreshInterval);

    connect(m_winSetting, &WinSetting::sigCpuOver, this, &WinHoverNet::onCpuOver);
    connect(m_winSetting, &WinSetting::sigMemOver, this, &WinHoverNet::onMemOver);
    connect(m_winSetting, &WinSetting::sigCpuOverNum, this, &WinHoverNet::onCpuOverNum);
    connect(m_winSetting, &WinSetting::sigMemOverNum, this, &WinHoverNet::onMemOverNum);

    connect(m_winSetting, &WinSetting::sigShowModel, this, &WinHoverNet::onShowModel);

//    connect(m_winSetting, &WinSetting::sigBtnApplyWinMain, this, &WinHoverNet::onBtnApplyWinMain);

//   connect(m_winSetting, &WinSetting::sigLabTextColor, this, &WinHoverNet::onLabTextColor);
//   connect(m_winSetting, &WinSetting::sigTextColor, this, &WinHoverNet::onTextColor);

//    connect(m_winSetting, &WinSetting::sigLabDiskReadText, this, &WinHoverNet::onLabDiskReadText);
//    connect(m_winSetting, &WinSetting::sigLabDiskWriteText, this, &WinHoverNet::onLabDiskWriteText);
//    connect(m_winSetting, &WinSetting::sigDisolayDisk, this, &WinHoverNet::onDisolayDisk);
//    connect(m_winSetting, &WinSetting::sigLocationExchangeDisk, this, &WinHoverNet::onLocationExchangeDisk);
//   connect(m_winSetting, &WinSetting::sigHoverDisplay, this, &WinHoverNet::onHoverDisplay);
}

/*!
 * \brief WinHoverNet::setLabWidgetLayout 设置 dde 的插件布局（水平 、垂直）
 * \param[in] isHorizontal true 水平； false 垂直
 */
void WinHoverNet::setLabWidgetLayout(bool isHorizontal)
{
    if (isHorizontal)
        m_orientation = Qt::Horizontal;
    else
        m_orientation = Qt::Vertical;

    setLabWidgetLayout(m_orientation);
}

/*!
 * \brief WinHoverNet::setLabWidgetLayout 设置 dde 的插件布局（水平 、垂直）
 * \param[in] orientation 希望设置为的布局方式
 */
void WinHoverNet::setLabWidgetLayout(Qt::Orientation orientation)
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
//            // 调试布局代码
//            m_vecLabel[i]->setAutoFillBackground(true);
//            m_vecLabel[i + 1]->setAutoFillBackground(true);
//            m_vecLabel[i + 2]->setAutoFillBackground(true);
//            m_vecLabel[i + 3]->setAutoFillBackground(true);
//            QPalette pa = m_vecLabel[i]->palette();
//            pa.setColor(QPalette::Background, Qt::green);
//            m_vecLabel[i]->setPalette(pa);
//            pa.setColor(QPalette::Background, Qt::red);
//            m_vecLabel[i + 1]->setPalette(pa);
//            pa.setColor(QPalette::Background, Qt::blue);
//            m_vecLabel[i + 2]->setPalette(pa);
//            pa.setColor(QPalette::Background, Qt::gray);
//            m_vecLabel[i + 3]->setPalette(pa);

            int iTo2 = i % 2;
            m_gridLayout->addWidget(m_vecLabel[i], 0, i / 2 + iTo2, Qt::AlignLeft | Qt::AlignBottom);
            m_gridLayout->addWidget(m_vecLabel[i + 1], 0, (i + 1) / 2 + (i + 1) % 2, Qt::AlignRight | Qt::AlignBottom);
            m_gridLayout->addWidget(m_vecLabel[i + 2], 1, i / 2 + iTo2, Qt::AlignLeft | Qt::AlignTop);
            m_gridLayout->addWidget(m_vecLabel[i + 3], 1, (i + 1) / 2 + (i + 1) % 2, Qt::AlignRight | Qt::AlignTop);

            const int space = -7;
            m_vecLabel[i]->setContentsMargins(0, 0, 0, space);
            m_vecLabel[i + 1]->setContentsMargins(0, 0, 0, space);
            m_vecLabel[i + 2]->setContentsMargins(0, space, 0, 0);
            m_vecLabel[i + 3]->setContentsMargins(0, space, 0, 0);

        }
    } else {
        for (int i = 0; i < m_vecLabel.count(); i += 2) {
            m_gridLayout->addWidget(m_vecLabel[i], i, 0, Qt::AlignLeft | Qt::AlignVCenter);
            m_gridLayout->addWidget(m_vecLabel[i + 1], i, 1, Qt::AlignRight | Qt::AlignVCenter);

            m_vecLabel[i]->setContentsMargins(0, 0, 0, 0);
            m_vecLabel[i + 1]->setContentsMargins(0, 0, 0, 0);
        }
    }
}

//bool WinHoverNet::isHoverDisplay()
//{
//    return m_hover;
//}

///*!
// * \brief WinHoverNet::DataOverWarning 当 cpu、mem、net 勾选预警提示，并且超过各自指定预警值，弹出提示窗口
// * \param tile 弹窗标题
// * \param text 弹窗内容
// * \param isTransient 是否为临时消息（默认显示 4000 ms 后自动消失）
// * \param ms 临时弹窗消息，默认显示时间毫秒数
// */\
//void WinHoverNet::DataOverWarning(QString title, QString text, QWidget *parent, bool isTransient, int ms)
//{
//    if (isTransient) {
//        QMessageBox *msg = new QMessageBox(QMessageBox::Information, title, text);
//        msg->setWindowFlags(Qt::WindowStaysOnTopHint);
//        QTimer::singleShot(ms, msg, &QMessageBox::close);
//        msg->exec();
//    } else {
//        QMessageBox::information(parent, title, text);
//    }
//}

/*!
 * \brief WinHoverNet::showTest 测试函数
 * \param str 打印自定义字符串
 */
void WinHoverNet::showTest(QString str)
{
    qDebug()<<"=================================#" + str << "===>"<<m_vecOverWarning
           <<m_vecOverWarning.size()<<m_vecOverWarning.capacity();

    qDebug()<<"=================================#" + str << "===>"<<m_vecOverWarningTemp
           <<m_vecOverWarningTemp.size()<<m_vecOverWarningTemp.capacity();
}

//QString WinHoverNet::hoverDisplayText()
//{
//    double run = 0;
//    double idle = 0;
//    if (m_hover) {
//        m_info->systemRunTime(run, idle);
//        return m_info->runTimeUnit(run);
//    } else {
//        return "";
//    }
//}

/*!
 * \brief WinHoverNet::onNet 定时刷新网速
 */
void WinHoverNet::onNet()
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
 * \brief WinHoverNet::onCpu 定时刷新 CPU
 */
void WinHoverNet::onCpu()
{
    QVector<CpuInfo> vec;
    m_info->cpuInfo(vec);

    double valCpu = (vec.begin()->cpuWork - m_vec.begin()->cpuWork) * 100.0 / (vec.begin()->cpuAll - m_vec.begin()->cpuAll);
    m_vecLabel[5]->setText(QString("%1%").arg(valCpu, 0, 'f', m_precision, QLatin1Char(' ')));

    m_vec.begin()->cpuWork = vec.begin()->cpuWork;
    m_vec.begin()->cpuAll = vec.begin()->cpuAll;

//    if (m_vecOverWarning[0].toBool()) {
//        QString title("CPU 提示");
//        QString text("");
//        bool ok = false;
//        int overNum = m_vecOverWarning[3].toInt(&ok);
//        if (ok)
//            text = "CPU 使用率超过" + QString::number(overNum) + "%";

//        if (valCpu >= overNum) {
//            m_vecOverWarning[0].setValue(false);
//            DataOverWarning(title, text, m_winSetting);
//        }
//    }
}

/*!
 * \brief WinHoverNet::onMemory 定时刷新 内存
 */
void WinHoverNet::onMemory()
{
    MemoryInfo info;
    m_info->memoryInfo(info);

    double mem = (info.memoryAll - info.memoryFree) * 100.0 / info.memoryAll;
    m_vecLabel[7]->setText(QString("%1%").arg(mem, 0, 'f', m_precision, QLatin1Char(' ')));

//    if (m_vecOverWarning[1].toBool()) {
//        QString title("内存提示");
//        QString text("");
//        bool ok = false;
//        int overNum = m_vecOverWarning[4].toInt(&ok);
//        if (ok)
//            text = "内存使用率超过" + QString::number(overNum) + "%";

//        if (mem >= overNum) {
//            DataOverWarning(title, text, m_winSetting);
//            m_vecOverWarning[1].setValue(false);
//        }
//    }
}

///*!
// * \brief WinHoverNet::onSystemRunTime 定时刷新 系统开机运行时间
// */
//void WinHoverNet::onSystemRunTime()
//{
//    double run = 0;
//    double idle = 0;
//    m_info->systemRunTime(run, idle);
//    //    ui->lab_32->setText(m_info->runTimeUnit(run));
//}

void WinHoverNet::onCurrentFont(const QFont &font)
{
    int nCount = m_gridLayout->count();
    for (int i = 0; i < nCount; ++i) {
        QLayoutItem *it = m_gridLayout->itemAt(i);
        QLabel * lab = static_cast<QLabel *>(it->widget());
        lab->setFont(font);
    }
}

void WinHoverNet::onFontSize(int size)
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

//void WinHoverNet::onCurrystemStyle(int index)
//{

//}

//void WinHoverNet::onCurrystemStyleText(QString text)
//{
//    json& jStyle = m_js["GeneralSetting"]["systemStyle"];
//    jStyle["SystemStyle"] = text;
//}

/*!
 * \brief WinHoverNet::onShowModel 插件水平还是垂直
 * \param[in] check 现实模式中水平 (radioHorizontal) 控件是否被选中
 * \line            true 水平； false 垂直
 */
void WinHoverNet::onShowModel(bool check)
{
    setLabWidgetLayout(check);
}

/*!
 * \brief WinHoverNet::onUnitModel 单位显示模式
 */
void WinHoverNet::onUnitModel(const QString &text)
{
    if (text == "Default")
        m_modelUnit = ModelUnit::Default;
    else if (text == "Upper")
        m_modelUnit = ModelUnit::Upper;
    else if (text == "Lower")
        m_modelUnit = ModelUnit::Lower;
    else if (text == "Mixed")
        m_modelUnit = ModelUnit::Mixed;
}

void WinHoverNet::onUnitModelIndex(int index)
{
    if (index == 0)
        m_modelUnit = ModelUnit::Default;
    else if (index == 1)
        m_modelUnit = ModelUnit::Mixed;
    else if (index == 2)
        m_modelUnit = ModelUnit::Upper;
    else if (index == 3)
        m_modelUnit = ModelUnit::Lower;
}

//void WinHoverNet::onLabTextColor(const QColor color)
//{
//    // TODO: 2021-01-08 加一行，颜色相同就返回, 结尾出，后面添加磁盘功能
//    QPalette palette;
//    palette.setColor(QPalette::WindowText, color);
//    m_vecLabel[0]->setPalette(palette);
//    m_vecLabel[2]->setPalette(palette);
//    m_vecLabel[4]->setPalette(palette);
//    m_vecLabel[6]->setPalette(palette);
//}

//void WinHoverNet::onTextColor(const QColor color)
//{
//    // TODO: 2021-01-08 加一行，颜色相同就返回, 结尾出，后面添加磁盘功能
//    QPalette palette;
//    palette.setColor(QPalette::WindowText, color);
//    m_vecLabel[1]->setPalette(palette);
//    m_vecLabel[3]->setPalette(palette);
//    m_vecLabel[5]->setPalette(palette);
//    m_vecLabel[7]->setPalette(palette);
//}

void WinHoverNet::onLabUploadText(const QString &text)
{
    if (m_vecLabel[0]->text() != text)
        m_vecLabel[0]->setText(text);
}

void WinHoverNet::onLabDownText(const QString &text)
{
    if (m_vecLabel[2]->text() != text)
        m_vecLabel[2]->setText(text);
}

void WinHoverNet::onLabCpuText(const QString &text)
{
    if (m_vecLabel[4]->text() != text)
        m_vecLabel[4]->setText(text);
}

void WinHoverNet::onLabMemoryText(const QString &text)
{
    if (m_vecLabel[6]->text() != text)
        m_vecLabel[6]->setText(text);
}

void WinHoverNet::onDisolayNet(bool check)
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

void WinHoverNet::onDisolayCPUAndMemory(bool check)
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

void WinHoverNet::onLocationExchangeNet(bool check)
{
    Q_UNUSED(check)

    QLayoutItem* itemLabUp = m_gridLayout->itemAtPosition(0, 0);
    QLayoutItem* itemUp = m_gridLayout->itemAtPosition(0, 1);
    QLayoutItem* itemLabDown = m_gridLayout->itemAtPosition(1, 0);
    QLayoutItem* itemDown = m_gridLayout->itemAtPosition(1, 1);
    QLabel* labUp = static_cast<QLabel *>(itemLabUp->widget());
    QLabel* up = static_cast<QLabel *>(itemUp->widget());
    QLabel* labDown = static_cast<QLabel *>(itemLabDown->widget());
    QLabel* down = static_cast<QLabel *>(itemDown->widget());

    // 交换: 先移除, 后添加
    m_gridLayout->removeWidget(labUp);
    m_gridLayout->removeWidget(up);
    m_gridLayout->removeWidget(labDown);
    m_gridLayout->removeWidget(down);

    m_gridLayout->addWidget(labDown, 0, 0);
    m_gridLayout->addWidget(down, 0, 1);
    m_gridLayout->addWidget(labUp, 1, 0);
    m_gridLayout->addWidget(up, 1, 1);
}

void WinHoverNet::onLocationExchangeCPUAndMenory(bool check)
{
    Q_UNUSED(check)

    QLayoutItem* itemLabCpu = m_gridLayout->itemAtPosition(0, 2);
    QLayoutItem* itemCpu = m_gridLayout->itemAtPosition(0, 3);
    QLayoutItem* itemLabMem = m_gridLayout->itemAtPosition(1, 2);
    QLayoutItem* itemMem = m_gridLayout->itemAtPosition(1, 3);
    QLabel* labCpu = static_cast<QLabel *>(itemLabCpu->widget());
    QLabel* cpu = static_cast<QLabel *>(itemCpu->widget());
    QLabel* labMem = static_cast<QLabel *>(itemLabMem->widget());
    QLabel* mem = static_cast<QLabel *>(itemMem->widget());

    // 交换: 先移除, 后添加
    m_gridLayout->removeWidget(labCpu);
    m_gridLayout->removeWidget(cpu);
    m_gridLayout->removeWidget(labMem);
    m_gridLayout->removeWidget(mem);

    m_gridLayout->addWidget(labMem, 0, 2);
    m_gridLayout->addWidget(mem, 0, 3);
    m_gridLayout->addWidget(labCpu, 1, 2);
    m_gridLayout->addWidget(cpu, 1, 3);
}

void WinHoverNet::onFractionalAccuracy(int num)
{
    if (m_precision != num)
        m_precision = num;
}

void WinHoverNet::onRefreshInterval(int interval)
{
    if (m_timer->interval() != interval)
        m_timer->setInterval(interval);
}

//void WinHoverNet::onHoverDisplay(bool check)
//{
//    if (m_hover != check)
//        m_hover = check;
//}

void WinHoverNet::onCpuOver(bool check)
{
    m_vecOverWarningTemp[0].setValue(check);
    onBtnApplyWinMain();
    showTest("onCpuOver");
}

void WinHoverNet::onMemOver(bool check)
{
    m_vecOverWarningTemp[1].setValue(check);
    onBtnApplyWinMain();
    showTest("onMemOver");
}

void WinHoverNet::onCpuOverNum(int cpu)
{
    m_vecOverWarningTemp[3].setValue(cpu);
    showTest("onCpuOverNum");
}

void WinHoverNet::onMemOverNum(int mem)
{
    m_vecOverWarningTemp[4].setValue(mem);
    showTest("onMemOverNum");
}

void WinHoverNet::onBtnApplyWinMain()
{
    m_vecOverWarning = m_vecOverWarningTemp;
}


