#get svn and build infomation that will be written to Src/Version.h
#call after command project for ${PROJECT_SOURCE_DIR}
# 当您调用`include(cmake/gitinfo.cmake)`命令时，CMake将读取并执行`gitinfo.cmake`文件中的语句。这就是为什么在该文件中使用了CMake语法。

# 下面是一些在`gitinfo.cmake`文件中使用的CMake命令及其含义：

# - `macro(get_git_info)`: 定义了一个名为`get_git_info`的宏。
# - `find_package(Git)`: 查找Git包，并设置相应的变量（例如，`GIT_FOUND`、`GIT_EXECUTABLE`等）以指示是否找到了Git。
# - `execute_process(COMMAND ...)`: 执行指定的命令。在这里，我们使用了`execute_process`命令来执行一系列Git命令，并将它们的输出存储在CMake变量中。
# - `set(<variable_name> <value>)`: 设置变量的值。
# - `message(...)`: 向终端输出消息。
# - `file(READ <file_path> <variable_name>)`: 读取指定文件的内容并将其存储在变量中。
# - `file(WRITE <file_path> <content>)`: 将指定的内容写入指定的文件。
# - `file(APPEND <file_path> <content>)`: 将指定的内容追加到指定的文件中。
# - `endmacro()`: 结束当前宏定义。

# 总之，通过使用CMake命令，`gitinfo.cmake`文件可以执行多个Git命令并将输出存储在CMake变量中。然后，这些变量用于生成一个包含有关Git仓库和构建环境的信息头文件。
macro(get_git_info)
    find_package(Git)
    # COMMAND ${GIT_EXECUTABLE}：指定要执行的命令。${GIT_EXECUTABLE}是一个CMake变量，其值为Git可执行文件的路径。
    # rev-parse --abbrev-ref HEAD：这是要传递给Git的参数。rev-parse命令用于解析并输出与指定引用相关联的SHA1哈希或其他信息。在这里，我们使用--abbrev-ref HEAD选项来获取当前所在分支的简短名称。
    # WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}：设置命令的工作目录。${CMAKE_SOURCE_DIR}是一个CMake变量，其值为包含CMakeLists.txt文件的顶级源代码目录的路径。
    # OUTPUT_VARIABLE TOP_GIT_BRANCH：指定输出结果应存储在哪个CMake变量中。在这里，我们将输出结果存储在名为TOP_GIT_BRANCH的变量中。
    # OUTPUT_STRIP_TRAILING_WHITESPACE：指定是否应去除输出结果末尾的空白字符（例如换行符、空格等）。
    # 总之，该命令执行了一个Git命令，将当前所在分支的名称存储在名为TOP_GIT_BRANCH的CMake变量中。通过调用get_git_info()宏，可以获取和使用此变量的值。
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse --abbrev-ref HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE TOP_GIT_BRANCH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    # execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --format=%H
    #     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    #     OUTPUT_VARIABLE TOP_GIT_HASH
    #     OUTPUT_STRIP_TRAILING_WHITESPACE
    # )

    # execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --format=%cd
    #     WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
    #     OUTPUT_VARIABLE TOP_GIT_DATE
    #     OUTPUT_STRIP_TRAILING_WHITESPACE
    # )

    #execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --pretty=oneline --abbrev-commit
    execute_process(COMMAND ${GIT_EXECUTABLE} log -1 --pretty=format:"%h"
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE TOP_GIT_LOG_LATEST
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    execute_process(
        COMMAND sh -c "id | awk '{printf $1}'"
        OUTPUT_VARIABLE TOP_BUILD_USER
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    execute_process(
        COMMAND sh -c "hostname | awk '{printf $1}'"
        OUTPUT_VARIABLE TOP_BUILD_HOST
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    execute_process(
        COMMAND sh -c "pwd | awk '{printf $1}'"
        OUTPUT_VARIABLE TOP_BUILD_PATH
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )

    execute_process(
        COMMAND sh -c "${GIT_EXECUTABLE} submodule foreach git log -1 --format='%h' | sed 'N;s/\\n/:/g' | grep -aoE 'xtopcom.*' | sed \"s/\'//g\" | sed 's/xtopcom\\\///g' | tr '\n' '|' "
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE TOP_GIT_SUBMODULE
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    
    # string (REGEX REPLACE "[\n\t\r]" "" TOP_GIT_BRANCH "${TOP_GIT_BRANCH}")
    # string (REGEX REPLACE "[\n\t\r]" "" TOP_GIT_HASH "${TOP_GIT_HASH}")
    # string (REGEX REPLACE "[\n\t\r]" "" TOP_GIT_DATE "${TOP_GIT_DATE}")

    message("git branch: ${TOP_GIT_BRANCH}")
    # message("git hash: ${TOP_GIT_HASH}")
    # message("git date: ${TOP_GIT_DATE}")
    message("git log -1: ")
    message("${TOP_GIT_LOG_LATEST}")
    message("git submodule info:")
    message("${TOP_GIT_SUBMODULE}")

    set(BUILD_OPTIONS_LIST "unknown")
    if (EXISTS ${PROJECT_BINARY_DIR}/build_options.inc)
        file(READ ${PROJECT_BINARY_DIR}/build_options.inc BUILD_OPTIONS_LIST)
        message("build options: ${BUILD_OPTIONS_LIST}")
    else()
        message("not find build_options file (should be generate by build.sh)")
    endif()

    set(VersionInc "${PROJECT_SOURCE_DIR}/src/libraries/xchaininit/src/version.inc")
    file(WRITE  ${VersionInc} "const std::string TOP_VERSION_PREFIX  = \"TOP_VERSION\";\n")  # will overwrite
    file(APPEND ${VersionInc} "const std::string TOP_GIT_BRANCH      = \"${TOP_GIT_BRANCH}\";\n")
    file(APPEND ${VersionInc} "const std::string TOP_GIT_HASH        = \"${TOP_GIT_HASH}\";\n")
    file(APPEND ${VersionInc} "const std::string TOP_GIT_DATE        = \"${TOP_GIT_DATE}\";\n")
    file(APPEND ${VersionInc} "const std::string TOP_GIT_LOG_LATEST  = ${TOP_GIT_LOG_LATEST};\n")  # multiline
    file(APPEND ${VersionInc} "const std::string TOP_GIT_SUBMODULE   = \"${TOP_GIT_SUBMODULE}\";\n")
    file(APPEND ${VersionInc} "const std::string TOP_BUILD_DATE      = __DATE__;\n")
    file(APPEND ${VersionInc} "const std::string TOP_BUILD_TIME      = __TIME__;\n")
    file(APPEND ${VersionInc} "const std::string TOP_BUILD_USER      = \"${TOP_BUILD_USER}\";\n")
    file(APPEND ${VersionInc} "const std::string TOP_BUILD_HOST      = \"${TOP_BUILD_HOST}\";\n")
    file(APPEND ${VersionInc} "const std::string TOP_BUILD_PATH      = \"${TOP_BUILD_PATH}\";\n")
    file(APPEND ${VersionInc} "const std::string TOP_BUILD_OPTIONS   = \"${BUILD_OPTIONS_LIST}\";\n")

endmacro()
