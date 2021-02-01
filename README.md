# lfxNet

<p align="right"><br><a href="README.en_US.md">English</a> | <a href="README.md">简体中文</a> | <a href="README.zh_TW.md">繁體中文</a></p>

 ![](https://img.shields.io/badge/language-c++-orange.svg) ![](https://img.shields.io/badge/language-Qt-orange.svg) ![](https://img.shields.io/github/license/xmuli/lfxNet) ![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/xmuli/lfxNet)  

 ![GitHub All Releases](https://img.shields.io/github/downloads/xmuli/lfxNet/total)  ![GitHub stars](https://img.shields.io/github/stars/xmuli/lfxNet?style=social) ![GitHub forks](https://img.shields.io/github/forks/xmuli/lfxNet?style=social) 

<br>

> `lfxNet` 是一款轻量、快速的实时显示系统资源信息的应用程序。

<img src="docs/lfxMonitorNetHor.gif" alt="lfxMonitorNetHor" width="65%">  <img src="docs/lfxMonitorNetVer.gif" alt="lfxMonitorNetVer" width="30%">

<br>

## 目录

- [背景](#背景)
- [编译](#编译)
- [下载](#下载)
- [作者](#作者)
- [鸣谢](#鸣谢)
- [协议](#协议)

<br>

### 背景

​		喜爱 `DDE` ，为 `Deepin` 爱好者、也是开发者之一。因习惯其它系统上有一个任务栏网速插件，但在 `Deepin/UOS`上没有相似的，故项目诞生。

<br>

​		在 `Linux`、`Windows`、`MacOS` 上提供一个桌面固定透明窗口，让此窗口协调于系统桌面，可实时显示网速、CPU 、内存、磁盘等活跃状态。同时单独提供 `Win10` 和 `DDE (为 lfxMonitorNet)` 的任务栏实时网速插件。

<br>

**lfxNet：** 制作一个固定在桌面的 `CPU` 使用率透明窗口（默认不打开）

<img src="docs/RunWindow.gif" alt="RunWindow" width="40%">

<br>

**lfxMonitorNet：** 

> **详细展示和运行效果，参见 💘  [README](https://github.com/xmuli/lfxNet/tree/master/dde-dock-plugin)** 

​		是 [lfxspeed](https://github.com/xmuli/lfxspeed) 的重构版本，起初，仅自用查看实时网速，后开源在社区被大家喜欢，甚是开心；后决定重构加扩展其为 [lfxsNet](https://github.com/xmuli/lfxsNet)，目标先写成通用的 `Linux` 、后空闲扩展到 `Win` 和 `Mac` 平台，可显示网速、CPU、MEM 等信息。

<img src="docs/DockSetting.png" alt="DockSetting" width="80%">

<br>

### 编译

采进入源码目录后，执行：

```bash
sudo apt install g++ cmake libqt5*-dev libdtk{core,widget,gui}-dev dde-dock-dev # 依赖

mkdir build && cd build # 编译
cmake ..
make -j16

sudo make install # 安装
```

**注意：** 若未生效，执行 `killall dde-dock` 重启任务栏后生效

<br>

若是帮助到了你，<font color=#FE7207  size=4 face="幼圆">可以点击该项目的的<img src="https://raw.githubusercontent.com/xmuli/xmuliPic/pic/2021/20210128152407.png" height="18" width="18"/>   **Star** 和<img src="https://raw.githubusercontent.com/xmuli/xmuliPic/pic/2021/20210128152444.png" height="18" width="18"/>  **Fork** 的两个图标，方便抬手之间，相当点个赞，</font>手有余香。

<br>

### 下载

可自行执行命令编译安装，也可下载对应平台安装包安装：[地址](https://github.com/xmuli/lfxNet/releases/tag/v0.9.2)  

<br>

### 作者

[偕臧](https://github.com/xmuli) 

<br>

### 鸣谢

[流年匆忙](https://github.com/justforlxz) 、 [zccrs](https://github.com/zccrs) 

<br>

### 协议

<img src="docs/MIT.png" alt="MIT" width="15%">

该项目基于 `MIT` 协议进行分发和使用。 有关更多信息，请参阅 [协议文件](/LICENSE)。



