// Copyright (c) 2017-2018 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "xrec_consortium_registration_contract.h"

#include "secp256k1/secp256k1.h"
#include "secp256k1/secp256k1_recovery.h"
#include "xbase/xmem.h"
#include "xbase/xutl.h"
#include "xbasic/xutility.h"
#include "xchain_fork/xutility.h"
#include "xchain_upgrade/xchain_data_processor.h"
#include "xcommon/xrole_type.h"
#include "xdata/xgenesis_data.h"
#include "xdata/xnative_contract_address.h"
#include "xdata/xproperty.h"
#include "xdata/xrootblock.h"
#include "xmetrics/xmetrics.h"
#include "xstore/xstore_error.h"

using top::base::xcontext_t;
using top::base::xstream_t;
using namespace top::data;

#if !defined (XREC_MODULE)
#    define XREC_MODULE "sysContract_"
#endif
#define XCONTRACT_PREFIX "registration_"

#define XREG_CONTRACT XREC_MODULE XCONTRACT_PREFIX

NS_BEG3(top, xvm, consortium)

#define TIMER_ADJUST_DENOMINATOR 10

xrec_consortium_registration_contract::xrec_consortium_registration_contract(common::xnetwork_id_t const & network_id) : xbase_t{network_id} {}

void xrec_consortium_registration_contract::setup() {
    MAP_CREATE(data::system_contract::XPORPERTY_CONTRACT_REG_KEY);
    MAP_CREATE(data::system_contract::XPORPERTY_CONTRACT_TICKETS_KEY);
    MAP_CREATE(data::system_contract::XPORPERTY_CONTRACT_REFUND_KEY);
    STRING_CREATE(data::system_contract::XPORPERTY_CONTRACT_GENESIS_STAGE_KEY);
    MAP_CREATE(data::system_contract::XPROPERTY_CONTRACT_SLASH_INFO_KEY);

    std::vector<std::pair<std::string, std::string>> db_kv_101;
    chain_data::xchain_data_processor_t::get_stake_map_property(common::xlegacy_account_address_t{SELF_ADDRESS()}, data::system_contract::XPORPERTY_CONTRACT_REG_KEY, db_kv_101);
    for (auto const & _p : db_kv_101) {
        auto const & node_info_serialized = _p.second;

        base::xstream_t stream{ base::xcontext_t::instance(), reinterpret_cast<uint8_t *>(const_cast<char *>(node_info_serialized.data())), static_cast<uint32_t>(node_info_serialized.size()) };
        data::system_contract::xreg_node_info node_info;
        node_info.serialize_from(stream);

        auto const old_role = static_cast<common::xlegacy_miner_type_t>(node_info.miner_type());
        switch (old_role) {
        case common::xlegacy_miner_type_t::advance:
            node_info.miner_type(common::xminer_type_t::advance);
            break;

        case common::xlegacy_miner_type_t::consensus:
            node_info.miner_type(common::xminer_type_t::validator);
            break;

        case common::xlegacy_miner_type_t::archive:
            node_info.miner_type(common::xminer_type_t::archive);
            break;

        case common::xlegacy_miner_type_t::edge:
            node_info.miner_type(common::xminer_type_t::edge);
            break;

        default:
            break;
        }
        stream.reset();
        node_info.serialize_to(stream);

        MAP_SET(data::system_contract::XPORPERTY_CONTRACT_REG_KEY, _p.first, {reinterpret_cast<char *>(stream.data()), static_cast<size_t>(stream.size())});
    }

    std::vector<std::pair<std::string, std::string>> db_kv_128;
    chain_data::xchain_data_processor_t::get_stake_map_property(common::xlegacy_account_address_t{SELF_ADDRESS()}, data::system_contract::XPORPERTY_CONTRACT_REFUND_KEY, db_kv_128);
    for (auto const & _p : db_kv_128) {
        MAP_SET(data::system_contract::XPORPERTY_CONTRACT_REFUND_KEY, _p.first, _p.second);
    }

    std::vector<std::pair<std::string, std::string>> db_kv_132;
    chain_data::xchain_data_processor_t::get_stake_map_property(
        common::xlegacy_account_address_t{SELF_ADDRESS()}, data::system_contract::XPROPERTY_CONTRACT_SLASH_INFO_KEY, db_kv_132);
    for (auto const & _p : db_kv_132) {
        MAP_SET(data::system_contract::XPROPERTY_CONTRACT_SLASH_INFO_KEY, _p.first, _p.second);
    }

    std::string db_kv_129;
    chain_data::xchain_data_processor_t::get_stake_string_property(
        common::xlegacy_account_address_t{SELF_ADDRESS()}, data::system_contract::XPORPERTY_CONTRACT_GENESIS_STAGE_KEY, db_kv_129);
    if (!db_kv_129.empty()) {
        STRING_SET(data::system_contract::XPORPERTY_CONTRACT_GENESIS_STAGE_KEY, db_kv_129);
    }

    const uint32_t old_tables_count = 256;
    for (auto table = 0; table < enum_vledger_const::enum_vbucket_has_tables_count; table++) {
        std::string table_address{std::string{sys_contract_sharding_vote_addr} + "@" + std::to_string(table)};
        std::map<std::string, uint64_t> adv_get_votes_detail;
        for (auto i = 1; i <= data::system_contract::XPROPERTY_SPLITED_NUM; i++) {
            std::string property;
            property = property + data::system_contract::XPORPERTY_CONTRACT_VOTES_KEY_BASE + "-" + std::to_string(i);
            {
                std::map<std::string, std::map<std::string, uint64_t>> votes_detail;
                for (uint32_t j = 0; j < old_tables_count; j++) {
                    auto table_addr = std::string{sys_contract_sharding_vote_addr} + "@" + base::xstring_utl::tostring(j);
                    std::vector<std::pair<std::string, std::string>> db_kv_112;
                    chain_data::xchain_data_processor_t::get_stake_map_property(common::xlegacy_account_address_t{table_addr}, property, db_kv_112);
                    for (auto const & _p : db_kv_112) {
                        base::xvaccount_t vaccount{_p.first};
                        auto account_table_id = vaccount.get_ledger_subaddr();
                        if (static_cast<uint16_t>(account_table_id) != static_cast<uint16_t>(table)) {
                            continue;
                        }
                        std::map<std::string, uint64_t> votes;
                        base::xstream_t stream(base::xcontext_t::instance(), (uint8_t *)_p.second.c_str(), (uint32_t)_p.second.size());
                        stream >> votes;
                        for (auto const & vote : votes) {
                            if (votes_detail[_p.first].count(vote.first)) {
                                votes_detail[_p.first][vote.first] += vote.second;
                            } else {
                                votes_detail[_p.first][vote.first] = vote.second;
                            }
                        }
                    }
                }
                for (auto const & vote_detail : votes_detail) {
                    for (auto const & adv_get_votes : vote_detail.second) {
                        if (adv_get_votes_detail.count(adv_get_votes.first)) {
                            adv_get_votes_detail[adv_get_votes.first] += adv_get_votes.second;
                        } else {
                            adv_get_votes_detail[adv_get_votes.first] = adv_get_votes.second;
                        }
                    }
                }
            }
        }
        {
            if(adv_get_votes_detail.empty()) {
                continue;
            }
            std::map<std::string, std::string> adv_get_votes_str_detail;
            for (auto const & adv_get_votes : adv_get_votes_detail) {
                adv_get_votes_str_detail.insert(std::make_pair(adv_get_votes.first, base::xstring_utl::tostring(adv_get_votes.second)));
            }
            xstream_t stream(xcontext_t::instance());
            stream << adv_get_votes_str_detail;
            std::string adv_get_votes_str{std::string((const char*)stream.data(), stream.size())};
            MAP_SET(data::system_contract::XPORPERTY_CONTRACT_TICKETS_KEY, table_address, adv_get_votes_str);
        }
    }

    top::chain_data::data_processor_t data;
    top::chain_data::xtop_chain_data_processor::get_contract_data(common::xlegacy_account_address_t{SELF_ADDRESS()}, data);
    TOP_TOKEN_INCREASE(data.top_balance);

    auto const & source_address = SOURCE_ADDRESS();
    MAP_CREATE(data::system_contract::XPORPERTY_CONTRACT_VOTE_REPORT_TIME_KEY);
    MAP_SET(data::system_contract::XPORPERTY_CONTRACT_VOTE_REPORT_TIME_KEY, source_address, base::xstring_utl::tostring(0));

#ifdef MAINNET_ACTIVATED
    data::system_contract::xactivation_record record;
    record.activated = 1;
    record.activation_time = 1;

    base::xstream_t stream(base::xcontext_t::instance());
    record.serialize_to(stream);
    auto value_str = std::string((char *)stream.data(), stream.size());
    STRING_SET(data::system_contract::XPORPERTY_CONTRACT_GENESIS_STAGE_KEY, value_str);
#endif

    xdbg("[xrec_consortium_registration_contract::setup] pid:%d", getpid());
    data::system_contract::xreg_node_info node_info;
    {
        common::xnetwork_id_t network_id{top::config::to_chainid(XGET_CONFIG(chain_name))};
        const std::vector<node_info_t> & seed_nodes = data::xrootblock_t::get_seed_nodes();
        for (size_t i = 0; i < seed_nodes.size(); i++) {
            node_info_t node_data = seed_nodes[i];

            if (MAP_FIELD_EXIST(data::system_contract::XPORPERTY_CONTRACT_REG_KEY, node_data.m_account.to_string())) {
                continue;
            }
            node_info.m_account             = node_data.m_account;
            node_info.m_account_mortgage    = 0;
            node_info.genesis(true);
            node_info.miner_type(common::xminer_type_t::edge | common::xminer_type_t::advance | common::xminer_type_t::validator);
            node_info.m_network_ids.insert(network_id);
            node_info.nickname              = std::string("bootnode") + std::to_string(i + 1);
            node_info.consensus_public_key  = xpublic_key_t{node_data.m_publickey};
            //xdbg("[xrec_consortium_registration_contract::setup] pid:%d,node account: %s, public key: %s\n", getpid(), node_data.m_account.c_str(), node_data.m_publickey.c_str());
            update_node_info(node_info);
        }
    }
}

