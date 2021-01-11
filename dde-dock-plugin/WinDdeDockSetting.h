#ifndef WINDDEDOCKSETTING_H
#define WINDDEDOCKSETTING_H

#include <QWidget>
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
    void readConfig();
    void saveConfig();
    bool isHorizontal();

    virtual bool eventFilter(QObject *watched, QEvent *event) override;

signals:
    void sigCurrentFont(const QFont &);
    void sigFontSize(int);
    void sigLabTextColor(const QColor);
    void sigTextColor(const QColor);
    void sigBackgroundColor(const QColor);

    void sigShowModel(bool);
    void sigUnitModel(int);
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

public:
    Ui::WinDdeDockSetting *ui;

private:
    json m_js;
};

#endif // WINDDEDOCKSETTING_H
