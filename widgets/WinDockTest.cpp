#include "WinDockTest.h"
#include "ui_WinDockTest.h"

WinDockTest::WinDockTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinDockTest)
{
    ui->setupUi(this);
}

WinDockTest::~WinDockTest()
{
    delete ui;
}