common::xminer_type_t legal_archive_address_check(common::xminer_type_t const & miner_type,
                                                  common::xaccount_address_t const & account_address) {
    static const std::vector<common::xaccount_address_t> legal_archive_address_list{common::xaccount_address_t{""}};
#if !defined(XENABLE_MOCK_ZEC_STAKE)
    // is archive but not legal, return invalid
    if (miner_type == common::xminer_type_t::archive &&
        std::find(legal_archive_address_list.begin(), legal_archive_address_list.end(), account_address) == legal_archive_address_list.end()) {
        return common::xminer_type_t::invalid;
    }
#endif
    return miner_type;
}

void xrec_consortium_registration_contract::registerNode(const std::string & miner_type_name,
                                              const std::string & nickname,
                                              const std::string & signing_key,
                                              const uint32_t dividend_rate
#if defined(XENABLE_MOCK_ZEC_STAKE)
                                            , common::xaccount_address_t const & registration_account
#endif  // #if defined(XENABLE_MOCK_ZEC_STAKE)
) {
    std::set<common::xnetwork_id_t> network_ids;
    common::xnetwork_id_t nid{top::config::to_chainid(XGET_CONFIG(chain_name))};
    network_ids.insert(nid);

#if defined(XENABLE_MOCK_ZEC_STAKE)
    registerNode2(miner_type_name, nickname, signing_key, dividend_rate, network_ids, registration_account);
#else   // #if defined(XENABLE_MOCK_ZEC_STAKE)
    registerNode2(miner_type_name, nickname, signing_key, dividend_rate, network_ids);
#endif  // #if defined(XENABLE_MOCK_ZEC_STAKE)
}

