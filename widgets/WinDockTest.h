#ifndef WINDOCKTEST_H
#define WINDOCKTEST_H

#include <QWidget>

namespace Ui {
class WinDockTest;
}

class WinDockTest : public QWidget
{
    Q_OBJECT

public:
    explicit WinDockTest(QWidget *parent = nullptr);
    ~WinDockTest();

private:
    Ui::WinDockTest *ui;
};

#endif // WINDOCKTEST_H
