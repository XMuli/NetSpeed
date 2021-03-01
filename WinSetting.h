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

//    int doubleClick();

    void init();
    void readConfig();
    void saveConfig();

    void cpoyToConfigPath();
    bool isHorizontal();
    bool isLightTheme();

    void writeDataToConfigPath();
    bool writeDataToConfigPath(QString sour, QString dest, QString sourName, QString destName);
    QString isInHomePath(QString homePath, QString systemPath, bool isHomePath = true);

public slots:
    void onBtnApplyToJson();


private:
    Ui::WinSetting *ui;
    json m_js;
    bool m_isHorizontal; // 插件的水平、垂直状态
    QString m_path;      // 数据保存路径
};

#endif // WINSETTING_H
