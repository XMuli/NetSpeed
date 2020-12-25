//
// Created by xmuli on 2020/12/25.
//

// You may need to build the project (run Qt uic code generator) to get "ui_WinSetting.h" resolved

#include "WinSetting.h"
#include "ui_WinSetting.h"

WinSetting::WinSetting(QWidget *parent) :
        QWidget(parent), ui(new Ui::WinSetting) {
    ui->setupUi(this);
}

WinSetting::~WinSetting() {
    delete ui;
}