void xrec_consortium_registration_contract::registerNode2(const std::string & miner_type_name,
                                               const std::string & nickname,
                                               const std::string & signing_key,
                                               const uint32_t dividend_rate,
                                               const std::set<common::xnetwork_id_t> & network_ids
#if defined(XENABLE_MOCK_ZEC_STAKE)
                                               ,
                                               common::xaccount_address_t const & registration_account
#endif  // #if defined(XENABLE_MOCK_ZEC_STAKE)
) {
    XMETRICS_COUNTER_INCREMENT(XREG_CONTRACT "registerNode_Called", 1);
    XMETRICS_TIME_RECORD(XREG_CONTRACT "registerNode_ExecutionTime");

    auto const miner_type = legal_archive_address_check(common::to_miner_type(miner_type_name), common::xaccount_address_t{SOURCE_ADDRESS()});
    XCONTRACT_ENSURE(miner_type != common::xminer_type_t::invalid, "xrec_consortium_registration_contract::registerNode2: invalid node_type!");

#if defined(XENABLE_MOCK_ZEC_STAKE)
    auto const & account = registration_account;
#else   // #if defined(XENABLE_MOCK_ZEC_STAKE)
    auto const & account = common::xaccount_address_t{ SOURCE_ADDRESS() };
#endif  // #if defined(XENABLE_MOCK_ZEC_STAKE)
    xdbg("[xrec_consortium_registration_contract::registerNode2] call xregistration_contract registerNode(), balance: %lld, account: %s, node_types: %s, signing_key: %s, dividend_rate: %u\n",
         GET_BALANCE(),
         account.to_string().c_str(),
         miner_type_name.c_str(),
         signing_key.c_str(),
         dividend_rate);

    XCONTRACT_ENSURE(common::is_t0(account) || common::is_t8(account), "only T0 or T8 account is allowed to be registered as node account");

    data::system_contract::xreg_node_info node_info;
    auto ret = get_node_info(account.to_string(), node_info);
    XCONTRACT_ENSURE(ret != 0, "xrec_consortium_registration_contract::registerNode2: node exist!");
    XCONTRACT_ENSURE(is_valid_name(nickname), "xrec_consortium_registration_contract::registerNode: invalid nickname");
    XCONTRACT_ENSURE(dividend_rate >= 0 && dividend_rate <= 100, "xrec_consortium_registration_contract::registerNode: dividend_rate must be >=0 and be <= 100");

    const xtransaction_ptr_t trans_ptr = GET_TRANSACTION();
    XCONTRACT_ENSURE(!account.empty(),
                     "xrec_consortium_registration_contract::registerNode: account must be not empty");

    xstream_t stream(xcontext_t::instance(), reinterpret_cast<uint8_t *>(const_cast<char *>(trans_ptr->get_source_action_para().data())), static_cast<uint32_t>(trans_ptr->get_source_action_para().size()));

//check account reg info from node manage contract
    bool check_ret = check_node_valid(account.to_string());
    XCONTRACT_ENSURE(check_ret, "[xrec_consortium_registration_contract][registerNode2] failed!");   

    data::xproperty_asset asset_out{0};
    stream >> asset_out.m_token_name;
    stream >> asset_out.m_amount;

    node_info.m_account = account;
    node_info.miner_type(miner_type);
#if defined(XENABLE_MOCK_ZEC_STAKE)
    node_info.m_account_mortgage = 100000000000000;
#else   // #if defined(XENABLE_MOCK_ZEC_STAKE)
    node_info.m_account_mortgage += asset_out.m_amount;
#endif  // #if defined(XENABLE_MOCK_ZEC_STAKE)
    node_info.nickname                  = nickname;
    node_info.consensus_public_key      = xpublic_key_t{signing_key};
    node_info.m_support_ratio_numerator = dividend_rate;
    if (network_ids.empty()) {
        xdbg("[xrec_consortium_registration_contract::registerNode2] network_ids empty");
        common::xnetwork_id_t nid{top::config::to_chainid(XGET_CONFIG(chain_name))};
        node_info.m_network_ids.insert(nid);
    } else {
        std::string network_ids_str;
        for (auto const & net_id : network_ids) {
            network_ids_str += net_id.to_string() + ' ';
        }
        xdbg("[xrec_consortium_registration_contract::registerNode2] network_ids %s", network_ids_str.c_str());
        node_info.m_network_ids = network_ids;
    }

    init_node_credit(node_info);

    uint64_t const min_deposit = node_info.get_required_min_deposit();
    xdbg("[xrec_consortium_registration_contract::registerNode2] call xregistration_contract registerNode() pid:%d, transaction_type:%d, source action type: %d, m_deposit: %" PRIu64
         ", min_deposit: %" PRIu64 ", account: %s",
         getpid(),
         trans_ptr->get_tx_type(),
         trans_ptr->get_source_action_type(),
         asset_out.m_amount,
         min_deposit,
         account.to_string().c_str());
    //XCONTRACT_ENSURE(asset_out.m_amount >= min_deposit, "xrec_consortium_registration_contract::registerNode2: mortgage must be greater than minimum deposit");
    XCONTRACT_ENSURE(node_info.m_account_mortgage >= min_deposit, "xrec_consortium_registration_contract::registerNode2: mortgage must be greater than minimum deposit");

    update_node_info(node_info);
    check_and_set_genesis_stage();

    XMETRICS_COUNTER_INCREMENT(XREG_CONTRACT "registerNode_Executed", 1);
    XMETRICS_COUNTER_INCREMENT(XREG_CONTRACT "registeredUserCnt", 1);
}

