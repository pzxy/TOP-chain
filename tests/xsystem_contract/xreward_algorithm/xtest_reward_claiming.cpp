// Copyright (c) 2017-2018 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <gtest/gtest.h>

#include <sstream>

#define private public

#include "xbase/xcontext.h"
#include "xconfig/xconfig_register.h"
#include "xdata/xnative_contract_address.h"
#include "xdata/xsystem_contract/xdata_structures.h"
#include "xstore/xaccount_context.h"
#include "xvm/xsystem_contracts/xreward/xtable_reward_claiming_contract.h"

using namespace top;
using namespace std;
using namespace top::base;
using contract_t = top::xvm::system_contracts::reward::xtable_reward_claiming_contract_t;

class xtop_test_reward_claiming : public testing::Test {
public:
    xtop_test_reward_claiming() {
    }
    ~xtop_test_reward_claiming() {
    }
    contract_t m_contract{common::xbeacon_network_id};

    std::map<string, string> voters;
    std::map<string, std::map<string, string>> pledge_votes_map;
    std::map<string, uint64_t> stored_expire_token_map;
    std::map<string, std::map<string, uint64_t>> votes_table_map;
    std::map<string, string> adv_votes;
    common::xlogic_time_t m_timer_height;

    void set_voters(string voter_account, string adv_account, uint64_t voter_num) {
        auto iter = voters.find(voter_account);
        std::map<std::string, uint64_t> votes_table;
        if (iter != voters.end()) {
            auto const & vote_table_str = iter->second;
            base::xstream_t stream(base::xcontext_t::instance(), (uint8_t *)vote_table_str.c_str(), (uint32_t)vote_table_str.size());
            stream >> votes_table;
        }
        votes_table[adv_account] = voter_num;
        base::xstream_t stream(base::xcontext_t::instance());
        stream << votes_table;
        string v1 = string((char *)stream.data(), stream.size());
        voters[voter_account] = v1;
    }

    void set_pledge_votes_map(string account, uint64_t lock_time, uint16_t duration, uint64_t vote_num) {
        auto m = pledge_votes_map.find(account);
        std::map<string, string> pledge_votes;
        if (m != pledge_votes_map.end()) {
            pledge_votes = m->second;
        }

        base::xstream_t first_stream{base::xcontext_t::instance()};
        first_stream << duration;
        first_stream << lock_time;

        base::xstream_t second_stream{base::xcontext_t::instance()};
        second_stream << vote_num;

        string first = string((char *)first_stream.data(), first_stream.size());
        string second = string((char *)second_stream.data(), second_stream.size());

        pledge_votes[first] = second;
        pledge_votes_map[account] = pledge_votes;
    }

    void set_stored_expire_token_map(string account, uint64_t expire_token) {
        stored_expire_token_map[account] = expire_token;
    }

    void set_now_time_height(uint64_t height) {
        m_timer_height = height;
    }

    void call_test_function() {
        m_contract.calc_section_votes_table_and_adv_vote(voters, pledge_votes_map, stored_expire_token_map, votes_table_map, adv_votes, 1, m_timer_height);
        std::cout << "call over" << std::endl;
    }

    void print_result() {
        std::cout << ">>> print adv_votes size: " << adv_votes.size() << std::endl;
        for (auto const & v : adv_votes) {
            std::cout << "> adv: " << v.first << ", gain_valid_sum: " << base::xstring_utl::touint64(v.second) << std::endl;
        }

        std::cout << ">>> print votes_table_map size:" << votes_table_map.size() << std::endl;
        for (auto const & v : votes_table_map) {
            for (auto const & vv : v.second) {
                std::cout << "> account: " << v.first << " {adv: " << vv.first << ", gain_valid_num: " << vv.second << "}" << std::endl;
            }
        }
    }

    void assert_adv_votes(string adv, uint64_t num) {
        string adv_gain_sum = adv_votes[adv];
        EXPECT_EQ(num, base::xstring_utl::touint64(adv_gain_sum));
    }
    void assert_votes_table_map(string account, string adv, uint64_t num) {
        std::map<string, uint64_t> votes_table = votes_table_map[account];
        uint64_t account_to_adv_num = votes_table[adv];
        EXPECT_EQ(num, account_to_adv_num);
    }
    void assert_sum() {
        std::map<string, uint64_t> new_adv_votes;
        for (auto const & iter : votes_table_map) {
            std::map<string, uint64_t> votes_table = iter.second;
            for (auto const & iter2 : votes_table) {
                string adv = iter2.first;
                auto v = new_adv_votes.find(adv);
                if (v != new_adv_votes.end()) {
                    new_adv_votes[adv] = v->second + iter2.second;
                } else {
                    new_adv_votes.insert({adv, iter2.second});
                }
            }
        }
        EXPECT_NE(0, new_adv_votes.size());
        EXPECT_EQ(adv_votes.size(), new_adv_votes.size());
        for (auto const & iter : adv_votes) {
            auto v = new_adv_votes[iter.first];
            EXPECT_EQ(v, base::xstring_utl::touint64(iter.second));
        }
    }
};

