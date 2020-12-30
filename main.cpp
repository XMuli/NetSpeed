#include <QApplication>
#include "widgets/WinSetting.h"
#include "lib/MonitorInfo_x11.h"
#include "widgets/WinDockNet.h"


//using namespace lfx;
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
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

    WinDockNet *winDockNet = new WinDockNet();
    winDockNet->resize(500, 600);
    winDockNet->show();

    return QApplication::exec();
}
