cmake_minimum_required(VERSION 3.13)
set(PROJECT_NAME NetSpeed)
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 0)
project(${PROJECT_NAME})

#set(CMAKE_CXX_FLAGS "-g -Wall")  # gdb 调试信息
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(QT_VERSION 5)
set(REQUIRED_LIBS Core Gui Widgets DBus)
set(REQUIRED_LIBS_QUALIFIED Qt5::Core Qt5::Gui Qt5::Widgets Qt5::DBus)
find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} REQUIRED) # REQUIRED: 找不到就报错


set(TS_FILES
        ./translations/zh_CN.ts
        ./translations/zh_TW.ts)

find_package(Qt${QT_VERSION} COMPONENTS ${REQUIRED_LIBS} LinguistTools REQUIRED)

qt5_create_translation(QM_FILES ${CMAKE_CURRENT_SOURCE_DIR} ${TS_FILES})

add_subdirectory(lib)
add_subdirectory(dde-dock-plugin)

# 加入一个配置头文件，用于处理 CMake 对源码的设置
#configure_file("${CMAKE_CURRENT_SOURCE_DIR}/config.h.in"
#               "${CMAKE_CURRENT_BINARY_DIR}/config.h")

#message(${PROJECT_SOURCE_DIR}) # 源码路径
#message(${PROJECT_BINARY_DIR}) # 二进制产物输出路径

file(GLOB DIR "widgets/*.h" "widgets/*.cpp" "widgets/*.ui" "*.h" "*.hpp" "*.cpp" "*.ui" "*.qrc" "*.json")

#foreach(v ${DIR})
#    message(${v})
#endforeach()
add_executable(${PROJECT_NAME} ${DIR} ${QM_FILES})
target_link_libraries(${PROJECT_NAME} MonitorInfo)   #NetPlugins

install(FILES "./resource/data/NetSpeed.json" DESTINATION "/usr/share/NetSpeed")
install(FILES "./resource/images/NetSpeed.svg" DESTINATION "/usr/share/NetSpeed")
install(FILES "./resource/NetSpeed.desktop" DESTINATION "/usr/share/applications")
file(GLOB TRANSLATIONS "${CMAKE_CURRENT_BINARY_DIR}/*.qm")  # 要带上构建生成的路径
install(FILES ${TRANSLATIONS}
        DESTINATION "/usr/share/NetSpeed/translations")
install(TARGETS ${PROJECT_NAME} DESTINATION "/usr/bin/NetSpeed")