void xrec_consortium_registration_contract::unregisterNode() {
    XMETRICS_COUNTER_INCREMENT(XREG_CONTRACT "unregisterNode_Called", 1);
    XMETRICS_TIME_RECORD(XREG_CONTRACT "unregisterNode_ExecutionTime");
    uint64_t cur_time = TIME();
    std::string const& account = SOURCE_ADDRESS();
    xdbg("[xrec_consortium_registration_contract::unregisterNode] call xregistration_contract unregisterNode(), balance: %lld, account: %s", GET_BALANCE(), account.c_str());

    data::system_contract::xreg_node_info node_info;
    auto ret = get_node_info(account, node_info);
    XCONTRACT_ENSURE(ret == 0, "xrec_consortium_registration_contract::unregisterNode: node not exist");

    data::system_contract::xslash_info s_info;
    if (get_slash_info(account, s_info) == 0 && s_info.m_staking_lock_time > 0) {
        XCONTRACT_ENSURE(cur_time - s_info.m_punish_time >= s_info.m_staking_lock_time, "[xrec_consortium_registration_contract::unregisterNode]: has punish time, cannot deregister now");
    }

    xdbg("[xrec_consortium_registration_contract::unregisterNode] call xregistration_contract unregisterNode() pid:%d, balance:%lld, account: %s, refund: %lld",
    getpid(), GET_BALANCE(), account.c_str(), node_info.m_account_mortgage);
    // refund
    // TRANSFER(account, node_info.m_account_mortgage);

    ins_refund(account, node_info.m_account_mortgage);

    delete_node_info(account);

    xdbg("[xrec_consortium_registration_contract::unregisterNode] finish call xregistration_contract unregisterNode() pid:%d", getpid());

    XMETRICS_COUNTER_DECREMENT(XREG_CONTRACT "registeredUserCnt", 1);
    XMETRICS_COUNTER_INCREMENT(XREG_CONTRACT "unregisterNode_Executed", 1);
}

void xrec_consortium_registration_contract::updateNodeInfo(const std::string & nickname, const int updateDepositType, const uint64_t deposit, const uint32_t dividend_rate, const std::string & node_types, const std::string & node_sign_key) {
    XMETRICS_COUNTER_INCREMENT(XREG_CONTRACT "updateNodeInfo_Called", 1);
    XMETRICS_TIME_RECORD(XREG_CONTRACT "updateNodeInfo_ExecutionTime");

    std::string const & account = SOURCE_ADDRESS();

    xdbg("[xrec_consortium_registration_contract::updateNodeInfo] call xregistration_contract updateNodeInfo() pid:%d, balance: %lld, account: %s, nickname: %s, updateDepositType: %u, deposit: %llu, dividend_rate: %u, node_types: %s, node_sign_key: %s",
         getpid(),
         GET_BALANCE(),
         account.c_str(),
         nickname.c_str(),
         updateDepositType,
         deposit,
         dividend_rate,
         node_types.c_str(),
         node_sign_key.c_str());

    data::system_contract::xreg_node_info node_info;
    auto ret = get_node_info(account, node_info);
    XCONTRACT_ENSURE(ret == 0, "xrec_consortium_registration_contract::updateNodeInfo: node does not exist!");
    XCONTRACT_ENSURE(is_valid_name(nickname) == true, "xrec_consortium_registration_contract::updateNodeInfo: invalid nickname");
    XCONTRACT_ENSURE(updateDepositType == 1 || updateDepositType == 2, "xrec_consortium_registration_contract::updateNodeInfo: invalid updateDepositType");
    XCONTRACT_ENSURE(dividend_rate >= 0 && dividend_rate <= 100, "xrec_consortium_registration_contract::updateNodeInfo: dividend_rate must be greater than or be equal to zero");
    auto const miner_type = legal_archive_address_check(common::to_miner_type(node_types), common::xaccount_address_t{account});
    XCONTRACT_ENSURE(miner_type != common::xminer_type_t::invalid, "xrec_consortium_registration_contract::updateNodeInfo: invalid node_type!");

    node_info.nickname          = nickname;
    node_info.miner_type(miner_type);

    uint64_t const min_deposit = node_info.get_required_min_deposit();
    if (updateDepositType == 1) { // stake deposit
        const xtransaction_ptr_t trans_ptr = GET_TRANSACTION();
        XCONTRACT_ENSURE(!account.empty(),
                         "xrec_consortium_registration_contract::updateNodeInfo: account must be not empty");
        if (trans_ptr->get_source_action_para().size() > 0) {
            xstream_t stream(xcontext_t::instance(), (uint8_t *)trans_ptr->get_source_action_para().data(), trans_ptr->get_source_action_para().size());
            data::xproperty_asset asset_out{0};
            stream >> asset_out.m_token_name;
            stream >> asset_out.m_amount;
            XCONTRACT_ENSURE(asset_out.m_amount == deposit, "xrec_consortium_registration_contract::updateNodeInfo: invalid deposit!");
            node_info.m_account_mortgage += asset_out.m_amount;
        }

        XCONTRACT_ENSURE(node_info.m_account_mortgage >= min_deposit, "xrec_consortium_registration_contract::updateNodeInfo: deposit not enough");
    } else {

        XCONTRACT_ENSURE(deposit <= node_info.m_account_mortgage && node_info.m_account_mortgage - deposit >= min_deposit, "xrec_consortium_registration_contract::updateNodeInfo: unstake deposit too big");
        if (deposit > 0) {
            TRANSFER(account, deposit);
        }
        node_info.m_account_mortgage -= deposit;
    }

    if (node_info.m_support_ratio_numerator != dividend_rate) {
        uint64_t cur_time = TIME();
        auto SDR_INTERVAL = XGET_ONCHAIN_GOVERNANCE_PARAMETER(dividend_ratio_change_interval);
        XCONTRACT_ENSURE(node_info.m_last_update_time == 0 || cur_time - node_info.m_last_update_time >= SDR_INTERVAL,
                     "xrec_consortium_registration_contract::updateNodeInfo: set must be longer than or equal to SDR_INTERVAL");

        node_info.m_support_ratio_numerator = dividend_rate;
        node_info.m_last_update_time = cur_time;
    }
    node_info.consensus_public_key = xpublic_key_t{node_sign_key};
    init_node_credit(node_info);

    update_node_info(node_info);
    XMETRICS_COUNTER_INCREMENT(XREG_CONTRACT "updateNodeInfo_Executed", 1);
}

