//
// Created by xmuli on 2020/12/28.
//

#include "MonitorInfo_x11.h"
#include <QFile>
#include <QString>
#include <QTextStream>
#include <QTextStream>
#include <QtMath>
#include <QObject>
#include <QDebug>

#define PROC_PATH_UPTIME    "/proc/uptime"      // "系统启动" 和 "系统空闲" 的时间
#define PROC_PATH_CPU       "/proc/stat"        // "CPU" 使用率 的状态
#define PROC_PATH_MEM       "/proc/meminfo"     // "内存" 和 "交换空间" 的状态
#define PROC_PATH_NET       "/proc/net/dev"     // "网速" 下载和上传 的状态
#define PROC_PATH_DISK      "/proc/diskstats"   // "磁盘" 读取和写入 的状态

LFX_BEGIN_NAESPACE

MonitorInfo_x11::MonitorInfo_x11()
{

}

/*!
 * \brief MonitorInfo_x11::systemRunTime 获取系统自开机的运行时间
 * \param[out] run 运行时间
 * \param[out] idle 闲置时间
 */
void MonitorInfo_x11::systemRunTime(double &run, double &idle)
{
    QFile file(PROC_PATH_UPTIME);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug()<<"\"/proc/uptime\" don't open!";
        return;
    }

    run = idle = 0;
    QTextStream stream(&file);
    QString line = stream.readLine().simplified();
    QStringList list = line.split(QRegExp("\\s{1,}"));
    if (!list.isEmpty()) {
        run = list[0].toDouble();
        idle = list[1].toDouble();
    }

    file.close();
}

/*!
 * \brief MonitorInfo_x11::cpuInfo 获取单个 CPU 信息
 * \param[out] vec 单个 CPU 信息
 * \see CpuInfo
 */
void MonitorInfo_x11::cpuInfo(QVector<CpuInfo> &vec)
{
    QFile file(PROC_PATH_CPU);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qErrnoWarning("\"/proc/stat\" don't open!");
        return;
    }

    QTextStream stream(&file);
    QString line = "";
    QStringList list = {};
    bool ok = false;
    CpuInfo info = {-1, 0, 0};
    int i = 0;

    do {
        line = stream.readLine().simplified();
        list = line.split(QRegExp("\\s{1,}"));

        for (auto v = list.begin() + 1; v != list.end(); ++v)
            info.cpuAll += v->toLong(&ok);

        info.cpuFree = list[4].toLong(&ok);
        info.index = i;

        vec.push_back(info);

    } while(i++, list[0] != "intr");

    vec.pop_back(); // 去掉多余
    file.close();
}

/*!
 * \brief MonitorInfo_x11::netInfo 获取网速信息
 * \param[out] upload 发送数据量（上传）
 * \param[out] down 接收数据量（下载）
 */
void MonitorInfo_x11::netInfo(long &upload, long &down)
{
    QFile file(PROC_PATH_NET);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {  // 在读取时，把行尾结束符修改为 '\n'； 在写入时，把行尾结束符修改为本地系统换行风格，比如Windows文本换行是 "\r\n"
        qErrnoWarning("\"/proc/net/dev\" don't open!");
        return;
    }

    upload = down = 0;
    long netUpload = 0;
    long netDown = 0;
    QTextStream stream(&file);
    QString line = "";
    line = stream.readLine().simplified();
    line = stream.readLine().simplified();
    line = stream.readLine().simplified();
    while (!line.isNull()) {
        QStringList list = line.split(QRegExp("\\s{1,}"));

        if (!list.isEmpty()) {
            netUpload = list[9].toLong();
            netDown = list[1].toLong();
        }

        upload += netUpload;
        down += netDown;
        line = stream.readLine().simplified();
    }

    file.close();
}

/*!
 * \brief MonitorInfo_x11::memoryInfo 获取内存、交换控件信息
 * \param[out] info 内存、交换控件信息
 */
void MonitorInfo_x11::memoryInfo(MemoryInfo &info)
{
    QFile file(PROC_PATH_MEM);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qErrnoWarning("\"/proc/meminfo\"  don't open!");
        return;
    }

    bool ok = false;
    QTextStream stream(&file);
    QString line = stream.readLine().simplified();
    while (!line.isNull()) {
        QStringList list = line.split(QRegExp("\\s{1,}"));
        if (list[0] == "MemTotal:")
            info.memoryAll = list[1].toLong(&ok);
        else if (list[0] == "MemAvailable:")
            info.memoryFree = list[1].toLong(&ok);
        else if (list[0] == "SwapTotal:")
            info.swapAll = list[1].toLong(&ok);
        else if (list[0] == "SwapFree:")
            info.swapFree = list[1].toLong(&ok);

        line = stream.readLine().simplified();
    }

    file.close();
}

/*!
 * \brief MonitorInfo_x11::netShowUnit 网速转换后的显示单位
 * \param[in] net 需转换的网速（默认为 Byte）
 * \param[out] model 转换后的单位模式
 * \return 转换单位后的网速数值
 *
 * ---------------------------------------------
 *  自动判断合适的速率单位,默认传进来的是 Byte       *
 *           bit    0 ~ 7 位（不到 1 字节）      *
 *           Byte   1    ~ 2^10  Byte          *
 *           KB     2^10 ~ 2^20  Byte          *
 *           MB     2^20 ~ 2^30  Byte          *
 *           GB     2^30 ~ 2^40  Byte          *
 *           TB     2^40 ~ 2^50  Byte          *
 * ---------------------------------------------
 *
 * 注意：请先将单位转为字节(Byte)后再传参
 */
