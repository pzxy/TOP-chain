cmake_minimum_required(VERSION 3.22)
# RESULT 指定变量名为XOS_ID，QUERY参数指定了要查询的系统信息的名称，DISTRIB_ID表示查询的是系统发行版id，
cmake_host_system_information(RESULT XOS_ID QUERY DISTRIB_ID)
if (${XOS_ID} MATCHES "centos")
    set(XSYS_VER 0)
elseif(${XOS_ID} MATCHES "ubuntu")
    set(XSYS_VER 1)
else()
    message(FATAL_ERROR "Uknown OS distribution ID")
endif()

if (${XSYS_VER} EQUAL 0)
    set(CMAKE_CXX_STANDARD 11)
else()
# 设置c++标准为c++23
    set(CMAKE_CXX_STANDARD 23)
endif()
# ON 表示必须支持选定的c++标准
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 选项表示是否启用编译器的扩展功能，扩展功能会降低可移植性。比如：inline namespace、constexpr、noexcept等C++11标准引入的新特性等；
set(CMAKE_CXX_EXTENSIONS OFF)
# 设置c标准选项
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
# Makefile文件是否输出详细的编译信息，还是只输出必要信息。默认为ON，即输出详细信息。
set(CMAKE_VERBOSE_MAKEFILE OFF)
# 生成编译命令的JSON文件，可以用于一些代码编辑器的自动补全和语法检查功能。默认根目录下的：compile_commands.json 文件。
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# 设置编译器类型，设置成Debug或者Release，如果没有设置编译类型，则设置成Debug
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

# 设置项目名，并指定编译器
project(XTOPCHAIN CXX C)
# 寻找静态链接库
SET(CMAKE_FIND_LIBRARY_SUFFIXES ".a")

# 没有c++编译器就输出FATAL_ERROR致命错误
if (NOT CMAKE_CXX_COMPILER)
    message(FATAL_ERROR "C++ Compiler not found")
endif()
# 将内置变量CMAKE_BUILD_TYPE 值转换成小写， 放在XBUILD_TYPE中。
string(TOLOWER ${CMAKE_BUILD_TYPE} XBUILD_TYPE)
# 如果 变量XBUILD_TYPE包含rel，就设置XBUILD_TYPE成release
if ("${XBUILD_TYPE}" MATCHES "rel")
    set(XBUILD_TYPE release)
endif()
# CMAKE_SYSTEM_NAME是CMake内置的变量，用于指定操作系统的名称，并转换成小写，就是：android，ios，linux，mac，win这些
string(TOLOWER ${CMAKE_SYSTEM_NAME} XSYSTEM_NAME_LOWERCASE)
# 按照上面的代码，1的话是ubuntu
if (${XSYS_VER} EQUAL 1)
# add_definitions 向C++编译器添加预定义宏
# MSGPACK_NO_BOOST 表示禁用boost库支持，“-D”选项以通知编译器定义该宏。
# 之所以禁用boost库支持，由于Ubuntu默认情况下安装的Boost库版本和路径与CMake的默认设置不一致，可能找不到这个库。
    add_definitions(-DMSGPACK_NO_BOOST)
# OPENSSL_API_COMPAT=10101表示启用与OpenSSL 1.1.1兼容的API
    add_definitions(-DOPENSSL_API_COMPAT=10101)
endif()
# 这些选项变量都是定义的，定义的这些变量可以通过 ${变量名} 的方式来使用。这里它的默认值就是OFF
# 如果要启用某个选项，可以在cmake命令后面加上 -D变量名=ON，比如-DXENABLE_CODE_COVERAGE=ON,或者这里直接将OFF改为ON
option(XENABLE_CODE_COVERAGE "Enable code coverage" OFF)# 是否启用代码覆盖率检测
option(BUILD_METRICS "build metrics" OFF)# 性能测试是否打开
option(XCHAIN_FORKED_BY_DEFAULT "Enable chain forked logic" OFF)# 分叉
option(ENABLE_METRICS_DATAOBJECT "Enable metrics dataobject" OFF)# 度量数据对象
option(DISABLE_CORE_SIGNAL_CAPTURE "disable core signal capture" OFF)# core signal capture
option(DISABLE_SIGNAL_CAPTURE "disable all signal capture" OFF)# ON则禁用所有信号捕获
option(XUSE_JEMALLOC "using jemalloc" OFF)# ON使用jemalloc内存分配器，jemalloc是一个开源的内存分配器，它的目标是替换glibc的malloc。

