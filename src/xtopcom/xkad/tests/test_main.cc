// Copyright (c) 2017-2019 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <fstream>

#include "xpbase/base/top_log.h"
#include "xpbase/base/top_utils.h"
#include "xpbase/base/top_config.h"
#include "xkad/routing_table/routing_utils.h"

using namespace top;

int main(int argc, char *argv[]) {
    global_platform_type = kPlatform;
    global_node_id = RandomString(256);
    global_node_id_hash = std::string("");

    {
        const std::string str_file = 
R"(
[log]
path = ./log/bitvpn.log

[node]
first_node = false
local_ip = 127.0.0.1
local_port = 0
client_mode = false
country = US
business = EDGE
public_endpoints = 127.0.0.1:9000
show_cmd = false
role = 1

[db]
path = ./db

)";
        std::fstream fout("/tmp/test_routing_table.conf", std::ios::out);
        fout << str_file;
        fout.close();
    }

    xinit_log("bitvpn_ut.log", true, true);
    xset_log_level(enum_xlog_level_debug);
    top::base::Config config;
    config.Init("/tmp/test_routing_table.conf");
    top::kadmlia::CreateGlobalXid(config);

    testing::GTEST_FLAG(output) = "xml:";
    testing::InitGoogleTest(&argc, argv);
    ::testing::InitGoogleMock(&argc, argv);
    int ret = RUN_ALL_TESTS();
    SleepMs(10 * 1000);
    TOP_INFO("exit");
    //_exit(0);
    return ret;
}
