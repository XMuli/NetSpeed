#ifndef WINDOCKNET_H
#define WINDOCKNET_H

#include "../lfx_global.h"
#include "../lib/MonitorInfo_x11.h"
#include <QWidget>
#include <QVector>
#include <QLabel>
#include <QGridLayout>
#include "WinDdeDockSetting.h"

LFX_USE_NAESPACE

class QTimer;
class WinDockNet : public QWidget
{
    Q_OBJECT

public:
    explicit WinDockNet(WinDdeDockSetting *winSetting = nullptr, Qt::Orientation orientation = Qt::Horizontal, QWidget *parent = nullptr);
    ~WinDockNet();

    WinDockNet* winDockNetObject();

    void init();
    void initSigConnect();
    void setLabWidgetLayout(bool isHorizontal);
    void setLabWidgetLayout(Qt::Orientation orientation);
    bool isHoverDisplay();
    void DataOverWarning(QString title, QString text, QWidget *parent = nullptr, bool isTransient = true, int ms = 1000 * 60 * 10);

    QString hoverDisplayText();

public slots:
    // 响应本身
    void onNet();
    void onCpu();
    void onMemory();
    void onSystemRunTime();

    // 响应 WinDdeDockSetting 发射的信号
    void onCurrentFont(const QFont &font);
    void onFontSize(int size);
    void onShowModel(bool check);
    void onUnitModel(const QString &text);
    void onUnitModelIndex(int index);
    void onLabTextColor(const QColor color);
    void onTextColor(const QColor color);
    void onLabUploadText(const QString &text);
    void onLabDownText(const QString &text);
    void onLabCpuText(const QString &text);
    void onLabMemoryText(const QString &text);

    void onDisolayNet(bool check);
    void onDisolayCPUAndMemory(bool check);
    void onLocationExchangeNet(bool check);
    void onLocationExchangeCPUAndMenory(bool check);

    void onFractionalAccuracy(int num);
    void onRefreshInterval(int interval);
    void onHoverDisplay(bool check);

private:
    long m_upload;   // 网速的上次数值
    long m_down;
    QVector<CpuInfo> m_vec;
    int m_precision; // 精确度
    bool m_hover;    // 悬浮现实额外信息

    MonitorInfo_x11 *m_info;
    ModelUnit m_modelUnit;
    QTimer *m_timer;              // 刷新时间

    WinDdeDockSetting *m_winSetting;
    Qt::Orientation m_orientation;
    QGridLayout *m_gridLayout;
    QVector<QLabel *> m_vecLabel; // 顺序：0上传标签、1上传；2下载标签、3下载；4CPU标签、5CPU；6Mem标签、7Mem；
};

#endif // WINDOCKNET_H
