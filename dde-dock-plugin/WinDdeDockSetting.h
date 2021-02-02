#ifndef WINDDEDOCKSETTING_H
#define WINDDEDOCKSETTING_H

#include <QButtonGroup>
#include <QDir>
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

public:
    bool isHorizontal();
    int doubleClick();
    void readConfigWinDdeDock();
    void saveConfigWinDdeDock();
    void readConfigWinMain();
    void saveConfigWinMain();

    void readConfig();
    void saveConfig();
    QString creatorConfigPath(QString path);
    bool writeDataToConfigPath(QString sour, QString dest, QString sourName, QString destName);
    void writeDataToConfigPath();
    QString configPath(QString systemPath, QString homePath, int &index);
    char* configPath(int &index, QString path = "");
    QString configPath();

private:
    void init();
    void initSigConnectWinDdeDock();
    void initSigConnectWinMain();
    void updateLabelText(bool isHorizontal);

    virtual bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    // WinDdeDock 相关
    void sigCurrentFont(const QFont &);
    void sigFontSize(int);
    void sigLabTextColor(const QColor);
    void sigTextColor(const QColor);

    void sigShowModel(bool);
    void sigUnitModel(const QString &);
    void sigUnitModelIndex(int);
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

    // WinMain 相关
    void sigCpuOver(bool check);
    void sigMemOver(bool check);
    void sigNetOver(bool check);
    void sigCpuOverNum(int cpu);
    void sigMemOverNum(int mem);
    void sigNetOverNum(int net);
    void sigNetNumUnit(const QString &unit);

    void sigBtnApplyWinMain();

public slots:
    void onBtnApplyWinDdeDock(bool check);
    void onBtnQuitWinDdeDock(bool check);
    void onBtnApplyWinMain(bool check);
    void onBtnQuitWinMain(bool check);

    void onDoubleClick(int index);
    void onBootUpUpdate(bool check);
//    void onCheckUpdate(bool check);
    void onChangePath();

    void onBtnGroupTheme(int index, bool checked);
    void onStyle(int index);

private:
    Ui::WinDdeDockSetting *ui;
    json m_js;
    bool m_isHorizontal; // 插件的水平、垂直状态
    QString m_path;      // 数据保存路径
    QButtonGroup *m_btnGroupTheme;
    int m_doubleClick;

    int m_cpuOverNum;
    int m_memOverNum;
    int m_NetOverNum;
    QString m_netOverUnit;
};

#endif // WINDDEDOCKSETTING_H