using xtest_reward_claiming_t = xtop_test_reward_claiming;

TEST_F(xtest_reward_claiming_t, test_demo) {
    // std::map<std::string, uint64_t> m1;
    // std::map<std::string, std::map<std::string, uint64_t>> m;
    // std::map<std::string, std::map<std::string, uint64_t>> m2;

    // m1["1123"] = 1021;
    // m1["214123"] = 2334;
    // m["mdd"] = m1;
    // base::xstream_t stream(top::base::xcontext_t::instance());
    // stream << m;
    // auto v = std::string((char *)stream.data(), stream.size());
    // std::cout << v << std::endl;

    // base::xstream_t stream2(top::base::xcontext_t::instance(), (uint8_t *)v.data(), v.size());
    // stream2 >> m2;
    // for (auto const & iter : m2) {
    //     std::cout << "k:" << iter.first << endl;
    //     for (auto const & iter2 : iter.second) {
    //         std::cout << "v: " << iter2.first, std::cout << "," << iter2.second << std::endl;
    //     }
    // }

    std::map<std::string, std::string> table_effective_rewards;
    std::map<std::string, std::string> table_effective_reward;
    std::map<std::string, std::string> reward_infos;
    std::map<std::string, std::map<std::string, std::string>> pledge_infos;
    std::map<std::string, std::map<std::string, uint64_t>> voter_infos;
    reward_infos["adv1"] = "1001";
    reward_infos["adv2"] = "1002";
    reward_infos["adv3"] = "1003";

    std::map<std::string, std::string> pledge_info;
    pledge_info["expired_token"] = "1000000000";
    pledge_info["0_0"] = "1000";
    pledge_info["0_30"] = "10000";
    pledge_infos["account1"] = pledge_info;

    std::map<std::string, uint64_t> voter_info;
    voter_info["account1"] = 1000;
    voter_info["account2"] = 2000;
    voter_infos["adv1"] = voter_info;

    base::xstream_t stream1(top::base::xcontext_t::instance());
    stream1 << reward_infos;
    auto reward_infos_str = std::string((char *)stream1.data(), stream1.size());
    table_effective_reward["reward_infos"] = reward_infos_str;

    base::xstream_t stream2(top::base::xcontext_t::instance());
    stream2 << pledge_infos;
    auto pledge_infos_str = std::string((char *)stream2.data(), stream2.size());
    table_effective_reward["pledge_infos"] = pledge_infos_str;

    base::xstream_t stream3(top::base::xcontext_t::instance());
    stream3 << voter_infos;
    auto voter_infos_str = std::string((char *)stream3.data(), stream3.size());
    table_effective_reward["voter_infos"] = voter_infos_str;

    base::xstream_t stream4(top::base::xcontext_t::instance());
    stream4 << table_effective_reward;
    auto table_effective_reward_str = std::string((char *)stream4.data(), stream4.size());

    table_effective_rewards["contract_addr1"] = table_effective_reward_str;

    Json::Value jv;
    for (auto const & table_effective_reward : table_effective_rewards) {
        auto const & contract = table_effective_reward.first;
        auto const & table_detail_str = table_effective_reward.second;
        Json::Value jvt1;
        std::map<std::string, std::string> table_detail;
        base::xstream_t stream(xcontext_t::instance(), (uint8_t *)table_detail_str.data(), table_detail_str.size());
        stream >> table_detail;
        for (auto const & detail : table_detail) {  // {reward:}
            auto key = detail.first;
            auto value_str = detail.second;
            Json::Value array;
            if (key == "reward_infos") {
                std::map<std::string, std::string> reward_infos;
                base::xstream_t stream(xcontext_t::instance(), (uint8_t *)value_str.data(), value_str.size());
                stream >> reward_infos;
                for (auto const & reward : reward_infos) {
                    Json::Value n;
                    n["account_addr"] = reward.first;
                    n["count"] = reward.second;
                    array.append(n);
                }
                jvt1["reward_infos"] = array;
            } else if (key == "pledge_infos") {
                std::map<std::string, std::map<std::string, std::string>> pledge_infos;
                base::xstream_t stream(xcontext_t::instance(), (uint8_t *)value_str.data(), value_str.size());
                stream >> pledge_infos;
                for (auto const & pledge : pledge_infos) {
                    Json::Value n;
                    n["account_addr"] = pledge.first;
                    Json::Value pledge_votes_arr;
                    for (auto const & v : pledge.second) {
                        if (v.first == "expired_token") {
                            n["expired_token"] = v.second;
                        } else {
                            Json::Value n2;
                            n2["locktimer_duration"] = v.first;
                            n2["count"] = (Json::UInt64)base::xstring_utl::touint64(v.second);
                            pledge_votes_arr.append(n2);
                        }
                    }
                    n["pledge_votes"] = pledge_votes_arr;
                    array.append(n);
                }
                jvt1["pledge_infos"] = array;
            } else {
                std::map<std::string, std::map<std::string, uint64_t>> voter_infos;
                base::xstream_t stream(xcontext_t::instance(), (uint8_t *)value_str.data(), value_str.size());
                stream >> voter_infos;
                for (auto const & v : voter_infos) {
                    Json::Value n;
                    n["account_addr"] = v.first;
                    Json::Value voters_arr;
                    for (auto const & v2 : v.second) {
                        Json::Value n2;
                        n2["account_addr"] = v2.first;
                        n2["count"] = (Json::UInt64)v2.second;
                        voters_arr.append(n2);
                    }
                    n["voters"] = voters_arr;
                    array.append(n);
                }
                jvt1["voter_infos"] = array;
            }
        }
        jvt1["account_addr"] = contract;
        jv["table_effective_rewards"].append(jvt1);
    }
    std::cout << jv.toStyledString () << std::endl;

    // current timer height
    string account = "T80000b2a8697e7a72542637d1b6c696642ad97bec6feb";
    string adv = "T00000LYhXYm8rrkfKkYrrjT8ibbFtdi1SJDrHRD";
    set_now_time_height(259200 + 10);
    // voters: {T80000b2a8697e7a72542637d1b6c696642ad97bec6feb: {T00000LYhXYm8rrkfKkYrrjT8ibbFtdi1SJDrHRD: 1000}}
    set_voters(account, adv, 1000);
    // pledge_votes_map: {T80000b2a8697e7a72542637d1b6c696642ad97bec6feb: {11248201_30: 5000}}
    // expired:  m_timer_height - lock_time >= duration * 24 * 60 * 6
    // m_timer_height - 10 >= 30 * 24 * 60 * 6 = 259200
    set_pledge_votes_map(account, 10, 30, 5000);
    set_pledge_votes_map(account, 20, 30, 2000);
    set_pledge_votes_map(account, 0, 0, 600);
    // stored_expire_token_map: {T80000b2a8697e7a72542637d1b6c696642ad97bec6feb: 300000000}
    set_stored_expire_token_map(account, 300000000);

    call_test_function();

    // calculation details
    string adv_gain_sum = adv_votes[adv];
    std::map<string, uint64_t> votes_table = votes_table_map[account];
    uint64_t account_to_adv_num = votes_table[adv];
    // stored_expire_token = stored_expire_token_map[account]; 300000000
    // calc_expire_token = if (expired && duration !=0 ) {return get_top_by_vote(vote_num, duration)}; get_top_by_vote(5000, 30) => 5000000000
    // unexpire_vote_num = if (!expired) ; 2000
    // valid_vote_sum = (stored_expire_token + calc_expire_token) / 1000000 + unexpire_vote_num; (300000000 + 5000000000)/1000000 + 2000 = 7300;
    // vote_sum = add(pledge_votes_map.second); 5000 + 2000 + 600 = 7600
    EXPECT_EQ(960, account_to_adv_num);                         // 1000 * 7300/7600 = 960
    EXPECT_EQ(960, base::xstring_utl::touint64(adv_gain_sum));  // adv_gain_sum + 960 = 960

    assert_adv_votes(adv, 960);
    assert_votes_table_map(account, adv, 960);
    assert_sum();
    // print_result();
}

