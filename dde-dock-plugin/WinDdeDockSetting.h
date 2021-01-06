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

    void readConfig();
    void saveConfig();


private:
    Ui::WinDdeDockSetting *ui;
};

#endif // WINDDEDOCKSETTING_H
