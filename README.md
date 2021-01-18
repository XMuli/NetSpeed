# lfxNet

<p align="right"><br><a href="README.md">English</a> | <a href="README.zh_CN.md">简体中文</a> | <a href="README.zh_TW.md">繁體中文</a></p>

`lfxNet` 是一款轻量、快速的显示实时系统资源的应用程序。

 `lfxMonitorNet` 是其子项目，可在 `DDE` 任务栏中，实时显示 “网速、CPU、内存” 等信息，附加自定义颜色、主题、相关流量预警提示等功能。

<img src="docs/lfxMonitorNetHor.gif" alt="lfxMonitorNetHor" width="65%">  <img src="docs/lfxMonitorNetVer.gif" alt="lfxMonitorNetVer" width="30%">

<br>

## 目录

- [背景](#背景)
- [编译](#编译)
- [卸载](#卸载)
- [维护者](#维护者)
- [许可协议](#许可协议)

<br>

### 背景

​		喜欢 `DDE` ，为 `Deepin` 爱好者、也是开发者之一。因习惯其它系统上有一个任务栏网速插件，但在 `Deepin/UOS`上没有相似的，故项目诞生。

​		项目愿景：最终提供 `Linux`、`Windows`、`MacOS` 上通用提供一个桌面固定透明窗口，让此窗口协调融于系统桌面，可实时显示网速、CPU 、内存、磁盘等活跃状态。同时单独提供 `Win10` 和 `DDE (为 lfxMonitorNet)` 的任务栏实时网速插件。

<br>

### lfxMonitorNet （DDE）

​		起初，仅自己方便实时查看网速，后开源在 [lfxspeed](https://github.com/xmuli/lfxspeed) 。并在社区收到留言表达感谢和喜欢、缺陷反馈等，与大家交流中甚是喜悦；曾有言，后面修复，仔细考虑后，决定重构为 [lfxsNet](https://github.com/xmuli/lfxsNet)，先写成通用的 Linux 、然后有时间再扩展至 Win 和 Mac 系类。能够达到完美的跨平台，一个萝卜可插多个坑，都可实时现实系统信息等状态

<img src="docs/lfxMonitorNet.png" alt="lfxMonitorNet" width="100%">

<br>

### 编译

采用 `cmake` 构建，进入源码目录后，执行如下：

```bash
mkdir build && cd build
cmake ..
make -j16
sudo make install
```

或直接使用 `IDE` 点击运行按钮，找到生成目录 `/dde-dock-plugin` 下，手动复制共享库： `sudo cp liblfxMonitorNet.so /usr/lib/dde-dock/plugins` ；

<br>

**注意：** 若未生效，执行 `killall dde-dock` 重启任务栏后生效

**开发文档：**  [Deepin 开发任务栏网速插件 lfxSpeed](https://xmuli.tech/posts/c225b552/) or [link](https://xmuli.blog.csdn.net/article/details/110672252)

<br>

### 卸载

任意路径下执行如下：

```bash
sudo rm -rf /usr/lib/dde-dock/plugins/liblfxMonitorNet.so
```

<br>

### 开发者

[@偕臧](https://github.com/xmuli)

<br>

### 许可协议

![](https://img.shields.io/github/license/xmuli/lfxSpeed) [MIT](LICENSE) © xmuli <偕臧>

