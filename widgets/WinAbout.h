#ifndef WINABOUT_H
#define WINABOUT_H

#include <QWidget>

namespace Ui {
class WinAbout;
}

class WinAbout : public QWidget
{
    Q_OBJECT

public:
    explicit WinAbout(QWidget *parent = nullptr);
    ~WinAbout();

private:
    Ui::WinAbout *ui;
};

#endif // WINABOUT_H
