# lfxMonitorNet

<p align="right"><br><a href="README.md">English</a> | <a href="README.zh_CN.md">简体中文</a> | <a href="README.zh_TW.md">繁體中文</a></p>

 `lfxMonitorNet` 是 `lfxNet` 的子项目，在 `DDE` 任务栏中，实时显示 “网速、CPU、内存” 等信息，附加自定义颜色、主题、相关流量预警提示等功能。

<img src="../docs/lfxMonitorNetHor.gif" alt="lfxMonitorNetHor" width="65%">  <img src="../docs/lfxMonitorNetVer.gif" alt="lfxMonitorNetVer" width="30%">

<img src="../docs/lfxMonitorNet.png" alt="lfxMonitorNet" width="100%">

<br>

## 目录

- [背景](#背景)
- [编译](#编译)
- [卸载](#卸载)
- [维护者](#维护者)
- [许可协议](#许可协议)

<br>

### 背景

​		喜爱 `DDE` ，是 `Deepin` 爱好者、也是开发者之一。因习惯桌面上有一个任务栏网速，但在 `Deepin/UOS`上没有相似的，故开发 `dde-dock` 的一款网速插件 `lfxMonitorNet`；用来显示实时网速插件。 `lfxMonitorNet`  是  [lfxspeed](https://github.com/xmuli/lfxspeed) 的彻底重构版本，故版本号延续。


<br>

### 编译

采用 `cmake` 构建，进入源码目录后，执行如下：

```bash
git clone https://github.com/xmuli/lfxNet.git
cd lfxNet
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

### 运行截图

附 `lfxMonitorNet` 在 `DDE` 上实际运行的效果图：

<img src="../docs/DockSetting.png" alt="DockSetting" width="70%">

<img src="../docs/Config.png" alt="Config" width="70%">

<img src="../docs/About.png" alt="About" width="70%">

<br>

### 开发者

[@偕臧](https://github.com/xmuli)

<br>

### 许可协议

![](https://img.shields.io/github/license/xmuli/lfxSpeed) [MIT](LICENSE) © xmuli <偕臧>

