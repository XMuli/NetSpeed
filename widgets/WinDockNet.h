#ifndef WINDOCKNET_H
#define WINDOCKNET_H

#include "../lfx_global.h"
#include <QWidget>
#include "../lib/MonitorInfo_x11.h"

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

private:
    long m_upload;  // 网速的上次数值
    long m_down;

    Ui::WinDockNet *ui;
    MonitorInfo_x11 *m_info;
    ModelUnit m_modelUnit;
    QTimer *m_timer;
};

#endif // WINDOCKNET_H
