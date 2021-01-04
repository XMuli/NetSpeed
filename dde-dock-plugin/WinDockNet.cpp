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
    m_info->netInfo(m_upload, m_down);
    m_info->cpuInfo(m_vec);
    m_precision = 2;
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
    double increaseUpload  = m_info->netShowUnit((upload - m_upload) / (m_timer->interval() / 1000.0), netUnit);
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