void xrec_consortium_registration_contract::redeemNodeDeposit() {
    XMETRICS_COUNTER_INCREMENT(XREG_CONTRACT "redeemNodeDeposit_Called", 1);
    XMETRICS_TIME_RECORD(XREG_CONTRACT "redeemNodeDeposit_ExecutionTime");
    uint64_t cur_time = TIME();
    std::string const & account = SOURCE_ADDRESS();
    xdbg("[xrec_consortium_registration_contract::redeemNodeDeposit] pid:%d, balance: %lld, account: %s", getpid(), GET_BALANCE(), account.c_str());

    data::system_contract::xrefund_info refund;
    auto ret = get_refund(account, refund);
    XCONTRACT_ENSURE(ret == 0, "xrec_consortium_registration_contract::redeemNodeDeposit: refund not exist");
    XCONTRACT_ENSURE(cur_time - refund.create_time >= data::system_contract::REDEEM_INTERVAL,
                     "xrec_consortium_registration_contract::redeemNodeDeposit: interval must be greater than or equal to REDEEM_INTERVAL");

    xdbg("[xrec_consortium_registration_contract::redeemNodeDeposit] pid:%d, balance:%llu, account: %s, refund amount: %llu\n", getpid(), GET_BALANCE(), account.c_str(), refund.refund_amount);
    // refund
    TRANSFER(account, refund.refund_amount);

    del_refund(account);
    XMETRICS_COUNTER_INCREMENT(XREG_CONTRACT "redeemNodeDeposit_Executed", 1);
}

void xrec_consortium_registration_contract::setDividendRatio(uint32_t dividend_rate) {
    XMETRICS_COUNTER_INCREMENT(XREG_CONTRACT "setDividendRatio_Called", 1);
    XMETRICS_TIME_RECORD(XREG_CONTRACT "setDividendRatio_ExecutionTime");
    std::string const & account = SOURCE_ADDRESS();
    uint64_t cur_time = TIME();

    data::system_contract::xreg_node_info node_info;
    auto ret = get_node_info(account, node_info);

    XCONTRACT_ENSURE(ret == 0, "xrec_consortium_registration_contract::setDividendRatio: node not exist");

    xdbg("[xrec_consortium_registration_contract::setDividendRatio] pid:%d, balance: %lld, account: %s, dividend_rate: %u, cur_time: %llu, last_update_time: %llu\n",
         getpid(),
         GET_BALANCE(),
         account.c_str(),
         dividend_rate,
         cur_time,
         node_info.m_last_update_time);

    auto SDR_INTERVAL = XGET_ONCHAIN_GOVERNANCE_PARAMETER(dividend_ratio_change_interval);
    XCONTRACT_ENSURE(node_info.m_last_update_time == 0 || cur_time - node_info.m_last_update_time >= SDR_INTERVAL,
                     "xrec_consortium_registration_contract::setDividendRatio: set must be longer than or equal to SDR_INTERVAL");
    XCONTRACT_ENSURE(dividend_rate >= 0 && dividend_rate <= 100, "xrec_consortium_registration_contract::setDividendRatio: dividend_rate must be >=0 and be <= 100");

    xdbg("[xrec_consortium_registration_contract::setDividendRatio] call xregistration_contract registerNode() pid:%d, balance:%lld, account: %s\n",
         getpid(),
         GET_BALANCE(),
         account.c_str());
    node_info.m_support_ratio_numerator = dividend_rate;
    node_info.m_last_update_time = cur_time;

    update_node_info(node_info);
    XMETRICS_COUNTER_INCREMENT(XREG_CONTRACT "setDividendRatio_Executed", 1);
}

void xrec_consortium_registration_contract::setNodeName(const std::string & nickname) {
    XMETRICS_COUNTER_INCREMENT(XREG_CONTRACT "setNodeName_Called", 1);
    XMETRICS_TIME_RECORD(XREG_CONTRACT "setNodeName_ExecutionTime");
    std::string const & account = SOURCE_ADDRESS();

    XCONTRACT_ENSURE(is_valid_name(nickname) == true, "xrec_consortium_registration_contract::setNodeName: invalid nickname");

    data::system_contract::xreg_node_info node_info;
    auto ret = get_node_info(account, node_info);
    XCONTRACT_ENSURE(ret == 0, "xrec_consortium_registration_contract::setNodeName: node not exist");

    xdbg("[xrec_consortium_registration_contract::setNodeName] pid:%d, balance: %lld, account: %s, nickname: %s\n", getpid(), GET_BALANCE(), account.c_str(), nickname.c_str());
    XCONTRACT_ENSURE(node_info.nickname != nickname, "xrec_consortium_registration_contract::setNodeName: nickname can not be same");

    node_info.nickname = nickname;
    update_node_info(node_info);
    XMETRICS_COUNTER_INCREMENT(XREG_CONTRACT "setNodeName_Executed", 1);
}

