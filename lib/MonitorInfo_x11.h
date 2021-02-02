//
// Created by xmuli on 2020/12/28.
//

#ifndef LFXNET_MONITORINFO_X11_H
#define LFXNET_MONITORINFO_X11_H

#include "../lfx_global.h"
#include <QVector>

LFX_BEGIN_NAESPACE

struct CpuInfo{
public:
    int index;     // CPU 序号 （若序号为 0, 则是总 CPU 信息）
    long cpuAll;   // CPU 总量
    long cpuWork;  // CPU 工作量
};

struct MemoryInfo{
public:
    long memoryAll;
    long memoryFree;
    long swapAll;
    long swapFree;
};

enum NetUnit {
    Bit,
    Byte,
    Kb,
    Mb,
    Gb,
    Tb,
    Unknow
};

enum ModelUnit {
    Default,   // 默认格式
    Mixed,     // 大小写混合
    Upper,     // 全部大写
    Lower      // 全部小写
};

class MonitorInfo_x11 {
public:
    MonitorInfo_x11();

    void systemRunTime(double &run, double &idle);
    void cpuInfo(QVector<CpuInfo> &vec);
    void netInfo(long &upload, long &down);
    void memoryInfo(MemoryInfo &info);

    double netShowUnit(long net, NetUnit &unit);
    QString netModelUnit(NetUnit unit, ModelUnit model);
    QString runTimeUnit(double s = 0);
};

LFX_END_NAESPACE
#endif //LFXNET_MONITORINFO_X11_H
