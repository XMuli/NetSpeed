// SPDX-SnippetCopyrightText: 2020-2025 Wengeng Zhang
// SPDX-Author: Wengeng Zhang <xmulitech@gmail.com>
// SPDX-License-Identifier: MIT
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
