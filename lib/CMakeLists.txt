set(LIB_SHARED_NAME "MonitorInfo")

file(GLOB DIR_LIB "*.h" "*.cpp")
add_library(${LIB_SHARED_NAME} SHARED ${DIR_LIB})  # 生成动态库 MonitorInfo，
target_link_libraries(${LIB_SHARED_NAME} PUBLIC ${REQUIRED_LIBS_QUALIFIED}) # 链接外部 Qt 库，且共享
