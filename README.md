# NetSpeed

<p align="right"><br><a href="README.en_US.md">English</a> | <a href="README.md">简体中文</a> | <a href="README.zh_TW.md">繁體中文</a></p>

 ![](https://img.shields.io/badge/language-c++-orange.svg) ![](https://img.shields.io/badge/language-Qt-orange.svg) ![](https://img.shields.io/github/license/xmuli/NetSpeed) ![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/xmuli/NetSpeed)  

 ![GitHub All Releases](https://img.shields.io/github/downloads/xmuli/NetSpeed/total)  ![GitHub stars](https://img.shields.io/github/stars/xmuli/NetSpeed?style=social) ![GitHub forks](https://img.shields.io/github/forks/xmuli/NetSpeed?style=social) 

<br>

> NetSpeed 是一款轻量的实时网速悬浮窗口。

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

### NetSpeed

​		 `Linux` 上提供一个悬浮网速窗口，可实时显示网速、CPU 、内存信息（通用）。



**功能：**

1. 多语言切换，智能单位切换
2. 自定义网速标签 / 背景标签内容、颜色、精准度、字体、字体大小、位置互换、是否显示
3. 自带两套浅色 / 暗色两套主题、切换程序风格
4. 支持水平和垂直两种模式、窗口置顶
5. 自定义数据导出
6. 欢迎帮助完善

<img src="https://cdn.jsdelivr.net/gh/xmuli/xmuliPic@pic/2021/20210316-162021.png" width="100%"/>

<img src="https://cdn.jsdelivr.net/gh/xmuli/xmuliPic@pic/2021/20210316-162022.png" width="100%"/>

<br>

### NetPlugin

​		一款轻便、快速的任务栏网速插件（仅 DDE）。姑且是 [lfxspeed](https://github.com/xmuli/lfxspeed) 的重构版本。 

<img src="https://cdn.jsdelivr.net/gh/xmuli/xmuliPic@pic/2021/20210316-162023.gif" width="60%"/> <img src="https://cdn.jsdelivr.net/gh/xmuli/xmuliPic@pic/2021/20210316-162024.gif" width="35%"/>

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

若是帮助到了你，<font color=#FE7207  size=4 face="幼圆">可以点击该项目的的<img src="https://raw.githubusercontent.com/xmuli/xmuliPic/pic/2021/20210128152407.png" height="18" width="18"/>   **Star** 和<img src="https://raw.githubusercontent.com/xmuli/xmuliPic/pic/2021/20210128152444.png" height="18" width="18"/>  **Fork** 的两个图标，方便抬手之间，相当点个赞，</font>手有余香。

<br>

### 卸载

```bash
sudo rm -rf /usr/lib/dde-dock/plugins/libNetPlugins.so ~/.config/NetSpeed /usr/share/NetSpeed
```

<br>

### 下载

可自行执行命令编译安装，也可下载对应平台安装包安装：[地址](https://github.com/xmuli/lfxNet/releases/tag/v0.9.2)  

<br>

### 作者

[偕臧](https://github.com/xmuli) 

<br>

### 鸣谢

[流年匆忙](https://github.com/justforlxz) 、 [zccrs](https://github.com/zccrs) ，期待更多人帮助改善，如果你喜欢且恰好有时间

<br>

### 协议

<img src="docs/MIT.png" alt="MIT" width="15%">

该项目基于 `MIT` 协议进行分发和使用。 有关更多信息，请参阅 [协议文件](/LICENSE)。