double MonitorInfo_x11::netShowUnit(long net, MonitorInfo_x11::NetUnit &unit)
{
    double ret = 0;
    if (0 <= net && net < qPow(2, 10)) {
        unit = MonitorInfo_x11::Byte;
        ret = net;
    } else if (qPow(2, 10) <= net && net < qPow(2, 20)) {
        unit = MonitorInfo_x11::Kb;
        ret = static_cast<double>(net / qPow(2, 10) * 1.0);
    } else if (qPow(2, 20) <= net && net < qPow(2, 30)) {
        unit = MonitorInfo_x11::Mb;
        ret = static_cast<double>(net / qPow(2, 20) * 1.0);
    } else if (qPow(2, 30) <= net && net < qPow(2, 40)) {
        unit = MonitorInfo_x11::Gb;
        ret = static_cast<double>(net / qPow(2, 30) * 1.0);
    } else if (qPow(2, 40) <= net && net < qPow(2, 50)) {
        unit = MonitorInfo_x11::Tb;
        ret = static_cast<double>(net / qPow(2, 40) * 1.0);
    } else {
        unit = MonitorInfo_x11::Unknow;
        qWarning()<<QObject::tr("本设备网络速率单位传输超过 TB, 或者低于 1 Byte.");
        ret = -1;
    }

    return ret;
}

/*!
 * \brief MonitorInfo_x11::netModelUnit 转换后网速的单位模式
 * \param[in] unit 对应网速单位范围
 * \param[out] model 最终显示的网速模式
 * \return 该模式下的对应字符串文本
 */
QString MonitorInfo_x11::netModelUnit(NetUnit unit, ModelUnit &model)
{
    switch (model) {
    case MonitorInfo_x11::Default: {
        switch (unit) {
        case MonitorInfo_x11::Bit:
            return QString("b/s");
        case MonitorInfo_x11::Byte:
            return QString("B/s");
        case MonitorInfo_x11::Kb:
            return QString("Kb/s");
        case MonitorInfo_x11::Mb:
            return QString("Mb/s");
        case MonitorInfo_x11::Gb:
            return QString("Gb/s");
        case MonitorInfo_x11::Tb:
            return QString("Tb/s");
        default:
            qWarning()<<QObject::tr("ModelUnit::Default,  MonitorInfo_x11 is Unknow.");
            return QString("");
        }
    }
    case MonitorInfo_x11::Upper: {
        switch (unit) {
        case MonitorInfo_x11::Bit:
            return QString("BIT/S");
        case MonitorInfo_x11::Byte:
            return QString("B/S");
        case MonitorInfo_x11::Kb:
            return QString("KB/S");
        case MonitorInfo_x11::Mb:
            return QString("MB/S");
        case MonitorInfo_x11::Gb:
            return QString("GB/S");
        case MonitorInfo_x11::Tb:
            return QString("TB/S");
        default:
            qWarning()<<QObject::tr("ModelUnit::Upper,  MonitorInfo_x11 is Unknow.");
            return QString("");
        }
    }
    case MonitorInfo_x11::Lower: {
        switch (unit) {
        case MonitorInfo_x11::Bit:
            return QString("bit/s");
        case MonitorInfo_x11::Byte:
            return QString("b/s");
        case MonitorInfo_x11::Kb:
            return QString("kb/s");
        case MonitorInfo_x11::Mb:
            return QString("mb/s");
        case MonitorInfo_x11::Gb:
            return QString("gb/s");
        case MonitorInfo_x11::Tb:
            return QString("tb/s");
        default:
            qWarning()<<QObject::tr("ModelUnit::Lower,  MonitorInfo_x11 is Unknow.");
            return QString("");
        }
    }
    default: {
        qWarning()<<QObject::tr("ModelUnit::Custom,  MonitorInfo_x11 is Custom.");
        // TODO: 2020-12-29
        return QString("");
    }
    }
}

/*!
 * \brief MonitorInfo_x11::runTimeUnit 系统时间运行转换格式: "系统已运行: %1天, %2:%3:%4"
 * \param[in] s 秒
 * \return 预期显示格式
 */
QString MonitorInfo_x11::runTimeUnit(double s)
{
    int time = qFloor(s);
    int ss = time % 60;
    int MM = (time % 3600) / 60;
    int hh = (time % 86400) / 3600;
    int dd = time / 86400;

    QString runTime = QString(QObject::tr("系统已运行: %1天, %2:%3:%4"))
            .arg(dd, 0, 'f', 0, QLatin1Char(' '))
            .arg(hh, 2, 'f', 0, QLatin1Char('0'))
            .arg(MM, 2, 'f', 0, QLatin1Char('0'))
            .arg(ss, 2, 'f', 0, QLatin1Char('0'));

    return runTime;
}

LFX_END_NAESPACE