void xrec_consortium_registration_contract::updateNodeSignKey(const std::string & node_sign_key) {
    XMETRICS_COUNTER_INCREMENT(XREG_CONTRACT "updateNodeSignKey_Called", 1);
    XMETRICS_TIME_RECORD(XREG_CONTRACT "updateNodeSignKey_ExecutionTime");
    std::string const & account = SOURCE_ADDRESS();

    data::system_contract::xreg_node_info node_info;
    auto ret = get_node_info(account, node_info);
    XCONTRACT_ENSURE(ret == 0, "xrec_consortium_registration_contract::updateNodeSignKey: node not exist");

    xdbg("[xrec_consortium_registration_contract::updateNodeSignKey] pid:%d, balance: %lld, account: %s, node_sign_key: %s\n", getpid(), GET_BALANCE(), account.c_str(), node_sign_key.c_str());
    XCONTRACT_ENSURE(node_info.consensus_public_key.to_string() != node_sign_key, "xrec_consortium_registration_contract::updateNodeSignKey: node_sign_key can not be same");

    node_info.consensus_public_key = xpublic_key_t{node_sign_key};
    update_node_info(node_info);
    XMETRICS_COUNTER_INCREMENT(XREG_CONTRACT "updateNodeSignKey_Executed", 1);
}

void xrec_consortium_registration_contract::update_node_info(data::system_contract::xreg_node_info const & node_info) {
    base::xstream_t stream(base::xcontext_t::instance());
    node_info.serialize_to(stream);

    std::string stream_str = std::string((char *)stream.data(), stream.size());

    XMETRICS_TIME_RECORD(XREG_CONTRACT "XPORPERTY_CONTRACT_REG_KEY_SetExecutionTime");
    MAP_SET(data::system_contract::XPORPERTY_CONTRACT_REG_KEY, node_info.m_account.to_string(), stream_str);
}

void xrec_consortium_registration_contract::delete_node_info(std::string const & account) {
    XMETRICS_TIME_RECORD(XREG_CONTRACT "XPORPERTY_CONTRACT_REG_KEY_RemoveExecutionTime");
    REMOVE(enum_type_t::map, data::system_contract::XPORPERTY_CONTRACT_REG_KEY, account);
}

int32_t xrec_consortium_registration_contract::get_node_info(const std::string & account, data::system_contract::xreg_node_info & node_info) {
    std::string value_str;
    {
        XMETRICS_TIME_RECORD(XREG_CONTRACT "XPORPERTY_CONTRACT_REG_KEY_GetExecutionTime");
        MAP_GET2(data::system_contract::XPORPERTY_CONTRACT_REG_KEY, account, value_str);
    }

    if (value_str.empty()) {
        xdbg("[xrec_consortium_registration_contract] account(%s) not exist pid:%d\n", account.c_str(), getpid());
        return store::xaccount_property_not_exist;
    }

    base::xstream_t stream(base::xcontext_t::instance(), (uint8_t *)value_str.c_str(), (uint32_t)value_str.size());

    node_info.serialize_from(stream);

    return 0;
}

bool xrec_consortium_registration_contract::check_if_signing_key_exist(const std::string & signing_key) {
    std::map<std::string, std::string> map_nodes;

    {
        XMETRICS_TIME_RECORD(XREG_CONTRACT "XPORPERTY_CONTRACT_REG_KEY_CopyGetExecutionTime");
        MAP_COPY_GET(data::system_contract::XPORPERTY_CONTRACT_REG_KEY, map_nodes);
    }

    for (auto const & it : map_nodes) {
        data::system_contract::xreg_node_info reg_node_info;
        xstream_t stream(xcontext_t::instance(), (uint8_t *)it.second.c_str(), it.second.size());
        reg_node_info.serialize_from(stream);
        if (reg_node_info.consensus_public_key.to_string() == signing_key) return true;
    }

    return false;
}

int32_t xrec_consortium_registration_contract::ins_refund(const std::string & account, uint64_t const & refund_amount) {
    data::system_contract::xrefund_info refund;
    get_refund(account, refund);

    refund.refund_amount += refund_amount;
    refund.create_time   = TIME();

    base::xstream_t stream(base::xcontext_t::instance());

    refund.serialize_to(stream);
    std::string stream_str = std::string((char *)stream.data(), stream.size());

    {
        XMETRICS_TIME_RECORD(XREG_CONTRACT "XPORPERTY_CONTRACT_REFUND_KEY_SetExecutionTime");
        MAP_SET(data::system_contract::XPORPERTY_CONTRACT_REFUND_KEY, account, stream_str);
    }

    return 0;
}

int32_t xrec_consortium_registration_contract::del_refund(const std::string & account) {
    {
        XMETRICS_TIME_RECORD(XREG_CONTRACT "XPORPERTY_CONTRACT_REFUND_KEY_RemoveExecutionTime");
        REMOVE(enum_type_t::map, data::system_contract::XPORPERTY_CONTRACT_REFUND_KEY, account);
    }

    return 0;
}

int32_t xrec_consortium_registration_contract::get_refund(const std::string & account, data::system_contract::xrefund_info & refund) {
    std::string value_str;
    {
        XMETRICS_TIME_RECORD(XREG_CONTRACT "XPORPERTY_CONTRACT_REFUND_KEY_GetExecutionTime");
        MAP_GET2(data::system_contract::XPORPERTY_CONTRACT_REFUND_KEY, account, value_str);
    }

    if (value_str.empty()) {
        xdbg("[xrec_consortium_registration_contract::get_refund] account(%s) not exist pid:%d\n", account.c_str(), getpid());
        return store::xaccount_property_not_exist;
    }

    base::xstream_t stream(base::xcontext_t::instance(), (uint8_t *)value_str.c_str(), (uint32_t)value_str.size());

    refund.serialize_from(stream);

    return 0;
}

