#include <QApplication>
#include "lib/MonitorInfo_x11.h"
#include "WinTransparent.h"
#include "WinDesktop.h"
#include "ProgressLabel.h"
#include "dde-dock-plugin/WinDockNet.h"
#include "dde-dock-plugin/WinDdeDockSetting.h"
#include "WinSetting.h"
#include "WinHoverNet.h"

LFX_USE_NAESPACE
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);

    // TEST:
//    WinDdeDockSetting *setting = new WinDdeDockSetting();
//    setting->readConfigWinDdeDock();
//    setting->show();

//    WinTransparent *winTrans = new WinTransparent();
//    winTrans->show();

    // 鼠标中键关闭（后面加个鼠标穿透固定在桌面）

//    WinSetting *winSetting = new WinSetting();
//    winSetting->show();

    WinHoverNet *hoverNet = WinHoverNet::getInstance();
    hoverNet->show();
//    WinSetting *net = new WinSetting();
//    net->show();

    return QApplication::exec();
}
