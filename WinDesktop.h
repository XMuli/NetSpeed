#ifndef WINDESKTOP_H
#define WINDESKTOP_H
#include <QTimer>
#include <QVBoxLayout>
#include "WinTransparent.h"
#include "lfx_global.h"
#include "lib/MonitorInfo_x11.h"


LFX_USE_NAESPACE
class WinDesktop : public WinTransparent
{
    Q_OBJECT
public:
    explicit WinDesktop(QWidget *parent = nullptr);
    void init();
    QVBoxLayout* mainLayout();

signals:
    void sigValChange();

public slots:
    void onUpdate();

private:
    QVBoxLayout *m_mainLayout;
    MonitorInfo_x11 *m_info;
    QVector<CpuInfo> m_vec;
    QTimer *m_timer;
};

#endif // WINDESKTOP_H