TEST_F(xtest_reward_claiming_t, test_one_account_vote_one_adv) {
    set_now_time_height(4924800 + 10);
    string account = "T80000b2a8697e7a72542637d1b6c696642ad97bec6feb";
    string adv = "T00000LYhXYm8rrkfKkYrrjT8ibbFtdi1SJDrHRD";
    set_voters(account, adv, 1000);
    set_pledge_votes_map(account, 10, 570, 5000);  // 4924800 + 10 - 10 >= 570 * 24 * 60 * 6 , expired, get_top_by_vote(5000, 570) -> 2500000000 token
    call_test_function();

    // valid_vote_sum = 2500000000/1000000 = 2500
    // vote_sum = 5000;
    assert_adv_votes(adv, 500);                 // 1000 * 2500 / 5000 = 500
    assert_votes_table_map(account, adv, 500);  // 500 = 500
    // print_result();
}

TEST_F(xtest_reward_claiming_t, test_one_account_vote_more_adv) {
    set_now_time_height(4924800 + 10);
    string account = "T80000b2a8697e7a72542637d1b6c696642ad97bec6feb";
    string adv1 = "T00000LYhXYm8rrkfKkYrrjT8ibbFtdi1SJDrHRD";
    string adv2 = "T00000LNmcQCfjjk1Dj691L75egthyzyBAdkRtx4";
    set_voters(account, adv1, 1000);
    set_voters(account, adv2, 500);
    set_pledge_votes_map(account, 10, 570, 5000);  // 4924800 + 10 - 10 >= 570 * 24 * 60 * 6 , expired, get_top_by_vote(5000, 570) -> 2500000000 token
    set_pledge_votes_map(account, 20, 570, 3000);  // 4924800 + 10 - 20 < 570 * 24 * 60 * 6 , not expired, 3000 ticket;
    call_test_function();

    // valid_vote_sum = 2500000000/1000000 + 3000 = 5500
    // vote_sum = 8000;
    assert_adv_votes(adv1, 687);                 // 1000 * 5500 / 8000 = 687
    assert_adv_votes(adv2, 343);                 // 500 * 5500 / 8000 = 343
    assert_votes_table_map(account, adv1, 687);  //  687
    assert_votes_table_map(account, adv2, 343);  //  343
    assert_sum();
    // print_result();
}

