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

    virtual bool eventFilter(QObject *watched, QEvent *event) override;

public slots:
    void onBtnSave(bool check);
    void onBtnQuit(bool check);

private:
    Ui::WinDdeDockSetting *ui;
    json m_js;
};

#endif // WINDDEDOCKSETTING_H