option(XBUILD_TEST "Enable building tests" OFF) # UNIT TEST disabled as default, use build option test to enable it.
# FALSE、OFF、NO、NOTFOUND、0 或空字符串被视为false，其他任何值被视为true
if (XBUILD_TEST)
# FATAL_ERROR：输出错误信息并停止构建。
# SEND_ERROR：输出错误信息，但不停止构建。
# WARNING：输出警告信息。
# AUTHOR_WARNING：输出作者警告信息。
# DEPRECATION：输出弃用警告信息。
# DEBUG：输出调试信息。
# TRACE：输出跟踪信息。
    message(STATUS "ENABLE  building tests ${XBUILD_TEST}")
endif()

option(DISABLE_EVM "Disable EVM" OFF) # EVM as default VM for now, use build option disable_evm to disable evm.
if (DISABLE_EVM)
    message(STATUS "DISABLE WITH EVM ${DISABLE_EVM}")
endif()

option(DISABLE_RATELIMIT "Disable rate limit" OFF) # RPC has rate limit as default, use build option noratelimit to disable it.
if (DISABLE_RATELIMIT)
    message(STATUS "DISABLE rate limit ${DISABLE_RATELIMIT}")
endif()

option(DISABLE_REAL_STAKE "Disable real stake require" OFF) # Real Stake requied as default, use build option mock_zec_stake to disable it.
if(DISABLE_REAL_STAKE)
    message(STATUS "DISABLE real stake ${DISABLE_REAL_STAKE}")
    # 这个定义将在编译时传递给编译器，并且可以在代码中使用#ifdef指令来检查该选项是否已启用。
    add_definitions(-DXENABLE_MOCK_ZEC_STAKE)
endif()

option(STATIC_CONSENSUS "Enable static consensus" OFF)
if (STATIC_CONSENSUS)
    if (NOT DISABLE_REAL_STAKE)
        message(FATAL_ERROR "should define mock_stake_zec before static_consensus")
    endif()
    message(STATUS "Enable static consensus ${STATIC_CONSENSUS}")
endif()

option(ELECT_WHEREAFTER "Enable static consensus, than continue election " OFF)
if (ELECT_WHEREAFTER)
    if (NOT STATIC_CONSENSUS)
        message(FATAL_ERROR "should define static_consensus before elect_whereafter")
    endif()
    message(STATUS "Enable static consensus, than continue election ${ELECT_WHEREAFTER}")
endif()

option(CONSENSUS_SWAP "Enable static consensus, than swap consensus group nodes" OFF)
if (CONSENSUS_SWAP)
    if (NOT ELECT_WHEREAFTER)
        message(FATAL_ERROR "should define elect_whereafter before consensus_swap")
    endif()
    message(STATUS "Enable static consensus, than swap consensus group nodes${CONSENSUS_SWAP}")
endif()

option(MAINNET_ACTIVATED "Mainnet activated" OFF) # Mainnet Activated if off as default, use build option mainnet_activated to enable it.
if (MAINNET_ACTIVATED)
    message(STATUS "Mainnet Activated ${MAINNET_ACTIVATED}")
endif()

#=============================================================================
# for git version
#==============================================================================
# include(filename [OPTIONAL] [RESULT_VARIABLE VAR])
# 生成git的一些信息,
include(cmake/gitinfo.cmake)
get_git_info()
# 替换version版本.
include(cmake/version.cmake)