TEST_F(xtest_reward_claiming_t, test_more_account_vote_one_adv) {
    set_now_time_height(4924800 + 10);
    string account1 = "T80000b2a8697e7a72542637d1b6c696642ad97bec6feb";
    string account2 = "T80000a36a6d82fa55894ea36d7b043522b808fef45695";
    string adv1 = "T00000LYhXYm8rrkfKkYrrjT8ibbFtdi1SJDrHRD";

    set_voters(account1, adv1, 1000);
    set_pledge_votes_map(account1, 10, 570, 5000);  // 4924800 + 10 - 10 >= 570 * 24 * 60 * 6 , expired, get_top_by_vote(5000, 570) -> 2500000000 token
    set_pledge_votes_map(account1, 20, 570, 3000);  // 4924800 + 10 - 20 < 570 * 24 * 60 * 6 , not expired, 3000 ticket;    set_pledge_votes_map(account, 10, 570, 5000);  //

    set_voters(account2, adv1, 500);
    set_pledge_votes_map(account2, 10, 570, 3000);  // 4924800 + 10 - 10 >= 570 * 24 * 60 * 6 ,expired, get_top_by_vote(3000, 570) -> 1500000000 token
    set_pledge_votes_map(account2, 20, 570, 3000);  // 4924800 + 10 - 20 < 570 * 24 * 60 * 6 , not expired, 3000 ticket;

    call_test_function();
    // account 1:
    // valid_vote_sum = 2500000000/1000000 + 3000 = 5500
    // vote_sum = 8000;

    // account 2:
    // valid_vote_sum = 1500000000/1000000 + 3000 = 4500
    // vote_sum = 6000;
    assert_votes_table_map(account1, adv1, 687);  // 1000 * 5500 / 8000 = 687
    assert_votes_table_map(account2, adv1, 375);  // 500 * 4500 / 6000 = 375
    assert_adv_votes(adv1, 1062);                 // 687 + 375 = 1062
    assert_sum();
    // print_result();
}

