# lfxNet

<p align="right"><br><a href="README.en_US.md">English</a> | <a href="README.md">简体中文</a> | <a href="README.zh_TW.md">繁體中文</a></p>

 ![](https://img.shields.io/badge/language-c++-orange.svg) ![](https://img.shields.io/badge/language-Qt-orange.svg) ![](https://img.shields.io/github/license/xmuli/lfxNet) ![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/xmuli/lfxNet)  

 ![GitHub All Releases](https://img.shields.io/github/downloads/xmuli/lfxNet/total)  ![GitHub stars](https://img.shields.io/github/stars/xmuli/lfxNet?style=social) ![GitHub forks](https://img.shields.io/github/forks/xmuli/lfxNet?style=social) 

<br>

> `lfxNet` 是一款輕量、快速的實時顯示系統資源信息的應用程式。

 <img src="docs/lfxMonitorNetHor.gif" alt="lfxMonitorNetHor" width="65%">  <img src="docs/lfxMonitorNetVer.gif" alt="lfxMonitorNetVer" width="30%">

<br>

## 目錄

- [背景](#背景)
- [編譯](#編譯)
- [下載](#下載)
- [作者](#作者)
- [鳴謝](#鳴謝)
- [協議](#協議)

<br>

### 背景

​		 喜愛 `DDE` ，為 `Deepin` 愛好者、也是開發者之一。因習慣其它系統上有一個任務欄網速外掛程式，但在 `Deepin/UOS`上沒有相似的，故項目誕生。

<br>

​		 在 `Linux`、`Windows`、`MacOS` 上提供一個桌面固定透明視窗，讓此視窗協調于系統桌面，可實時顯示網速、CPU 、記憶體、磁碟等活躍狀態。同時單獨提供 `Win10` 和 `DDE (為 lfxMonitorNet)` 的任務欄實時網速外掛程式。

<br>

**lfxNet：** 努力完善中，，，

<br>

**lfxMonitorNet：**

> **詳細展示和運行效果，參見 💘  [README](https://github.com/xmuli/lfxNet/tree/master/dde-dock-plugin)**

​		是 [lfxspeed](https://github.com/xmuli/lfxspeed) 的重構版本，起初，僅自用查看實時網速，後開源在社區被大家喜歡，甚是開心；後決定重構加擴展其為 [lfxsNet](https://github.com/xmuli/lfxsNet)，目標先寫成通用的 `Linux` 、後空閑擴展到 `Win` 和 `Mac` 平臺，可顯示網速、CPU、MEM 等信息。

<br>

### 編譯

採進入源碼目錄後，執行：

```bash
sudo apt install g++ cmake libqt5*-dev libdtk{core,widget,gui}-dev dde-dock-dev # 依賴

mkdir build && cd build # 編譯
cmake ..
make -j16

sudo make install # 安裝
```

**注意：** 若未生效，執行 `killall dde-dock` 重啟任務欄後生效

<br>

若是幫助到了你，<font color=#FE7207  size=4 face="幼圆"> 可以點擊該項目的的 <img src="https://raw.githubusercontent.com/xmuli/xmuliPic/pic/2021/20210128152407.png" height="18" width="18"/>  **Star** 和 <img src="https://raw.githubusercontent.com/xmuli/xmuliPic/pic/2021/20210128152444.png" height="18" width="18"/> **Fork** 的兩個圖標，方便抬手之間，相當點個贊，</font> 手有餘香。

<br>

### 下載

可自行執行命令編譯安裝，也可下載對應平臺安裝包安裝：[地址](https://github.com/xmuli/lfxNet/releases/tag/v0.9.2)  

<br>

### 作者

[偕臧](https://github.com/xmuli)

### 鳴謝

[流年匆忙](https://github.com/justforlxz) 、 [zccrs](https://github.com/zccrs)

<br>

### 協議

<img src="docs/MIT.png" alt="MIT" width="15%">

該項目基于 `MIT` 協議進行分發和使用。 有關更多信息，請參閱 [協議文件](/LICENSE)。
