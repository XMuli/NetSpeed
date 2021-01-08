#include "WinDockNet.h"
#include "ui_WinDockNet.h"

#include <QTimer>
#include <QDebug>


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
WinDockNet::WinDockNet(WinDdeDockSetting *winSetting, QWidget *parent)
    : m_winSetting(winSetting)
    , QWidget(parent)
    , ui(new Ui::WinDockNet)
    , m_info(new MonitorInfo_x11())
    , m_timer(new QTimer())
    , m_modelUnit(Default)
    , m_precision(2)
//    , m_hover(true)
{
    ui->setupUi(this);

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
    delete ui;
}

void WinDockNet::init()
{
    m_winSetting->readConfig();

    m_info->netInfo(m_upload, m_down);
    m_info->cpuInfo(m_vec);

//    setAutoFillBackground(true);  // 暂时不设置背景颜色

    connect(m_winSetting, &WinDdeDockSetting::sigCurrentFont, this, &WinDockNet::onCurrentFont);
    connect(m_winSetting, &WinDdeDockSetting::sigFontSize, this, &WinDockNet::onFontSize);
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


}


bool WinDockNet::setPtrWinDdeDockSetting(WinDdeDockSetting *winSetting)
{
    if (winSetting == nullptr) {
        return false;
    } else {
        m_winSetting = winSetting;
        return true;
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
    ui->lab_12->setText(QString("%1").arg(increaseUpload, 0, 'f', m_precision, QLatin1Char(' ')) + unit);

    netUnit = Byte;
    double increaseDown = m_info->netShowUnit((down - m_down) / (m_timer->interval() / 1000.0), netUnit);
    unit = m_info->netModelUnit(netUnit, m_modelUnit);
    ui->lab_22->setText(QString("%1").arg(increaseDown, 0, 'f', m_precision, QLatin1Char(' ')) + unit);

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
    ui->lab_14->setText(QString("%1%").arg(valCpu, 0, 'f', m_precision, QLatin1Char(' ')));
}

/*!
 * \brief WinDockNet::onMemory 定时刷新 内存
 */
void WinDockNet::onMemory()
{
    MemoryInfo info;
    m_info->memoryInfo(info);

    double mem = (info.memoryAll - info.memoryFree) * 100.0 / info.memoryAll;
    ui->lab_24->setText(QString("%1%").arg(mem, 0, 'f', m_precision, QLatin1Char(' ')));
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
    // TODO: 2021-01-08 修改插件字体
}

void WinDockNet::onFontSize(int size)
{
    // TODO: 2021-01-08 修改插件字体大小
}

void WinDockNet::onTextColor(const QColor color)
{
    // TODO: 2021-01-08 加一行，颜色相同就返回, 结尾出，后面添加磁盘功能
    QPalette palette;
    palette.setColor(QPalette::WindowText, color);
    ui->lab_12->setPalette(palette);
    ui->lab_14->setPalette(palette);
    ui->lab_22->setPalette(palette);
    ui->lab_24->setPalette(palette);
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
    if (ui->lab_11->text() != text)
        ui->lab_11->setText(text);
}

void WinDockNet::onLabDownText(const QString &text)
{
    if (ui->lab_21->text() != text)
        ui->lab_21->setText(text);
}

void WinDockNet::onLabCpuText(const QString &text)
{
    if (ui->lab_13->text() != text)
        ui->lab_13->setText(text);
}

void WinDockNet::onLabMemoryText(const QString &text)
{
    if (ui->lab_23->text() != text)
        ui->lab_23->setText(text);
}

void WinDockNet::onDisolayNet(bool check)
{
    if (check) {
        ui->lab_11->show();
        ui->lab_12->show();
        ui->lab_21->show();
        ui->lab_22->show();
    } else {
        ui->lab_11->hide();
        ui->lab_12->hide();
        ui->lab_21->hide();
        ui->lab_22->hide();
    }

}

void WinDockNet::onDisolayCPUAndMemory(bool check)
{
    if (check) {
        ui->lab_13->show();
        ui->lab_14->show();
        ui->lab_23->show();
        ui->lab_24->show();
    } else {
        ui->lab_13->hide();
        ui->lab_14->hide();
        ui->lab_23->hide();
        ui->lab_24->hide();
    }
}

void WinDockNet::onLocationExchangeNet(bool check)
{
    Q_UNUSED(check)
    QLabel labLab(ui->lab_11->text());
    QLabel labText(ui->lab_12->text());

    ui->lab_11->setText(ui->lab_21->text());
    ui->lab_12->setText(ui->lab_22->text());
    ui->lab_21->setText(labLab.text());
    ui->lab_22->setText(labText.text());
}

void WinDockNet::onLocationExchangeCPUAndMenory(bool check)
{
    Q_UNUSED(check)
    QLabel labLab(ui->lab_13->text());
    QLabel labText(ui->lab_14->text());

    ui->lab_13->setText(ui->lab_23->text());
    ui->lab_14->setText(ui->lab_24->text());
    ui->lab_23->setText(labLab.text());
    ui->lab_24->setText(labText.text());
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


