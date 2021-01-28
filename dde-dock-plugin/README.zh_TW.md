# lfxMonitorNet

<p align="right"><br><a href="README.en_US.md">English</a> | <a href="README.md">简体中文</a> | <a href="README.zh_TW.md">繁體中文</a></p>

 ![](https://img.shields.io/badge/language-c++-orange.svg) ![](https://img.shields.io/badge/language-Qt-orange.svg) ![](https://img.shields.io/github/license/xmuli/lfxNet) ![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/xmuli/lfxNet)  

 ![GitHub All Releases](https://img.shields.io/github/downloads/xmuli/lfxNet/total)  ![GitHub stars](https://img.shields.io/github/stars/xmuli/lfxNet?style=social) ![GitHub forks](https://img.shields.io/github/forks/xmuli/lfxNet?style=social) 

<br>

> `lfxMonitorNet`  是 `dde-dock` 的網速外掛程式，也是 [lfxspeed](https://github.com/xmuli/lfxspeed) 的重構版本。

​		起初，僅自用查看實時網速，後開源在社區被大家喜歡，甚是開心；後決定重構加擴展其為 [lfxsNet](https://github.com/xmuli/lfxsNet)，目標先寫成通用的 `Linux` 、後空閑擴展到 `Win` 和 `Mac` 平臺，可顯示網速、CPU、MEM 等信息。

<img src="../docs/lfxMonitorNetHor.gif" alt="lfxMonitorNetHor" width="65%"> <img src="../docs/lfxMonitorNetVer.gif" alt="lfxMonitorNetVer" width="30%">

<br>

## 目錄

- [背景](#背景)
- [功能](#功能)
- [編譯](#編譯)
- [截圖](#截圖)
- [作者](#作者)
- [鳴謝](#鳴謝)
- [協議](#協議)

<br>

### 背景

​		重構之前的[舊項目](https://github.com/xmuli/lfxspeed)，修複諸多缺陷，和新添加許多新功能。當處于正式 `v1.0` 版本，才算一個達到我的預期版本，心裡才覺得算是寫好了。

<br>

### 功能

- 支持功能：
 - 高分屏顯示
 - 用戶自定義配置
 - 為系統所有用戶安裝
 - 顯示的首碼內容和顏色
 - 顯示的標籤內容和顏色
 - 更改顯示字體和字體大小
 - 適配 `dock` 的模式（水平和垂直）
 - 自動切換單位（預設智能）
 - 調整網路速率精確度
 - 調整刷新時間間隔
 - 顯示額外顯示的懸浮信息
 - 左鍵單擊顯示的動作（可單擊打開 “系統監視器”）
 - 配置數據導出（可系統路徑 | 用戶路徑）
 - `CPU`、記憶體、的利用率；流量的使用預警
 - 應用的主題樣式（淺色和暗色模式）
 - 更改系統所自帶的所有風格選擇

<br>


### 編譯

採用 `cmake` 構建，進入源碼目錄後，執行如下：

```bash
sudo apt install g++ cmake libqt5*-dev libdtk{core,widget,gui}-dev dde-dock-dev # 依賴

mkdir build && cd build # 編譯
cmake ..
make -j16

sudo make install # 安裝
```

或直接使用 `IDE` 點擊運行按鈕，找到生成目錄 `/dde-dock-plugin` 下，手動複製共用庫： `sudo cp liblfxMonitorNet.so /usr/lib/dde-dock/plugins` ；

<br>

**注意：** 若未生效，執行 `killall dde-dock` 重啟任務欄後生效

<br>

### 截圖

設置個性化，更多截圖效果參見：[連結](https://github.com/xmuli/lfxNet/tree/master/docs)

<img src="../docs/lfxMonitorNet.png" alt="lfxMonitorNet" width="100%">

 <img src="../docs/DockSetting.png" alt="DockSetting" width="75%">

 <img src="../docs/Config.png" alt="Config" width="75%">

 <img src="../docs/About.png" alt="About" width="75%">

<br>

### 作者

[偕臧](https://github.com/xmuli)

<br>

### 鳴謝

[流年匆忙](https://github.com/justforlxz) 、 [zccrs](https://github.com/zccrs) 、

<br>

### 協議

<img src="../docs/MIT.png" alt="MIT" width="15%">

該項目基于 `MIT` 協議進行分發和使用。 有關更多信息，請參閱 [協議文件](../LICENSE)。


