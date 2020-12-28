#include <QApplication>
#include "widgets/WinSetting.h"
#include "lib/MonitorInfo_x11.h"


using namespace lfx;
int main(int argc, char *argv[]) {
    QApplication a(argc, argv);
//    WinSetting setting;
//    setting.show();

    QVector<CpuInfo> vec;
    MonitorInfo_x11 *info = new MonitorInfo_x11();

//    info->cpuInfo(vec);
//    long upload = 0;
//    long down = 0;
//    info->netInfo(upload, down);

    MemoryInfo mem;
    info->memoryInfo(mem);

    return QApplication::exec();
}
