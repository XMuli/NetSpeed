#set(DDE_DOCK_PLUGINS_PATH "/usr/lib/dde-dock/plugins")
#if (EXISTS ${DDE_DOCK_PLUGINS_PATH})
    set(PLUGIN_NAME "lfxMonitorNet")
    set(CMAKE_AUTOUIC ON)

    find_package(DtkWidget REQUIRED) # 一次只能找一个库
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(DockInterface REQUIRED dde-dock)  # 效果等价运行 pkg-config --libs dtkwidget

    #include_directories(. DIR_INCLUDE_DDEDOCKdock_NET) # 配合下面使用，在 QtCreator 上头文件是灰色
    #aux_source_directory(. DIR_LIB_DDEDOCK_NET) # 查找仅当前路径下所有源文件，保存到后者变量
    file(GLOB DIR_DDE_DOCK_NET "*.h" "*.cpp" "*.ui" "*.qrc" "../lib/MonitorInfo_x11.cpp")
    ADD_LIBRARY(${PLUGIN_NAME} SHARED ${DIR_DDE_DOCK_NET} "MonitorNet.json") # "MonitorNet.json" 仅项目树中显示，可不加

    target_include_directories(${PLUGIN_NAME} PUBLIC  # 添加头文件，在有目标文件后执行
        ${DtkWidget_INCLUDE_DIRS}         # 变量是由 find_package(DtkWidget .. 后生成； 下同
        ${DockInterface_INCLUDE_DIRS}
        )
    target_link_libraries(${PLUGIN_NAME} PRIVATE  # 添加链接文件
        ${REQUIRED_LIBS_QUALIFIED}
        ${DtkWidget_LIBRARIES}
        ${DockInterface_LIBRARIES}
        )

    #set(CMAKE_INSTALL_PREFIX "/usr")  # 默认为 "/usr/local", 若指定 / 的绝对路径，则 install 命令使用此变量作作为前缀会失效
#    install(FILES "../config.json" DESTINATION "~/.config")
    install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION "/usr/lib/dde-dock/plugins")


    # 参考：https://github.com/linuxdeepin/dde-dock/blob/master/plugins/plugin-guide/plugins-developer-guide.md

    # 说明:
    # REQUIRED 参数表示如果没有找到则报错
    # find_package 命令在找到并加载指定的库之后会创建一些变量，比如：
    # <库名>_FOUND          是否找到（Qt5Widgets_FOUND）
    # <库名>_DIR            在哪个目录下找到的（Qt5Widgets_DIR）
    # <库名>_INCLUDE_DIRS   有哪些头文件目录（Qt5Widgets_INCLUDE_DIRS）
    # <库名>_LIBRARIES      有哪些库文件（Qt5Widgets_LIBRARIES）


    # find_package 命令还可以用来加载 cmake 的功能模块
    # 并不是所有的库都直接支持 cmake 查找的，但大部分都支持了 pkg-config 这个标准，
    # 因此 cmake 提供了间接加载库的模块：FindPkgConfig， 下面这行命令表示加载 FindPkgConfig 模块，
    # 这个 cmake 模块提供了额外的基于 “pkg-config” 加载库的能力
    # 执行下面的命令后后会设置如下变量，不过一般用不到：
    # PKG_CONFIG_FOUND            pkg-config 可执行文件是否找到了
    # PKG_CONFIG_EXECUTABLE       pkg-config 可执行文件的路径
    # PKG_CONFIG_VERSION_STRING   pkg-config 的版本信息


    # 加载 FindPkgConfig 模块后就可以使用 pkg_check_modules 命令加载需要的库
    # pkg_check_modules 命令是由 FindPkgConfig 模块提供的，因此要使用这个命令必须先加载 FindPkgConfig 模块。
    # 执行 pkg_check_modules 命令加载库也会设置一些类似执行 find_package 加载库后设置的变量：
    # DockInterface_FOUND
    # DockInterface_INCLUDE_DIRS
    # DockInterface_LIBRARIES
    # 还有有另外的一些变量以及更灵活的用法，比如一次性查找多个库
#endif()


