#include <QApplication>
#include <QLoggingCategory>
#include "widgets/WinSetting.h"
#include "lib/MonitorInfo_x11.h"
#include "dde-dock-plugin/WinDockNet.h"
#include "dde-dock-plugin/WinDdeDockSetting.h"


//using namespace lfx;
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
    QLoggingCategory::setFilterRules("*.debug=true\nqt.*.debug=false");
//    QT_LOGGING_RULES=qt.qpa.input= false;
//    QLoggingCategory::setFilterRules("*.debug=true\nqt.*.debug=false");
//    WinSetting setting;
//    setting.show();

//    QVector<CpuInfo> vec;
//    MonitorInfo_x11 *info = new MonitorInfo_x11();

//    info->cpuInfo(vec);
//    long upload = 0;
//    long down = 0;
//    info->netInfo(upload, down);

//    MemoryInfo mem;
//    info->memoryInfo(mem);

//    WinDockNet *winDockNet = new WinDockNet();
//    winDockNet->resize(500, 600);
//    winDockNet->show();
    WinDdeDockSetting *setting = new WinDdeDockSetting();
    setting->readConfig();
    setting->show();


//    WinDockNet *net = new WinDockNet();
//    net->show();

    return QApplication::exec();
}
