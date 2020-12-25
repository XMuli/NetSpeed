set(PLUGIN_NAME "MonitorNet")

#include_directories(. DIR_INCLUDE_DDEDOCK_NET) # 配合下面使用，在 QtCreator 上头文件是灰色
#aux_source_directory(. DIR_LIB_DDEDOCK_NET) # 查找仅当前路径下所有源文件，保存到后者变量
file(GLOB DIR_DDE_DOCK_NET "*.h" "*.cpp")
ADD_LIBRARY(${PLUGIN_NAME} SHARED ${DIR_DDE_DOCK_NET})

#set(CMAKE_INSTALL_PREFIX "/usr")  # 默认为 "/usr/local", 若指定 / 的绝对路径，则 install 命令使用此变量作作为前缀会失效
install(TARGETS ${PLUGIN_NAME} LIBRARY DESTINATION "/usr/lib/dde-dock/plugins")
