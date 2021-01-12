#ifndef WINDDEDOCKSETTING_H
#define WINDDEDOCKSETTING_H

#include <QButtonGroup>
#include <QWidget>
#include "../lib/MonitorInfo_x11.h"
LFX_USE_NAESPACE
#include "../json.hpp"
using json = nlohmann::json;

namespace Ui {
class WinDdeDockSetting;
}

class WinDdeDockSetting : public QWidget
{
    Q_OBJECT

public:
    explicit WinDdeDockSetting(QWidget *parent = nullptr);
    ~WinDdeDockSetting();

    void init();
    void initSigConnectWinDdeDock();
    void initSigConnectWinMain();
    void readConfigWinDdeDock();
    void saveConfigWinDdeDock();
    void readConfigWinMain();
    void saveConfigWinMain();
    bool isHorizontal();
    void updateLabelText(bool isHorizontal);

    virtual bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    // WinDdeDock 相关
    void sigCurrentFont(const QFont &);
    void sigFontSize(int);
    void sigLabTextColor(const QColor);
    void sigTextColor(const QColor);
    void sigBackgroundColor(const QColor);

    void sigShowModel(bool);
    void sigUnitModel(const QString &);
    void sigLabUploadText(const QString &);
    void sigLabDownText(const QString &);
    void sigLabCpuText(const QString &);
    void sigLabMemoryText(const QString &);
    void sigLabDiskReadText(const QString &);
    void sigLabDiskWriteText(const QString &);

    void sigDisolayNet(bool);
    void sigDisolayCPUAndMemory(bool);
    void sigDisolayDisk(bool);
    void sigLocationExchangeNet(bool);
    void sigLocationExchangeCPUAndMenory(bool);
    void sigLocationExchangeDisk(bool);

    void sigFractionalAccuracy(int);
    void sigRefreshInterval(int);
    void sigHoverDisplay(bool);

public slots:
    void onBtnSave(bool check);
    void onBtnQuit(bool check);

    void onBootUpUpdate(bool check);
//    void onCheckUpdate(bool check);
    void onChangePath();
    void onCpuOver(bool check);
//    void onMemOver(bool check);
//    void onNetOver(bool check);
    void onCpuOverNum(int cpu);
    void onMemOverNum(int mem);
    void onNetOverNum(int net);
    void onNetNumUnit(const QString &unit);

    void onBtnGroupTheme(int index, bool checked);
    void onStyle(int index);

private:
    Ui::WinDdeDockSetting *ui;
    json m_js;
    bool m_isHorizontal; // 插件的水平、垂直状态
    QString m_path;      // 数据保存路径
    QButtonGroup *m_btnGroupTheme;

    int m_cpuOverNum;
    int m_memOverNum;
    int m_NetOverNum;
    QString m_netOverUnit;
};

#endif // WINDDEDOCKSETTING_H
