# NetSpeed

![](https://img.shields.io/github/license/XMuli/NetSpeed) ![](https://img.shields.io/github/v/release/XMuli/NetSpeed?style=flat&color=birightgreen) ![](https://img.shields.io/badge/powered%20by-XMuli-ff69b4) 

![](https://img.shields.io/github/languages/code-size/XMuli/NetSpeed)  ![](https://img.shields.io/github/downloads/XMuli/NetSpeed/total) ![](https://img.shields.io/github/stars/XMuli/NetSpeed?style=social) ![](https://img.shields.io/github/forks/XMuli/NetSpeed?style=social&label=Fork)

<br>

> **本项目已收录于 [Product](https://product.xmuli.tech/)**

<br>

## 目录

- [背景](#背景)

- [NetSpeed](#NetSpeed)

- [NetPlugin](#NetPlugin)

- [上架 App Store](#上架 App Store)

- [编译](#编译)

- [下载](#下载)

- [作者](#作者)

- [鸣谢](#鸣谢)

  

<br>

### 背景

喜爱 `DDE` ，为 `Deepin` 爱好者、也是开发者之一。因习惯其它系统上有一个任务栏网速插件，但在 `Deepin/UOS`上没有相似的，故项目诞生。 本项目支持在 Deepin V20.9、等环境的 UOS 或其它任何 Linux 发行版。此项目为  [lfxSpeed](https://github.com/XMuli/lfxSpeed) 项目的重写和功能延续。

<br>

### NetSpeed

NetSpeed 是一款轻量的实时网速悬浮窗口，在国产操作系统 UOS / Deepin V20.9 上，支持原生 DDE-DOCK 的网速插件形式；在其它 Linux 发行版上提供独立悬浮网速窗口，可实时显示网速、CPU 、内存。



本项目博文实现原理：[Deepin 任务栏网速插件推荐 NetSpeed (重构 lfxSpeed)](https://xmuli.blog.csdn.net/article/details/113573870)                   [镜像](https://xmuli.tech/posts/164f4705/)



**功能：**

1. 多语言切换，智能单位切换
2. 自定义网速标签 / 背景标签内容、颜色、精准度、字体、字体大小、位置互换、是否显示
3. 自带两套浅色 / 暗色两套主题、切换程序风格
4. 支持水平和垂直两种模式、窗口置顶
5. 自定义数据导出
6. 完善欢迎帮助界面

<img src="https://cdn.jsdelivr.net/gh/xmuli/xmuliPic@pic/2021/20210316-162021.png" width="100%"/>

<img src="https://cdn.jsdelivr.net/gh/xmuli/xmuliPic@pic/2021/20210316-162022.png" width="100%"/>

<img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2025/202506162316340.png" width="100%"/>

<br>

### NetPlugin

​		一款轻便、快速的任务栏网速插件（仅 DDE）。姑且是 [lfxspeed](https://github.com/xmuli/lfxspeed) 的重构版本。 

<img src="https://cdn.jsdelivr.net/gh/xmuli/xmuliPic@pic/2021/20210316-162023.gif" width="60%"/> <img src="https://cdn.jsdelivr.net/gh/xmuli/xmuliPic@pic/2021/20210316-162024.gif" width="35%"/>

<br>

### 上架 App Store

<img src="https://fastly.jsdelivr.net/gh/XMuli/xmuliPic@pic/2025/202506162315588.png" width="100%"/>

<br>

### 编译

进入源码目录后，执行（在 Deepin V20.9 上编译示例）：

```bash
sudo apt install g++ cmake libqt5*-dev libdtk{core,widget,gui}-dev dde-dock-dev # 依赖

mkdir build && cd build # 编译
cmake ..
make -j16

sudo make install # 安装
```

**注意：** 若未生效，执行 `killall dde-dock` 重启任务栏后生效

<br>

### 卸载

```bash
sudo rm -rf /usr/lib/dde-dock/plugins/libNetPlugins.so ~/.config/NetSpeed /usr/share/NetSpeed
```

<br>

### 下载

可自行执行命令编译安装，也可下载对应平台安装包安装：[地址](https://github.com/xmuli/lfxNet/releases/tag/v0.9.2)  

<br>

### 贡献者

作者：[偕臧](https://github.com/xmuli)            鸣谢: [流年匆忙](https://github.com/justforlxz) 、 [zccrs](https://github.com/zccrs) 

<br>

若是帮助到了你，<font color=#FE7207  size=4 face="幼圆">可以点击该项目的的<img src="https://raw.githubusercontent.com/xmuli/xmuliPic/pic/2021/20210128152407.png" height="18" width="18"/>   **Star** 和<img src="https://raw.githubusercontent.com/xmuli/xmuliPic/pic/2021/20210128152444.png" height="18" width="18"/>  **Fork** 的两个图标，方便抬手之间，相当点个赞，</font>手有余香。