void xrec_consortium_registration_contract::check_and_set_genesis_stage() {
    std::string value_str;
    data::system_contract::xactivation_record record;

    {
        XMETRICS_TIME_RECORD(XREG_CONTRACT "XPORPERTY_CONTRACT_GENESIS_STAGE_KEY_GetExecutionTime");
        value_str = STRING_GET(data::system_contract::XPORPERTY_CONTRACT_GENESIS_STAGE_KEY);
    }

    if (!value_str.empty()) {
        base::xstream_t stream(base::xcontext_t::instance(), (uint8_t *)value_str.c_str(), (uint32_t)value_str.size());
        record.serialize_from(stream);
    }
    if (record.activated) {
        xinfo("[xrec_consortium_registration_contract::check_and_set_genesis_stage] activated: %d, activation_time: %llu, pid:%d\n", record.activated, record.activation_time, getpid());
        return;
    }

    std::map<std::string, std::string> map_nodes;
    try {
        XMETRICS_TIME_RECORD(XREG_CONTRACT "XPORPERTY_CONTRACT_REG_KEY_CopyGetExecutionTime");
        MAP_COPY_GET(data::system_contract::XPORPERTY_CONTRACT_REG_KEY, map_nodes);
    } catch (std::runtime_error & e) {
        xdbg("[xrec_consortium_registration_contract::check_and_set_genesis_stage] MAP COPY GET error:%s", e.what());
    }

    bool active = data::system_contract::check_registered_nodes_active(map_nodes);
    if (active) {
        record.activated = 1;
        record.activation_time = TIME();
    }

    base::xstream_t stream(base::xcontext_t::instance());
    record.serialize_to(stream);
    value_str = std::string((char *)stream.data(), stream.size());
    {
        XMETRICS_TIME_RECORD(XREG_CONTRACT "XPORPERTY_CONTRACT_GENESIS_STAGE_KEY_SetExecutionTime");
        STRING_SET(data::system_contract::XPORPERTY_CONTRACT_GENESIS_STAGE_KEY, value_str);
    }
}

void xrec_consortium_registration_contract::updateNodeType(const std::string & node_types) {
    XMETRICS_COUNTER_INCREMENT(XREG_CONTRACT "updateNodeType_Called", 1);
    XMETRICS_TIME_RECORD(XREG_CONTRACT "updateNodeType_ExecutionTime");
    std::string const& account = SOURCE_ADDRESS();

    xdbg("[xrec_consortium_registration_contract::updateNodeType] pid: %d, balance: %lld, account: %s, node_types: %s", getpid(), GET_BALANCE(), account.c_str(), node_types.c_str());

    data::system_contract::xreg_node_info node_info;
    auto ret = get_node_info(account, node_info);
    XCONTRACT_ENSURE(ret == 0, "xrec_consortium_registration_contract::updateNodeType: node not exist");

    auto const miner_type = legal_archive_address_check(common::to_miner_type(node_types), common::xaccount_address_t{account});
    XCONTRACT_ENSURE(miner_type != common::xminer_type_t::invalid, "xrec_consortium_registration_contract::updateNodeType: invalid node_type!");
    XCONTRACT_ENSURE(miner_type != node_info.miner_type(), "xrec_consortium_registration_contract::updateNodeType: node_types can not be same!");

    const xtransaction_ptr_t trans_ptr = GET_TRANSACTION();
    XCONTRACT_ENSURE(!account.empty(), "xrec_consortium_registration_contract::updateNodeType: account must be not empty");

    if (trans_ptr->get_source_action_para().size() > 0) {
        xstream_t stream(xcontext_t::instance(), (uint8_t*)trans_ptr->get_source_action_para().data(), trans_ptr->get_source_action_para().size());

        data::xproperty_asset asset_out{0};
        stream >> asset_out.m_token_name;
        stream >> asset_out.m_amount;

        node_info.m_account_mortgage += asset_out.m_amount;
    }

    node_info.miner_type(miner_type);
    uint64_t min_deposit = node_info.get_required_min_deposit();
    xdbg(("[xrec_consortium_registration_contract::updateNodeType] min_deposit: %" PRIu64 ", account: %s, account morgage: %llu\n"),
        min_deposit, account.c_str(), node_info.m_account_mortgage);
    XCONTRACT_ENSURE(node_info.m_account_mortgage >= min_deposit, "xrec_consortium_registration_contract::updateNodeType: deposit not enough");

    init_node_credit(node_info);
    update_node_info(node_info);
    check_and_set_genesis_stage();

    XMETRICS_COUNTER_INCREMENT(XREG_CONTRACT "updateNodeType_Executed", 1);
}

void xrec_consortium_registration_contract::stakeDeposit() {
    XMETRICS_COUNTER_INCREMENT(XREG_CONTRACT "stakeDeposit_Called", 1);
    XMETRICS_TIME_RECORD(XREG_CONTRACT "stakeDeposit_ExecutionTime");
    std::string const& account = SOURCE_ADDRESS();
    data::system_contract::xreg_node_info node_info;
    auto ret = get_node_info(account, node_info);
    XCONTRACT_ENSURE(ret == 0, "xrec_consortium_registration_contract::stakeDeposit: node not exist");

    const xtransaction_ptr_t trans_ptr = GET_TRANSACTION();

    xstream_t stream(xcontext_t::instance(), (uint8_t*)trans_ptr->get_source_action_para().data(), trans_ptr->get_source_action_para().size());

    data::xproperty_asset asset_out{0};
    stream >> asset_out.m_token_name;
    stream >> asset_out.m_amount;

    xdbg("[xrec_consortium_registration_contract::stakeDeposit] pid: %d, balance: %lld, account: %s, stake deposit: %llu\n", getpid(), GET_BALANCE(), account.c_str(), asset_out.m_amount);
    XCONTRACT_ENSURE(asset_out.m_amount != 0, "xrec_consortium_registration_contract::stakeDeposit: stake deposit can not be zero");

    node_info.m_account_mortgage += asset_out.m_amount;
    update_node_info(node_info);
    XMETRICS_COUNTER_INCREMENT(XREG_CONTRACT "stakeDeposit_Executed", 1);
}

