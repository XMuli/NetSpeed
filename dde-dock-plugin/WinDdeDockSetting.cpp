#include "WinDdeDockSetting.h"
#include "ui_WinDdeDockSetting.h"
//#include <QFile>
//#include <QByteArray>
//#include <QJsonDocument>
//#include <QJsonParseError>
//#include <QDebug>
#include <fstream>
#include <iostream>
using namespace std;

WinDdeDockSetting::WinDdeDockSetting(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WinDdeDockSetting)
{
    ui->setupUi(this);
}

WinDdeDockSetting::~WinDdeDockSetting()
{
    delete ui;
}

/*!
 * \brief WinDdeDockSetting::readConfig 从 config.json 读取配置写入到 UI 控件显示
 */
void WinDdeDockSetting::readConfig()
{
//    QFile file("/home/xmuli/project/github/lfxNet/config.json");
////    qDebug()<<"current applicationDirPath: "<<QCoreApplication::applicationDirPath()<<QApplication::applicationFilePath();
////    qDebug()<<"current currentPath: "<<QDir::currentPath()<<QDir::currentPath();
//    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
//        qErrnoWarning( + "../config.json  don't open!");
//        return;
//    }

//    QByteArray byteArr = file.readAll();
//    QJsonParseError error;
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArr, &error);

//    if (error.error != QJsonParseError::NoError) {
//        qErrnoWarning(error.errorString().toStdString().c_str());
//    }

    json js;
    ifstream jfile("/home/xmuli/project/github/lfxNet/config.json");
    jfile >> js;

    cout<<"--->@1@"<<js<<endl;
    cout<<"--->@2@"<<js["WinDdeDock"]<<endl;

//    json jj = json::parse("/home/xmuli/project/github/lfxNet/config.json");
//    cout<<"--->@3@"<<jj<<endl;

}
