#ifndef WINHOVERNET_H
#define WINHOVERNET_H

#include "lfx_global.h"
#include "lib/MonitorInfo_x11.h"
#include <QWidget>
#include <QVector>
#include <QLabel>
#include <QGridLayout>
#include "WinSetting.h"
#include "WinTransparent.h"
LFX_USE_NAESPACE

class QTimer;
class WinHoverNet : public WinTransparent
{
    Q_OBJECT

private:
    explicit WinHoverNet(Qt::Orientation orientation = Qt::Horizontal, QWidget *parent = nullptr);

public:
    static WinHoverNet* getInstance();
    ~WinHoverNet();


    void init();
    void initSigConnect();
    void setLabWidgetLayout(bool isHorizontal);
    void setLabWidgetLayout(Qt::Orientation orientation);

    bool setSingleInstance();

public slots:
    // 响应本身
    void onNet();
    void onCpu();
    void onMemory();

    // 响应 个性化 发射的信号
    void onUnitModel(const QString &text);
    void onUnitModelIndex(int index);

    void onLabUploadText(const QString &text);
    void onLabDownText(const QString &text);
    void onLabCpuText(const QString &text);
    void onLabMemoryText(const QString &text);

    void onCurrentFont(const QFont &font);
    void onFontSize(int size);

    void onLabTextColor(const QColor color);
    void onTextColor(const QColor color);
    void onBackgroundColor(const QColor color);
    void onBackgroundImage(const QColor color);

    // 响应 基础配置 发射的信号
    //    void onCurrystemStyle(int index);
    //    void onCurrystemStyleText(QString text);
    void onWindowTop(bool check);

    void onDisolayNet(bool check);
    void onDisolayCPUAndMemory(bool check);
    void onLocationExchangeNet(bool check);
    void onLocationExchangeCPUAndMenory(bool check);
    void onFractionalAccuracy(int num);
    void onRefreshInterval(int interval);

    void onShowModel(bool check);

protected:
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
//        virtual bool eventFilter(QObject *watched, QEvent *event) override;

private:
    WinSetting *m_winSetting;                // UI 界面控件
    long m_upload;                           // 网速的上次数值
    long m_down;
    QVector<CpuInfo> m_vec;                  // 获取 CPU 信息

    MonitorInfo_x11 *m_info;
    ModelUnit m_modelUnit;                   // 网速单位模式
    QTimer *m_timer;                         // 定时刷新内容
    int m_precision;                         // 精确度
    Qt::Orientation m_orientation;           // 控件布局（水平 | 竖直）
    QGridLayout *m_gridLayout;               // 存放下面 8 个标签的布局
    QVector<QLabel *> m_vecLabel;            // 顺序：0上传标签、1上传；2下载标签、3下载；4CPU标签、5CPU；6Mem标签、7Mem；
};

#endif // WINHOVERNET_H
