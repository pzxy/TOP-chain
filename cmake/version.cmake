# get version, set version compile option
# 这段 CMake 代码的语义是：

# 检查 ${PROJECT_SOURCE_DIR}/src/xtopcom/xversion/version 文件是否存在。
# 如果文件存在，则将其内容读取到 PROGRAM_VERSION 变量中。
# 从 PROGRAM_VERSION 字符串中去除前导和尾随空格。
# 在控制台输出编译程序版本信息。
# 使用 configure_file() 函数替换 version.h.in 模板文件中的变量，生成一个名为 generated/version.h 的头文件。只替换模板文件中使用 "@" 包围的变量。
# 如果文件不存在，则打印致命错误消息并停止编译过程。
if(EXISTS "${PROJECT_SOURCE_DIR}/src/xtopcom/xversion/version")
    file(READ "${PROJECT_SOURCE_DIR}/src/xtopcom/xversion/version" PROGRAM_VERSION)
    string(STRIP "${PROGRAM_VERSION}" PROGRAM_VERSION)
    message("compile program version: ${PROGRAM_VERSION}")

    configure_file(
      ${PROJECT_SOURCE_DIR}/src/xtopcom/xversion/version.h.in
      generated/version.h
      @ONLY
      )
else()
    message(FATAL_ERROR "File ${PROJECT_SOURCE_DIR}/src/xtopcom/xversion/version not found")
endif()


