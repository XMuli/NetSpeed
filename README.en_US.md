# lfxNet

<p align="right"><br><a href="README.en_US.md">English</a> | <a href="README.md">简体中文</a> | <a href="README.zh_TW.md">繁體中文</a></p>

 ![](https://img.shields.io/badge/language-c++-orange.svg) ![](https://img.shields.io/badge/language-Qt-orange.svg) ![](https://img.shields.io/github/license/xmuli/lfxNet) ![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/xmuli/lfxNet)  

 ![GitHub All Releases](https://img.shields.io/github/downloads/xmuli/lfxNet/total)  ![GitHub stars](https://img.shields.io/github/stars/xmuli/lfxNet?style=social) ![GitHub forks](https://img.shields.io/github/forks/xmuli/lfxNet?style=social) 

<br>

> `lfxNet` is a lightweight, fast application that displays system resource information in real time.

<img src="docs/lfxMonitorNetHor.gif" alt="lfxMonitorNetHor" width="65%"> <img src="docs/lfxMonitorNetVer.gif" alt=" lfxMonitorNetVer" width="30%">

<br>

## directory

- [Background](#Background)
- [Compile](#Compile)
- [Download](#Download)
- [Author](#Author)
- [Acknowledgements](#Acknowledgements)
- [License](#License)

<br>

### Background

​		I love `DDE`, I am a `Deepin` lover and one of the developers. The project was born because I was used to a taskbar speed plugin on other systems, but there was nothing similar on `Deepin/UOS`.

<br>

​		Provide a desktop fixed transparent window on `Linux`, `Windows`, `MacOS`, so that this window coordinates with the system desktop and can display the active status of internet speed, CPU, memory, disk, etc. in real time. Also available separately for `Win10` and `DDE (for lfxMonitorNet)` is the Taskbar Realtime Internet Speed plugin.

<br>

**lfxNet:** Create a `CPU` usage-rate transparent window fixed to the desktop (not open by default)

<img src="docs/RunWindow.gif" alt="RunWindow" width="40%">

<br>

**lfxMonitorNet:** 

> **For detailed demonstration and running results, see 💘 [README](https://github.com/xmuli/lfxNet/tree/master/dde-dock-plugin)** 

​		is a refactored version of [lfxspeed](https://github.com/xmuli/lfxspeed), at first, it was only for self-use to check the real-time network speed, then it was open-sourced in the community and people liked it, so I was very happy; then I decided to refactor and extend it to [lfxNet](https://github.com/xmuli/lfxsNet), the goal is to first write it for general purpose `Linux`, and then extend it to `Win` and `Mac` platforms at leisure, to display network speed, CPU, MEM and other information.

<img src="docs/DockSetting.png" alt="DockSetting" width="80%">

<br>

### Compile

After picking into the source directory, execute.

```bash
sudo apt install g++ cmake libqt5*-dev libdtk{core,widget,gui}-dev dde-dock-dev # dependencies

mkdir build && cd build # compile
cmake ...
make -j16

sudo make install # Install
```

**Note:** If it doesn't work, run `killall dde-dock` to restart the taskbar and it will take effect

<br>

If it helped you, <font color=#FE7207 size=4 face="幼圆"> can click on the project’s  <img src="https://raw.githubusercontent.com/touwoyimuli/FigureBed/master/project_log/20190709023321.png" height="18" width="18"/>  **Star** and <img src="https://raw.githubusercontent.com/touwoyimuli/FigureBed/master/project_log/20190709023317.png" height ="18" width="18"/> **Fork**’s two icons are convenient for raising your hands. It’s quite a thumbs-up.</font> There is a fragrance in your hand.

<br>

### Download

You can compile and install by executing the command yourself, or download the corresponding platform installer to install: [address](https://github.com/xmuli/lfxNet/releases/tag/v0.9.2)  

<br>

### Author

[偕臧](https://github.com/xmuli) 

<br>

### Acknowledgements

[流年匆忙](https://github.com/justforlxz) 、 [zccrs](https://github.com/zccrs) 

<br>

### License

<img src="docs/MIT.png" alt="MIT" width="15%">

This project is distributed and used based on the `MIT` protocol. For more information, see [LICENSE ](/LICENSE).
