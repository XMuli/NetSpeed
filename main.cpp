#include <QApplication>
#include "lib/MonitorInfo_x11.h"
#include "widgets/WinDockNet.h"
#include "dde-dock-plugin/WinDockNet.h"
#include "dde-dock-plugin/WinDdeDockSetting.h"

int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // TEST:
//    WinDdeDockSetting *setting = new WinDdeDockSetting();
//    setting->readConfigWinDdeDock();
//    setting->show();

//    WinDockNet *dockNet = new WinDockNet();
//    dockNet->show();

    return QApplication::exec();
}
