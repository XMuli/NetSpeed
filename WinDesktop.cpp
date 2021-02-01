#include "WinDesktop.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QTimer>
#include <QWidget>
#include <QPushButton>
#include "ProgressLabel.h"


/*!
 * \brief WinDesktop::WinDesktop
 *
 * \htmlonly
 * <pre style="font-family: FreeMono, Consolas, Menlo, 'Noto Mono', 'Courier New', Courier, monospace;line-height: 100%;">
 * ==============================
 * ||      CPU 各核占用率       ||
 * ==============================
 * </pre>
 * \endhtmlonly
 * \param parent
 */
WinDesktop::WinDesktop(QWidget *parent)
    : WinTransparent(parent)
    , m_mainLayout(new QVBoxLayout(this))
    , m_info(new MonitorInfo_x11())
    , m_timer(new QTimer())
{
    init();
}

void WinDesktop::init()
{
    m_mainLayout->setMargin(6);
    m_mainLayout->setSpacing(10);

    m_info->cpuInfo(m_vec);
    bool isBegin = true;
    for (auto v : m_vec) {
        QWidget *widget = new QWidget();
        widget->setContentsMargins(0, 0, 0, 0);
        widget->setMaximumHeight(12);
//        QPalette pal = widget->palette();  // Test
//        pal.setColor(QPalette::Background, Qt::green);
//        widget->setAutoFillBackground(true);
//        widget->setPalette(pal);

        QHBoxLayout *hLayout = new QHBoxLayout(widget);
        hLayout->setMargin(0);
        hLayout->setSpacing(0);

        QString text("");
        if (isBegin) {
            text = QString("CPU") + ": ";
            isBegin = false;
        } else {
            text = QString("CPU") + QString::number(v.index) + ": ";
        }

        QLabel *lab = new QLabel(text);
        lab->setMargin(0);
        lab->setMinimumWidth(50);
        ProgressLabel *progress = new ProgressLabel();

//        QPalette pal2 = widget->palette();    // Test
//        pal2.setColor(QPalette::Background, Qt::gray);
//        progress->setAutoFillBackground(true);
//        progress->setPalette(pal2);
        progress->winDestktopPtr(this);
        progress->m_val = v.cpuWork;
        progress->m_all = v.cpuAll;

        QLabel *percentage = new QLabel("00.00%");
        percentage->setMargin(0);
        percentage->setMinimumWidth(55);
        percentage->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

        hLayout->addWidget(lab, 1, Qt::AlignLeft);
        hLayout->addWidget(progress, 9); // 不能加 Qt::AlignLeft
        hLayout->addWidget(percentage, 1, Qt::AlignRight);
        m_mainLayout->addWidget(widget);
    }
    m_mainLayout->addStretch(10);

    connect(m_timer, &QTimer::timeout, this, &WinDesktop::onUpdate);
    m_timer->setInterval(1000);
    m_timer->start();

    resize(300, 400);
}


QVBoxLayout* WinDesktop::mainLayout()
{
    return m_mainLayout;
}

void WinDesktop::onUpdate()
{
    auto list = this->findChildren<ProgressLabel *>();
    QVector<CpuInfo> vec;
    m_info->cpuInfo(vec);

    for (int i = 0; i < list.size(); i += 3) {
        int num = i + 1;
        int numNext = i + 2;
        int index = i / 3;

        list[num]->m_val = vec[index].cpuWork - m_vec[index].cpuWork;
        list[num]->m_all = vec[index].cpuAll - m_vec[index].cpuAll;
//        qDebug()<<"---------------------------->" << i << num << list[num]->m_val <<  list[num]->m_all;

        list[numNext]->setText(QString::number(list[num]->m_val * 100.0  / list[num]->m_all, 'f', 2) + "%");
        m_vec[index].cpuWork = vec[index].cpuWork;
        m_vec[index].cpuAll = vec[index].cpuAll;

    }

    emit sigValChange();
}
