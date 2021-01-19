#include <QApplication>
#include <QLoggingCategory>
#include <QFile>
#include "widgets/WinSetting.h"
#include "lib/MonitorInfo_x11.h"
#include "dde-dock-plugin/WinDockNet.h"
#include "dde-dock-plugin/WinDdeDockSetting.h"


//using namespace lfx;
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // TEST:
    WinDdeDockSetting *setting = new WinDdeDockSetting();
    setting->readConfigWinDdeDock();
    setting->show();

//    QFile file("/home/xmuli/.config/lfxNet/config.json");
//    file.setPermissions(QFile::ReadOwner | QFile::WriteOwner
//                        | QFile::ReadGroup | QFile::WriteGroup
//                        | QFile::ReadOther | QFile::WriteOther);


    return QApplication::exec();
}
