#include "WinDockNet.h"
#include "ui_WinDockNet.h"

#include <QTimer>

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
WinDockNet::WinDockNet(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinDockNet),
    m_info(new MonitorInfo_x11()),
    m_timer(new QTimer()),
    m_modelUnit(Default)
{
    ui->setupUi(this);

    init();
    connect(m_timer, &QTimer::timeout, this, &WinDockNet::onNet);
    connect(m_timer, &QTimer::timeout, this, &WinDockNet::onCpu);
    connect(m_timer, &QTimer::timeout, this, &WinDockNet::onMemory);
    m_timer->setInterval(1000);
    m_timer->start();
}

WinDockNet::~WinDockNet()
{
    delete ui;
}

void WinDockNet::init()
{
    m_info->netInfo(m_upload, m_down);
}

/*!
 * \brief WinDockNet::onNet 时刻刷新网速
 */
void WinDockNet::onNet()
{
    long upload = 0;
    long down = 0;
    NetUnit netUnit = Byte;
    QString unit = "";

    m_info->netInfo(upload, down);
    double increaseUpload  = m_info->netShowUnit((upload - m_upload) / (m_timer->interval() / 1000.0), netUnit);
    unit = m_info->netModelUnit(netUnit, m_modelUnit);
    ui->lab_12->setText(QString("%1").arg(increaseUpload, 0, 'f', 2, QLatin1Char(' ')) + unit);

    netUnit = Byte;
    double increaseDown = m_info->netShowUnit((down - m_down) / (m_timer->interval() / 1000.0), netUnit);
    unit = m_info->netModelUnit(netUnit, m_modelUnit);
    ui->lab_22->setText(QString("%1").arg(increaseDown, 0, 'f', 2, QLatin1Char(' ')) + unit);

    m_upload = upload;
    m_down = down;
}

/*!
 * \brief WinDockNet::onCpu 时刻刷新 CPU
 */
void WinDockNet::onCpu()
{

}

/*!
 * \brief WinDockNet::onMemory 时刻刷新 内存
 */
void WinDockNet::onMemory()
{

}