void xrec_consortium_registration_contract::unstakeDeposit(uint64_t unstake_deposit) {
    XMETRICS_COUNTER_INCREMENT(XREG_CONTRACT "unstakeDeposit_Called", 1);
    XMETRICS_TIME_RECORD(XREG_CONTRACT "unstakeDeposit_ExecutionTime");
    std::string const& account = SOURCE_ADDRESS();
    data::system_contract::xreg_node_info node_info;
    auto ret = get_node_info(account, node_info);
    XCONTRACT_ENSURE(ret == 0, "xrec_consortium_registration_contract::unstakeDeposit: node not exist");

    xdbg("[xrec_consortium_registration_contract::unstakeDeposit] pid: %d, balance: %lld, account: %s, unstake deposit: %llu, account morgage: %llu\n",
        getpid(), GET_BALANCE(), account.c_str(), unstake_deposit, node_info.m_account_mortgage);
    XCONTRACT_ENSURE(unstake_deposit != 0, "xrec_consortium_registration_contract::unstakeDeposit: unstake deposit can not be zero");

    uint64_t min_deposit = node_info.get_required_min_deposit();
    XCONTRACT_ENSURE(unstake_deposit <= node_info.m_account_mortgage && node_info.m_account_mortgage - unstake_deposit >= min_deposit, "xrec_consortium_registration_contract::unstakeDeposit: unstake deposit too big");
    TRANSFER(account, unstake_deposit);

    node_info.m_account_mortgage -= unstake_deposit;
    update_node_info(node_info);
    XMETRICS_COUNTER_INCREMENT(XREG_CONTRACT "unstakeDeposit_Executed", 1);
}

int32_t xrec_consortium_registration_contract::get_slash_info(std::string const & account, data::system_contract::xslash_info & node_slash_info) {
    std::string value_str;
    {
        XMETRICS_TIME_RECORD(XREG_CONTRACT "XPROPERTY_CONTRACT_SLASH_INFO_KEY_GetExecutionTime");
        MAP_GET2(data::system_contract::XPROPERTY_CONTRACT_SLASH_INFO_KEY, account, value_str);
    }
    if (value_str.empty()) {
        xdbg("[xrec_consortium_registration_contract][get_slash_info] account(%s) not exist,  pid:%d\n", account.c_str(), getpid());
        return store::xaccount_property_not_exist;
    }

    xstream_t stream(xcontext_t::instance(), (uint8_t *)value_str.data(), value_str.size());
    node_slash_info.serialize_from(stream);

    return 0;
}

bool xrec_consortium_registration_contract::is_valid_name(const std::string & nickname) const {
    int len = nickname.length();
    if (len < 4 || len > 16) {
        return false;
    }

    return std::find_if(nickname.begin(), nickname.end(), [](unsigned char c) { return !std::isalnum(c) && c != '_'; }) == nickname.end();
};

void xrec_consortium_registration_contract::init_node_credit(data::system_contract::xreg_node_info & node_info) {
    if (node_info.could_be_validator()) {
        if (node_info.m_validator_credit_numerator == 0 ) {
            node_info.m_validator_credit_numerator = XGET_ONCHAIN_GOVERNANCE_PARAMETER(initial_creditscore);
            xdbg("validator account %s credit score %" PRIu64, node_info.m_account.to_string().c_str(), node_info.m_validator_credit_numerator);
        }
    }
    if (node_info.could_be_auditor()) {
        if (node_info.m_auditor_credit_numerator == 0) {
            node_info.m_auditor_credit_numerator = XGET_ONCHAIN_GOVERNANCE_PARAMETER(initial_creditscore);
            xdbg("auditor account %s credit score %" PRIu64, node_info.m_account.to_string().c_str(), node_info.m_auditor_credit_numerator);
        }
    }

}

bool xrec_consortium_registration_contract::check_node_valid(std::string const &account_str)
{
    std::string reg_node_info_str;

    try {
        MAP_GET2(data::system_contract::XPROPERTY_NODE_INFO_MAP_KEY, account_str, reg_node_info_str, sys_contract_rec_node_manage_addr);
        if(reg_node_info_str.empty()) {
            xwarn("[xrec_consortium_registration_contract::check_node_valid] can't find account from node manage contract %s", account_str.c_str());
            return false;
        }
    } catch (top::error::xtop_error_t const&) {
        xdbg("[xrec_consortium_registration_contract::check_node_valid] can't find %s", account_str.c_str());
        return false; // not exist
    }

    data::system_contract::xnode_manage_account_info_t reg_account_info;
    base::xstream_t _stream(base::xcontext_t::instance(), (uint8_t*)reg_node_info_str.data(), reg_node_info_str.size());
    if (_stream.size() > 0) {
        reg_account_info.serialize_from(_stream);
    }

    std::string check_all, check_ca, check_expiry_time;
    try {
        MAP_GET2(data::system_contract::XPROPERTY_NODE_CHECK_OPTION_KEY, "check_all", check_all, sys_contract_rec_node_manage_addr);
        MAP_GET2(data::system_contract::XPROPERTY_NODE_CHECK_OPTION_KEY, "check_ca", check_ca, sys_contract_rec_node_manage_addr);
        MAP_GET2(data::system_contract::XPROPERTY_NODE_CHECK_OPTION_KEY, "check_expiry_time", check_expiry_time, sys_contract_rec_node_manage_addr);
    } catch (top::error::xtop_error_t const&) {
        xwarn("[xrec_consortium_registration_contract::check_node_valid] can't find XPROPERTY_NODE_CHECK_OPTION_KEY");
        return false;
    }

    if (check_all == "1") {
        uint64_t cur_time = TIME();
        if (check_ca == "1") {
            if (reg_account_info.cert_time < cur_time) {
                xwarn("[xrec_consortium_registration_contract::check_node_valid] account %s  time is expiry", account_str.c_str());
                return false;
            }
        }
        if (check_expiry_time == "1") {
            if (reg_account_info.expiry_time < cur_time) {
                xwarn("[xrec_consortium_registration_contract::check_node_valid] account %s cert time is expiry", account_str.c_str());
                return false;
            }
        }
    }
    return true;
}


NS_END3

#undef XREG_CONTRACT
#undef XCONTRACT_PREFIX
#undef XZEC_MODULE