TEST_F(xtest_reward_claiming_t, test_more_account_vote_more_adv) {
    set_now_time_height(4924800 + 10);
    string account1 = "T80000b2a8697e7a72542637d1b6c696642ad97bec6feb";
    string account2 = "T80000a36a6d82fa55894ea36d7b043522b808fef45695";
    string adv1 = "T00000LYhXYm8rrkfKkYrrjT8ibbFtdi1SJDrHRD";
    string adv2 = "T00000LNmcQCfjjk1Dj691L75egthyzyBAdkRtx4";

    set_voters(account1, adv1, 1000);
    set_voters(account1, adv2, 2000);
    set_pledge_votes_map(account1, 10, 570, 5000);  // 4924800 + 10 - 10 >= 570 * 24 * 60 * 6 , expired, get_top_by_vote(5000, 570) -> 2500000000 token
    set_pledge_votes_map(account1, 20, 570, 3000);  // 4924800 + 10 - 20 < 570 * 24 * 60 * 6 , not expired, 3000 ticket;    set_pledge_votes_map(account, 10, 570, 5000);  //

    set_voters(account2, adv1, 500);
    set_voters(account2, adv2, 1000);
    set_pledge_votes_map(account2, 10, 570, 3000);  // 4924800 + 10 - 10 >= 570 * 24 * 60 * 6 ,expired, get_top_by_vote(3000, 570) -> 1500000000 token
    set_pledge_votes_map(account2, 20, 570, 3000);  // 4924800 + 10 - 20 < 570 * 24 * 60 * 6 , not expired, 3000 ticket;

    call_test_function();
    // account 1:
    // valid_vote_sum = 2500000000/1000000 + 3000 = 5500
    // vote_sum = 8000;

    // account 2:
    // valid_vote_sum = 1500000000/1000000 + 3000 = 4500
    // vote_sum = 6000;
    assert_votes_table_map(account1, adv1, 687);   // 1000 * 5500 / 8000 = 687
    assert_votes_table_map(account1, adv2, 1375);  // 2000 * 5500 / 8000 = 1375

    assert_votes_table_map(account2, adv1, 375);  // 500 * 4500 / 6000 = 375
    assert_votes_table_map(account2, adv2, 750);  // 1000 * 4500 / 6000 = 750
    assert_adv_votes(adv1, 1062);                 // 687 + 375 = 1062
    assert_adv_votes(adv2, 2125);                 // 1375 + 750 = 2125
    assert_sum();
    // print_result();
}

TEST_F(xtest_reward_claiming_t, test_more_account_vote_more_adv_expired) {
    set_now_time_height(4924800 + 10);
    string account1 = "T80000b2a8697e7a72542637d1b6c696642ad97bec6feb";
    string account2 = "T80000a36a6d82fa55894ea36d7b043522b808fef45695";
    string adv1 = "T00000LYhXYm8rrkfKkYrrjT8ibbFtdi1SJDrHRD";
    string adv2 = "T00000LNmcQCfjjk1Dj691L75egthyzyBAdkRtx4";

    set_voters(account1, adv1, 1000);
    set_voters(account1, adv2, 2000);
    set_pledge_votes_map(account1, 10, 570, 5000);  // 4924800 + 10 - 10 >= 570 * 24 * 60 * 6 , expired, get_top_by_vote(5000, 570) -> 2500000000 token
    set_pledge_votes_map(account1, 20, 570, 3000);  // 4924800 + 10 - 20 < 570 * 24 * 60 * 6 , not expired, 3000 ticket;    set_pledge_votes_map(account, 10, 570, 5000);  //

    set_stored_expire_token_map(account1, 2000000000);
    set_pledge_votes_map(account1, 0, 0, 3000);

    set_voters(account2, adv1, 500);
    set_voters(account2, adv2, 1000);
    set_pledge_votes_map(account2, 10, 570, 3000);  // 4924800 + 10 - 10 >= 570 * 24 * 60 * 6 ,expired, get_top_by_vote(3000, 570) -> 1500000000 token
    set_pledge_votes_map(account2, 20, 570, 3000);  // 4924800 + 10 - 20 < 570 * 24 * 60 * 6 , not expired, 3000 ticket;

    set_stored_expire_token_map(account2, 1800000000);
    set_pledge_votes_map(account2, 0, 0, 2000);

    call_test_function();
    // account 1:
    // valid_vote_sum = (2500000000+2000000000)/1000000 + 3000 = 7500
    // vote_sum = 5000+3000+3000 = 11000

    // account 2:
    // valid_vote_sum = (1500000000+1800000000)/1000000 + 3000 = 6300
    // vote_sum = 3000 + 3000 + 2000 = 8000
    assert_votes_table_map(account1, adv1, 681);   // 1000 * 7500 / 11000 = 681
    assert_votes_table_map(account1, adv2, 1363);  // 2000 * 7500 / 11000 = 1363

    assert_votes_table_map(account2, adv1, 393);  // 500 * 6300 / 8000 = 393
    assert_votes_table_map(account2, adv2, 787);  // 1000 * 6300 / 8000 = 787
    assert_adv_votes(adv1, 1074);                 // 681 + 393 = 1074
    assert_adv_votes(adv2, 2150);                 // 1363 + 787 = 2150
    assert_sum();
    // print_result();
}