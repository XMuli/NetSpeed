# lfxMonitorNet

<p align="right"><br><a href="README.en_US.md">English</a> | <a href="README.md">简体中文</a> | <a href="README.zh_TW.md">繁體中文</a></p>

 ![](https://img.shields.io/badge/language-c++-orange.svg) ![](https://img.shields.io/badge/language-Qt-orange.svg) ![](https://img.shields.io/github/license/xmuli/lfxNet) ![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/xmuli/lfxNet)  

 ![GitHub All Releases](https://img.shields.io/github/downloads/xmuli/lfxNet/total)  ![GitHub stars](https://img.shields.io/github/stars/xmuli/lfxNet?style=social) ![GitHub forks](https://img.shields.io/github/forks/xmuli/lfxNet?style=social) 

<br>

> `lfxMonitorNet`  是 `dde-dock` 的网速插件，也是 [lfxspeed](https://github.com/xmuli/lfxspeed) 的重构版本。

​		起初，仅自用查看实时网速，后开源在社区被大家喜欢，甚是开心；后决定重构加扩展其为 [lfxsNet](https://github.com/xmuli/lfxsNet)，目标先写成通用的 `Linux` 、后空闲扩展到 `Win` 和 `Mac` 平台，可显示网速、CPU、MEM 等信息。 

<img src="../docs/lfxMonitorNetHor.gif" alt="lfxMonitorNetHor" width="65%"> <img src="../docs/lfxMonitorNetVer.gif" alt="lfxMonitorNetVer" width="30%">

<br>

## 目录

- [背景](#背景)
- [功能](#功能)
- [编译](#编译)
- [截图](#截图)
- [作者](#作者)
- [鸣谢](#鸣谢)
- [协议](#协议)

<br>

### 背景

​		重构之前的[旧项目](https://github.com/xmuli/lfxspeed)，修复诸多缺陷，和新添加许多新功能。当处于正式 `v1.0` 版本，才算一个达到我的预期版本，心里才觉得算是写好了。

<br>

### 功能

- 支持功能：
  - 高分屏显示
  - 用户自定义配置
  - 为系统所有用户安装
  - 显示的前缀内容和颜色
  - 显示的标签内容和颜色
  - 更改显示字体和字体大小
  - 适配 `dock` 的模式（水平和垂直）
  - 自动切换单位（默认智能）
  - 调整网络速率精确度
  - 调整刷新时间间隔
  - 显示额外显示的悬浮信息
  - 左键单击显示的动作（可单击打开 “系统监视器”）
  - 配置数据导出（可系统路径 | 用户路径）
  - `CPU`、内存、的利用率；流量的使用预警
  - 应用的主题样式（浅色和暗色模式）
  - 更改系统所自带的所有风格选择
  

<br>

### 编译

采用 `cmake` 构建，进入源码目录后，执行如下：

```bash
sudo apt install g++ cmake libqt5*-dev libdtk{core,widget,gui}-dev dde-dock-dev # 依赖

mkdir build && cd build # 编译
cmake ..
make -j16

sudo make install # 安装
```

或直接使用 `IDE` 点击运行按钮，找到生成目录 `/dde-dock-plugin` 下，手动复制共享库： `sudo cp liblfxMonitorNet.so /usr/lib/dde-dock/plugins` ；

<br>

**注意：** 若未生效，执行 `killall dde-dock` 重启任务栏后生效

<br>

### 截图

设置个性化，更多截图效果参见：[链接](https://github.com/xmuli/lfxNet/tree/master/docs) 

<img src="../docs/lfxMonitorNet.png" alt="lfxMonitorNet" width="100%">

 <img src="../docs/DockSetting.png" alt="DockSetting" width="75%">

 <img src="../docs/Config.png" alt="Config" width="75%">

 <img src="../docs/About.png" alt="About" width="75%">

<br>

### 作者

[偕臧](https://github.com/xmuli) 

<br>

### 鸣谢

[流年匆忙](https://github.com/justforlxz) 、 [zccrs](https://github.com/zccrs) 、

<br>

### 协议

<img src="../docs/MIT.png" alt="MIT" width="15%">

该项目基于 `MIT` 协议进行分发和使用。 有关更多信息，请参阅 [协议文件](../LICENSE)。



