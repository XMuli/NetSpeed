#ifndef WINDOCKNET_H
#define WINDOCKNET_H

#include "../lfx_global.h"
#include "../lib/MonitorInfo_x11.h"
#include <QWidget>
#include <QVector>
#include "WinDdeDockSetting.h"

LFX_USE_NAESPACE

namespace Ui {
class WinDockNet;
}

class QTimer;
class WinDockNet : public QWidget
{
    Q_OBJECT

public:
    explicit WinDockNet(WinDdeDockSetting *winSetting = nullptr, QWidget *parent = nullptr);
    ~WinDockNet();

    void init();
    bool setPtrWinDdeDockSetting(WinDdeDockSetting *winSetting);

public slots:
    // 响应本身
    void onNet();
    void onCpu();
    void onMemory();
    void onSystemRunTime();

    // 响应 WinDdeDockSetting 发射的信号
    void onCurrentFont(const QFont &font);
    void onFontSize(int size);
    void onTextColor(const QColor color);
    void onBackgroundColor(const QColor color);
    void onLabUploadText(const QString &text);
    void onLabDownText(const QString &text);
    void onLabCpuText(const QString &text);
    void onLabMemoryText(const QString &text);
//    void onLabDiskReadText(const QString &text);
//    void onLabDiskWriteText(const QString &text);

    void onDisolayNet(bool check);
    void onDisolayCPUAndMemory(bool check);
//    void onDisolayDisk(bool check);
    void onLocationExchangeNet(bool check);
    void onLocationExchangeCPUAndMenory(bool check);
//    void onLocationExchangeDisk(bool check);

    void onFractionalAccuracy(int num);
    void onRefreshInterval(int interval);
//    void sigHoverDisplay(bool check);

private:
    long m_upload;   // 网速的上次数值
    long m_down;
    QVector<CpuInfo> m_vec;
    int m_precision; // 精确度
//    bool m_hover;    // 悬浮现实额外信息

    Ui::WinDockNet *ui;
    MonitorInfo_x11 *m_info;
    ModelUnit m_modelUnit;
    QTimer *m_timer;

    WinDdeDockSetting *m_winSetting;
};

#endif // WINDOCKNET_H