#==============================================================================
# In order to set default executable and library binaries' location,
# one solution, by setting
# variables (CMAKE_RUNTIME_OUTPUT_DIRECTORY / CMAKE_LIBRARY_OUTPUT_DIRECTORY)
# or properties (RUNTIME_OUTPUT_DIRECTORY / LIBRARY_OUTPUT_DIRECTORY),
# doesn't work, althrough CMake documentation says it's the recommonded way.
# Fall back to use EXECUTABLE_OUTPUT_PATH / LIBRARY_OUTPUT_PATH.
#==============================================================================
#set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)
#set_property(DIRECTORY PROPERTY RUNTIME_OUTPUT_DIRECTORY ${XTOPCHAIN_BINARY_DIR}/bin/${CMAKE_BUILD_TYPE})
#set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR/lib})
#set_property(DIRECTORY PROPERTY LIBRARY_OUTPUT_DIRECTORY ${XTOPCHAIN_BINARY_DIR}/lib/${CMAKE_BUILD_TYPE})
# XTOPCHAIN_BINARY_DIR : /home/kwin/workspace/top/TOP-chain/cbuild
set(EXECUTABLE_OUTPUT_PATH ${XTOPCHAIN_BINARY_DIR}/bin/${CMAKE_SYSTEM_NAME}/)
set(LIBRARY_OUTPUT_PATH ${XTOPCHAIN_BINARY_DIR}/lib/${CMAKE_SYSTEM_NAME}/)
# LIBRARY_OUTPUT_DIRECTORY是CMake内置的变量之一。它用于指定构建生成库文件（静态库或共享库）的输出目录
# 如果您没有显式地设置这个变量，CMake会默认将静态库和共享库都放置在${CMAKE_BINARY_DIR}/lib/目录下
# 其中 CMAKE_BINARY_DIR是CMake内置的变量之一。它表示运行cmake命令时所在的构建目录.
if ("${LIBRARY_OUTPUT_DIRECTORY}" STREQUAL "")
    set(LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})
