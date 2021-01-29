#include "WinDesktop.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QDebug>
#include <QTimer>
#include "ProgressLabel.h"


WinDesktop::WinDesktop(QWidget *parent)
    : WinTransparent(parent)
    , mainLayout(new QVBoxLayout(this))
    , m_info(new MonitorInfo_x11())
    , m_timer(new QTimer())
{
    init();
}

void WinDesktop::init()
{
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setMargin(6);

    m_info->cpuInfo(m_vec);
    for (auto v : m_vec) {
        QHBoxLayout *hLayout = new QHBoxLayout();
        hLayout->setContentsMargins(0, 0, 0, 0);
        hLayout->setMargin(0);

        QLabel *lab = new QLabel(QString("CPU") + QString::number(v.index) + ": ");
        ProgressLabel *progress = new ProgressLabel();
        progress->winDestktopPtr(this);
        progress->m_val = v.cpuWork;
        progress->m_all = v.cpuAll;
        QLabel *percentage = new QLabel(QString::number(v.cpuWork * 100.0 / v.cpuAll) + "%" );

        hLayout->addWidget(lab);
        hLayout->addWidget(progress);
        hLayout->addWidget(percentage);
        mainLayout->addLayout(hLayout);
    }

    connect(m_timer, &QTimer::timeout, this, &WinDesktop::onUpdate);
    m_timer->setInterval(1000);
    m_timer->start();
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

//    int n = 0;
//    for (auto v : list) {
//        int num = n / 3;
//        if (n % 3 == 1) {
//            v->m_val = vec[num].cpuWork - m_vec[num].cpuWork;
//            v->m_all = vec[num].cpuAll - m_vec[num].cpuAll;

//            m_vec[num].cpuWork = vec[num].cpuWork;
//            m_vec[num].cpuAll = vec[num].cpuAll;
//            qDebug()<<"--abc-------------------------->"<< n << num << v->m_val << v->m_all;

//        } else if (n % 3 == 2){
//            num = (n - 2) / 3;

////            v->m_val = vec[num].cpuWork - m_vec[num].cpuWork;
////            v->m_all = vec[num].cpuAll - m_vec[num].cpuAll;

//            qDebug()<<"--abc2-------------------------->"<< n << num << v->m_val << v->m_all;
//            v->setText(QString::number(v->m_val * 100  / v->m_all) + "%" );
////            m_vec[num].cpuWork = vec[num].cpuWork;
////            m_vec[num].cpuAll = vec[num].cpuAll;
//        }

//         if (++n >= 3 * m_vec.count() - 1)
//             break;
//    }

    emit sigValChange();
}
