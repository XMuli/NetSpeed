#ifndef WINSETTING_H
#define WINSETTING_H

#include <QButtonGroup>
#include <QDir>
#include <QWidget>
#include "lib/MonitorInfo_x11.h"
LFX_USE_NAESPACE
#include "json.hpp"
using json = nlohmann::json;

namespace Ui {
class WinSetting;
}

class WinSetting : public QWidget
{
    Q_OBJECT

public:
    explicit WinSetting(QWidget *parent = nullptr);
    ~WinSetting();

public:
    void init();
    void readConfig();
    void readConfig(bool isHomePath);
    void saveConfig();
    void saveConfig(bool &isHomePath);
    void saveConfigThemeIsLight(bool isLight);
    void onlyFirstEmitSig();

    bool isHorizontal();
    bool isLightTheme();

    QString getConfigPath(bool &isHomePath);
    QString getConfigPath(QString homePath, QString systemPath, bool &isHomePath);

    void writeDataToConfigPath();
    bool writeDataToConfigPath(QString sour, QString dest, QString sourName, QString destName);

    void initSigConnectPersonalization();
    void initSigConnectGeneralSetting();

signals:
    // --------个性化 相关--------
    void sigUnitModel(const QString &);
    void sigUnitModelIndex(int);

    void sigLabUploadText(const QString &);
    void sigLabDownText(const QString &);
    void sigLabCpuText(const QString &);
    void sigLabMemoryText(const QString &);

    void sigCurrentFont(const QFont &);
    void sigFontSize(int);
    // 此处是事件过滤器，修改颜色 3+1
    void sigTheme(bool checked);


    // --------常规配置 相关--------
    void sigCurrystemStyle(int index);
    void sigCurrystemStyleText(const QString &);
    // 悬浮窗口 暂时空
    void sigDisolayNet(bool);
    void sigDisolayCPUAndMemory(bool);
    void sigLocationExchangeNet(bool);
    void sigLocationExchangeCPUAndMenory(bool);
    void sigFractionalAccuracy(int);
    void sigRefreshInterval(int);

    void sigCpuOver(bool check);
    void sigMemOver(bool check);
    void sigCpuOverNum(int cpu);
    void sigMemOverNum(int mem);

    void sigShowModel(bool);

public slots:
    void onTheme(bool checked);
    void onBtnApplyToJson();
    void onBtnApplyWinSetting();
    void onBtnQuitWinSetting();

    void onCurrystemStyle(int index);
    void onCurrystemStyleText(QString text);

private:
    Ui::WinSetting *ui;
    json m_js;
    bool m_isHorizontal; // 插件的水平、垂直状态
    QString m_path;      // 数据保存路径
};

#endif // WINSETTING_H
