//
// Created by xmuli on 2020/12/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_WinAbout.h" resolved

#include "WinAbout.h"
#include "ui_WinAbout.h"

WinAbout::WinAbout(QWidget *parent) :
        QWidget(parent), ui(new Ui::WinAbout) {
    ui->setupUi(this);
}

WinAbout::~WinAbout() {
    delete ui;
}