endif()
# RUNTIME_OUTPUT_DIRECTORY 变量用于指定生成的可执行文件和共享库等运行时输出目录。而 CMAKE_SOURCE_DIR 变量表示项目根目录的路径。
# RUNTIME_OUTPUT_DIRECTORY 默认为 CMAKE_SOURCE_DIR/bin
if ("${RUNTIME_OUTPUT_DIRECTORY}" STREQUAL "")
    set(RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
endif()
# CMAKE_CXX_COMPILER_ID 返回编译器标识符
# GNU：表示 GNU 编译器
# Clang：表示 Clang 编译器
# Intel：表示 Intel C++ 编译器
# MSVC：表示 Microsoft Visual C++ 编译器
if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    # common compiling options,向编译器添加编译参数选项. 
    # -static：指定使用静态链接方式链接库文件。
    # -static-libgcc 和 -static-libstdc++：指定在静态链接时使用静态版本的 GCC C++ 运行库和标准C库。
    # -fno-strict-aliasing：指定禁用严格别名规则，以便编译器可以进行某些优化。
    # -pthread：指定使用 POSIX 线程库。
    # -fstack-protector-strong：指定启用堆栈保护机制，以便能够检测一些常见的堆栈溢出错误。
    # -fno-short-enums：指定禁用枚举类型的隐式转换，以便在编译时提供更多的类型安全性。
    # -Wno-deprecated-declarations：指定忽略已经过时的函数或语法声明的警告。
    # -Wno-unused-function：指定忽略未使用的函数的警告。
        add_compile_options(
        -static
        -static-libgcc
        -static-libstdc++
        -fno-strict-aliasing
        -pthread
        -fstack-protector-strong
        -fno-short-enums
        -Wno-deprecated-declarations
        -Wno-unused-function
    )
    # XOS_ID 变量是上面设置上的
    if (${XOS_ID} MATCHES "ubuntu")
    # -static-pie 选项则指定生成静态的位置独立代码。在 Linux 系统上，位置独立代码可以提高程序的安全性和可移植性。
    # 别的系统中,可能不支持共享库.所以要用-fPIE
        add_compile_options(-static-pie)
    else()
        add_compile_options(-fpie -fPIE)
    endif()

    # CXX only
    # '-frtti' 表示启用运行时类型信息（RTTI），允许程序在运行时使用 type_info 类型的对象，以及 dynamic_cast 和 typeid 运算符。
    # '-fexceptions' 表示启用异常处理机制，允许程序抛出和捕获异常。
    # '-fthreadsafe-statics' 表示对静态变量进行线程安全处理，在多线程环境下保证静态变量的正确性和一致性。
    # $<...>是 CMake的生成器表达式语法，用来在构建时根据目标属性的值动态设置编译选项
    # 当 LINKER_LANGUAGE 等于CXX时,才会添加-frtti
    add_compile_options($<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-frtti>)
    add_compile_options($<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-fexceptions>)
    add_compile_options($<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-fthreadsafe-statics>)
    # _D_GNU_SOURCE 宏定义，告诉编译器启用GNU特定的扩展和功能。限Linux系统,使程序能够访问一些非标准的函数和库。如getresuid(), setresuid()等
    # 在源代码中加入#define _GNU_SOURCE或者使用编译选项-D_GNU_SOURCE来启用。由于这些扩展和功能是非标准的，因此可能会导致代码的可移植性问题。
    add_definitions(-D_GNU_SOURCE)

    #==============================================================================
    # Althrough CMake documentation (3.6.3) says add_compile_options supports
    # generator expressions, the actual result is -buggy-.
    # add_compile_options doesn't fully working with generator expressions.
    #
    # $<$<OR:?1,?2>:...>
    # for OR expression, do not insert any whitespace character between ?1 and ?2.
    #==============================================================================
    # options for Debug
    #add_compile_options($<$<OR:$<CONFIG:Debug>,$<CONFIG:debug>>:-g2 -ggdb -O0 -fno-omit-frame-pointer>)

    # set_property函数用于在目标或目录上设置属性。在这种情况下，DIRECTORY APPEND PROPERTY指定我们正在设置当前目录的属性。
    # 此属性将应用于该目录及其所有子目录中构建的所有目标。
    # "DIRECTORY APPEND PROPERTY"选项表示我们正在设置的是目录的属性，而不是目标的属性。具体来说，"APPEND"选项表示我们将添加一个值到已经存在的属性之后，而不是覆盖它。
    # COMPILE_OPTIONS 属性被附加这些内容: 设置了Debug和debug配置的编译选项。这些选项包括-g3（生成调试信息），-ggdb（使用gdb调试器），-O0（禁用优化）和-fno-omit-frame-pointer（保留用于调试的帧指针）。
    set_property(DIRECTORY APPEND PROPERTY COMPILE_OPTIONS $<$<OR:$<CONFIG:Debug>,$<CONFIG:debug>>:-g3 -ggdb -O0 -fno-omit-frame-pointer>)
    set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<OR:$<CONFIG:Debug>,$<CONFIG:debug>>:DEBUG _DEBUG>)

    # options for Release & RelWithDebInfo
    # add_compile_options($<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:release>>:-fomit-frame-pointer -ffunction-sections -fdata-sections>)
    # 配置release 
    # -ffunction-sections: 此标志告诉编译器将每个函数放置到输出文件的自己的部分中。这可以潜在地使链接器丢弃未使用的函数，从而产生较小的二进制文件。
    # -fdata-sections: 类似于-ffunction-sections，此标志告诉编译器将每个全局变量放置到其自己的部分中。
    # -flto=auto: 启用 GCC 和 Clang 编译器的链接时优化 (LTO)。LTO 允许编译器执行考虑整个程序而不是单个源文件的优化。=auto 选项指定如果编译器和链接器支持，则启用 LTO。
    # -fuse-linker-plugin: 与 LTO 相关，此标志启用 LTO 的链接器插件。
    set_property(DIRECTORY APPEND PROPERTY COMPILE_OPTIONS $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:release>>:-ffunction-sections -fdata-sections -flto=auto -fuse-linker-plugin>)
    set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:release>>:NDEBUG>)

    # options for Release
    # 如果构建配置为Release或者release，则将"-g0"选项添加到编译选项中。"-g0"选项告诉编译器在生成可执行文件时不要包含任何调试信息。这可以减小可执行文件的大小，并提高其运行效率。
    add_compile_options($<$<OR:$<CONFIG:Release>,$<CONFIG:release>>:-g0>)

    # options for RelWithDebInfo
    # "-g1"选项告诉编译器在生成可执行文件时包含基本的调试信息，-ggdb"选项告诉编译器在生成可执行文件时包含完整的调试信息
    add_compile_options($<$<CONFIG:RelWithDebInfo>:-g1>)
    add_compile_options($<$<CONFIG:RelWithDebInfo>:-ggdb>)
else()
    message(FATAL_ERROR "Not supported C++ Compiler: " ${CMAKE_CXX_COMPILER_ID})
endif()

if ((BUILD_GPERF OR BUILD_GHPERF OR TCMALLOC) AND XUSE_JEMALLOC)
    message(FATAL_ERROR "Not allowed to enable tcmalloc and jemalloc at the same time")
endif()

if (ENABLE_JEPROF)
    if (NOT XUSE_JEMALLOC)
        message(FATAL_ERROR "should define use_jemalloc before enable_jeprof")
    endif()
    add_definitions(-DENABLE_JEPROF)
