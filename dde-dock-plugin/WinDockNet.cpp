#include "WinDockNet.h"

#include <QTimer>
#include <QDebug>
#include <QObjectList>

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
    , m_modelUnit(Default)
    , m_precision(2)
    , m_orientation(Qt::Horizontal)
    , m_gridLayout(new QGridLayout(this))
    , m_vecLabel(8, nullptr)
//    , m_hover(true)
{
    init();
    connect(m_timer, &QTimer::timeout, this, &WinDockNet::onNet);
    connect(m_timer, &QTimer::timeout, this, &WinDockNet::onCpu);
    connect(m_timer, &QTimer::timeout, this, &WinDockNet::onMemory);
    connect(m_timer, &QTimer::timeout, this, &WinDockNet::onSystemRunTime);
    m_timer->setInterval(1000);
    m_timer->start();
}

WinDockNet::~WinDockNet()
{
}

void WinDockNet::init()
{
    for (auto it = m_vecLabel.begin(); it != m_vecLabel.end(); ++it)
        *it = new QLabel();

     m_gridLayout->setContentsMargins(0, 0, 0, 0);
     m_gridLayout->setSpacing(0);

    connect(m_winSetting, &WinDdeDockSetting::sigCurrentFont, this, &WinDockNet::onCurrentFont);
    connect(m_winSetting, &WinDdeDockSetting::sigFontSize, this, &WinDockNet::onFontSize);
    connect(m_winSetting, &WinDdeDockSetting::sigUnitModel, this, &WinDockNet::onUnitModel);
    connect(m_winSetting, &WinDdeDockSetting::sigShowModel, this, &WinDockNet::onShowModel);
    connect(m_winSetting, &WinDdeDockSetting::sigLabTextColor, this, &WinDockNet::onLabTextColor);
    connect(m_winSetting, &WinDdeDockSetting::sigTextColor, this, &WinDockNet::onTextColor);
    connect(m_winSetting, &WinDdeDockSetting::sigBackgroundColor, this, &WinDockNet::onBackgroundColor);
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
//    connect(m_winSetting, &WinDdeDockSetting::sigHoverDisplay, this, &WinDockNet::sigHoverDisplay);

//    setAutoFillBackground(true);  // 暂时不设置背景颜色

   m_winSetting->readConfig();

   setLabWidgetLayout(m_winSetting->isHorizontal());
   m_info->netInfo(m_upload, m_down);
   m_info->cpuInfo(m_vec);
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
        m_gridLayout->addWidget(m_vecLabel[0], 0 , 0);
        m_gridLayout->addWidget(m_vecLabel[1], 0 , 1);
        m_gridLayout->addWidget(m_vecLabel[2], 1 , 0);
        m_gridLayout->addWidget(m_vecLabel[3], 1 , 1);
        m_gridLayout->addWidget(m_vecLabel[4], 0 , 2);
        m_gridLayout->addWidget(m_vecLabel[5], 0 , 3);
        m_gridLayout->addWidget(m_vecLabel[6], 1 , 2);
        m_gridLayout->addWidget(m_vecLabel[7], 1 , 3);
    } else {
        m_gridLayout->addWidget(m_vecLabel[0], 0 , 0);
        m_gridLayout->addWidget(m_vecLabel[1], 0 , 1);
        m_gridLayout->addWidget(m_vecLabel[2], 1 , 0);
        m_gridLayout->addWidget(m_vecLabel[3], 1 , 1);
        m_gridLayout->addWidget(m_vecLabel[4], 2 , 0);
        m_gridLayout->addWidget(m_vecLabel[5], 2 , 1);
        m_gridLayout->addWidget(m_vecLabel[6], 3 , 0);
        m_gridLayout->addWidget(m_vecLabel[7], 3 , 1);
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

void WinDockNet::onBackgroundColor(const QColor color)
{
    if (palette().color(QPalette::Background) == color)
        return;

    QPalette pale = const_cast<QPalette&>(palette());
    pale.setColor(QPalette::Background, color);
    setPalette(pale);
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

//void WinDockNet::sigHoverDisplay(bool check)
//{
//    if (m_hover != check)
//        m_hover = check;
//}


