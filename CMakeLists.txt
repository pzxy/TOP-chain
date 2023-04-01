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
include(cmake/gitinfo.cmake)
get_git_info()

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

set(EXECUTABLE_OUTPUT_PATH ${XTOPCHAIN_BINARY_DIR}/bin/${CMAKE_SYSTEM_NAME}/)
set(LIBRARY_OUTPUT_PATH ${XTOPCHAIN_BINARY_DIR}/lib/${CMAKE_SYSTEM_NAME}/)

if ("${LIBRARY_OUTPUT_DIRECTORY}" STREQUAL "")
    set(LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})
endif()

if ("${RUNTIME_OUTPUT_DIRECTORY}" STREQUAL "")
    set(RUNTIME_OUTPUT_DIRECTORY ${EXECUTABLE_OUTPUT_PATH})
endif()

if (${CMAKE_CXX_COMPILER_ID} STREQUAL GNU)
    # common compiling options
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

    if (${XOS_ID} MATCHES "ubuntu")
        add_compile_options(-static-pie)
    else()
        add_compile_options(-fpie -fPIE)
    endif()

    # CXX only
    add_compile_options($<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-frtti>)
    add_compile_options($<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-fexceptions>)
    add_compile_options($<$<STREQUAL:$<TARGET_PROPERTY:LINKER_LANGUAGE>,CXX>:-fthreadsafe-statics>)

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
    set_property(DIRECTORY APPEND PROPERTY COMPILE_OPTIONS $<$<OR:$<CONFIG:Debug>,$<CONFIG:debug>>:-g3 -ggdb -O0 -fno-omit-frame-pointer>)
    set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<OR:$<CONFIG:Debug>,$<CONFIG:debug>>:DEBUG _DEBUG>)

    # options for Release & RelWithDebInfo
    # add_compile_options($<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:release>>:-fomit-frame-pointer -ffunction-sections -fdata-sections>)
    set_property(DIRECTORY APPEND PROPERTY COMPILE_OPTIONS $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:release>>:-ffunction-sections -fdata-sections -flto=auto -fuse-linker-plugin>)
    set_property(DIRECTORY APPEND PROPERTY COMPILE_DEFINITIONS $<$<OR:$<CONFIG:Release>,$<CONFIG:RelWithDebInfo>,$<CONFIG:release>>:NDEBUG>)

    # options for Release
    add_compile_options($<$<OR:$<CONFIG:Release>,$<CONFIG:release>>:-g0>)

    # options for RelWithDebInfo
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