endif()

if (ADDRESS_SANITIZER)
    add_compile_options(-fsanitize=address)
endif()

if (BUILD_METRICS)
    add_definitions(-DENABLE_METRICS)
endif()

if (BUILD_XSECURITY)
    add_definitions(-DENABLE_XSECURITY)
endif()

if (BUILD_GPERF)
    add_definitions(-DENABLE_GPERF)
endif()

if (BUILD_GHPERF)
    add_definitions(-DENABLE_GHPERF)
endif()

if (TCMALLOC)
    add_definitions(-DENABLE_TCMALLOC)
endif()

# for slash test
if (SLASH_TEST)
    add_definitions(-DSLASH_TEST)
endif()

# for workload test
if (WORKLOAD_TEST)
    add_definitions(-DWORKLOAD_TEST)
endif()

if (XENABLE_PSTACK)
    add_definitions(-DXENABLE_PSTACK)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -rdynamic")
endif()

if (XENABLE_MEMCHECK_DBG)
    add_definitions(-DXENABLE_MEMCHECK_DBG)
endif()

if (XENABLE_P2P_BENDWIDTH)
    add_definitions(-DXENABLE_P2P_BENDWIDTH)
endif()

if (NO_TX_BATCH)
    add_definitions(-DNO_TX_BATCH)
endif()

if (ENABLE_CREATE_USER)
    add_definitions(-DENABLE_CREATE_USER)
endif()

if (DB_KV_STATISTIC)
    add_definitions(-DDB_KV_STATISTIC)
endif()

if (DB_CACHE)
    add_definitions(-DDB_CACHE)
endif()

if (XBUILD_CI)
    add_definitions(-DXBUILD_CI)
endif()

if (XBUILD_DEV)
    add_definitions(-DXBUILD_DEV)
endif()

if (XBUILD_GALILEO)
    add_definitions(-DXBUILD_GALILEO)
endif()

if (XBUILD_BOUNTY)
    add_definitions(-DXBUILD_BOUNTY)
endif()

if (BUILD_RUSTVM)
    add_definitions(-DBUILD_RUSTVM)
endif()

if (LEAK_TRACER)
    add_definitions(-DLEAK_TRACER)
endif()

if (STORE_UNIT_BLOCK)
    add_definitions(-DSTORE_UNIT_BLOCK)
endif()

if (XCHAIN_FORKED_BY_DEFAULT)
    add_definitions(-DXCHAIN_FORKED_BY_DEFAULT=${XCHAIN_FORKED_BY_DEFAULT})
    message("XCHAIN_FORKED_BY_DEFAULT: ${XCHAIN_FORKED_BY_DEFAULT}")
endif()

if (ENABLE_METRICS_DATAOBJECT)
    if (NOT BUILD_METRICS)
        message(FATAL_ERROR "should define metrics before metrics_dataobject")
    endif()
    add_definitions(-DENABLE_METRICS_DATAOBJECT)
endif()

if (DISABLE_CORE_SIGNAL_CAPTURE)
    add_definitions(-DDISABLE_CORE_SIGNAL_CAPTURE)
endif()

if (DISABLE_SIGNAL_CAPTURE)
    add_definitions(-DDISABLE_SIGNAL_CAPTURE)
endif()

if (${CMAKE_BUILD_TYPE} STREQUAL RelWithDebInfo)
    add_definitions(-DRELEASEDEBINFO)
endif()

if (CHECKPOINT_TEST)
    add_definitions(-DCHECKPOINT_TEST)
endif()

if (ETH_BRIDGE_TEST)
    add_definitions(-DETH_BRIDGE_TEST)
endif()

if (ETH2_SEPOLIA)
    add_definitions(-DETH2_SEPOLIA)
endif()

if (CACHE_SIZE_STATISTIC)
    add_definitions(-DCACHE_SIZE_STATISTIC)
endif()

if (CACHE_SIZE_STATISTIC_MORE_DETAIL)
    add_definitions(-DCACHE_SIZE_STATISTIC_MORE_DETAIL)
endif()

if (PERIOD_MOCK)
    add_definitions(-DPERIOD_MOCK)
endif()

if (CROSS_TX_DBG)
    add_definitions(-DCROSS_TX_DBG)
endif()

if (XBUILD_CONSORTIUM)
    add_definitions(-DXBUILD_CONSORTIUM)
