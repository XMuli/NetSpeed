#include "WinAbout.h"
#include "ui_WinAbout.h"

WinAbout::WinAbout(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinAbout)
{
    ui->setupUi(this);
}

WinAbout::~WinAbout()
{
    delete ui;
}
