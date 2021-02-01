#ifndef WINDOCKNET_H
#define WINDOCKNET_H

#include "../lfx_global.h"
#include "../lib/MonitorInfo_x11.h"
#include <QWidget>
#include <QVector>

LFX_USE_NAESPACE

namespace Ui {
class WinDockNet;
}

class QTimer;
class WinDockNet : public QWidget
{
    Q_OBJECT

public:
    explicit WinDockNet(QWidget *parent = nullptr);
    ~WinDockNet();
    void init();

public slots:
    void onNet();
    void onCpu();
    void onMemory();
    void onSystemRunTime();

private:
    long m_upload;  // 网速的上次数值
    long m_down;
    QVector<CpuInfo> m_vec;
    int m_precision; // 精度

    Ui::WinDockNet *ui;
    MonitorInfo_x11 *m_info;
    ModelUnit m_modelUnit;
    QTimer *m_timer;
};

#endif // WINDOCKNET_H