endif()

message(STATUS "CMAKE_BUILD_TYPE:" ${CMAKE_BUILD_TYPE})
message(STATUS "CMAKE_SYSTEM_NAME:" ${CMAKE_SYSTEM_NAME})
message(STATUS "CMAKE_CXX_COMPILER_ID:" ${CMAKE_CXX_COMPILER_ID})
message(STATUS "XENABLE_CODE_COVERAGE:" ${XENABLE_CODE_COVERAGE})
message(STATUS "XBUILD_TEST:" ${XBUILD_TEST})
message(STATUS "BUILD_METRICS:" ${BUILD_METRICS})
message(STATUS "ADDRESS_SANITIZER:" ${ADDRESS_SANITIZER})
message(STATUS "XCHAIN_FORKED_BY_DEFAULT:" ${XCHAIN_FORKED_BY_DEFAULT})
message(STATUS "DISABLE_CORE_SIGNAL_CAPTURE:" ${DISABLE_CORE_SIGNAL_CAPTURE})
message(STATUS "DISABLE_SIGNAL_CAPTURE:" ${DISABLE_SIGNAL_CAPTURE})
message(STATUS "XUSE_JEMALLOC:" ${XUSE_JEMALLOC})
message(STATUS "LIBRARY_OUTPUT_DIRECTORY:" ${LIBRARY_OUTPUT_DIRECTORY})
message(STATUS "RUNTIME_OUTPUT_DIRECTORY:" ${RUNTIME_OUTPUT_DIRECTORY})

find_package(Threads REQUIRED)
if (XUSE_JEMALLOC)
    link_directories(${CMAKE_CURRENT_SOURCE_DIR}/src/xtopcom/xdepends/jemalloc/lib)
    link_libraries(jemalloc_pic)
elseif(BUILD_GPERF OR BUILD_GHPERF OR TCMALLOC)
    include_directories(SYSTEM src/xtopcom/xdepends/gperftools/src)
    link_directories(src/xtopcom/xdepends/gperftools/.libs)
endif()

include_directories(src/xtopcom)
include_directories(src/xtopcom/xtopcl/src)  # xtopcom build warning will not print
include_directories(src/xtopcom/xtopcl/include)  # xtopcom build warning will not print
include_directories(src/libraries)
include_directories(src/services)
include_directories(${CMAKE_CURRENT_BINARY_DIR})

if (${XSYS_VER} EQUAL 0)
    include_directories(SYSTEM src/xtopcom/xdepends/include/trezor-crypto)
    include_directories(SYSTEM src/xtopcom/xdepends/include)
    include_directories(SYSTEM src/xtopcom/xdepends/GSL/include)

    link_directories(src/xtopcom/xdepends/libs/${XSYSTEM_NAME_LOWERCASE})

    add_subdirectory(src/xtopcom/xdepends)
else()
    include_directories(SYSTEM third_party/include)
    include_directories(SYSTEM third_party/asio/asio/include)
    include_directories(SYSTEM third_party/trezor)
    include_directories(SYSTEM third_party/trezor/trezor-crypto)
    include_directories(SYSTEM third_party/jsoncpp/include)
    include_directories(SYSTEM third_party/ethash/include)
    include_directories(SYSTEM third_party/ethash/lib)
    include_directories(SYSTEM third_party/nlohmann/json/single_include)
    include_directories(SYSTEM third_party/nlohmann/fifo_map/src)
    include_directories(SYSTEM third_party/crossguid/include)
    include_directories(SYSTEM third_party/lz4/lib)

    link_directories(third_party/prebuild/libs/${XSYSTEM_NAME_LOWERCASE})

    add_subdirectory(third_party)
endif()

link_directories(${LIBRARY_OUTPUT_DIRECTORY})
link_directories(src/xtopcom/xbase/libs/${CMAKE_SYSTEM_NAME}/${XBUILD_TYPE}/${XOS_ID})

set(XDB_TYPE "ROCKSDB")
add_subdirectory(src)

if (XBUILD_TEST)
    enable_testing()

    if (${XSYS_VER} EQUAL 0)
        include_directories(SYSTEM src/xtopcom/xdepends/googletest/googletest/include)
        include_directories(SYSTEM src/xtopcom/xdepends/googletest/googlemock/include)
    endif()
    include_directories(.)

    add_subdirectory(tests)

endif()
