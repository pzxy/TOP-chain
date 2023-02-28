// Copyright (c) 2018-2020 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "xplugin_impl/xaudit_plugin.h"

#include "xplugin_impl/xaudit_plugin_extern.h"

#include <iostream>
using namespace std;

NS_BEG2(top, data)

int32_t code(enum_audit_result result) {
    return static_cast<int32_t>(result);
}
xaudit_pligin * xaudit_pligin::instance_ = nullptr;

xaudit_pligin & xaudit_pligin::instance() {
    if (instance_ == nullptr) {
        instance_ = new xaudit_pligin();
    }
    return *instance_;
}

xaudit_pligin::xaudit_pligin() {
    instance_ = this;
}

xaudit_pligin::xaudit_pligin(const string & plugin_name) {
    set_plugin_name(plugin_name);
    instance_ = this;
}

bool xaudit_pligin::load() {
    if (nullptr != m_auditor) {
        return true;
    }
    m_auditor = std::make_shared<auditor_t *>(auditor_new());
    return nullptr != m_auditor.get();
}

void xaudit_pligin::run() {
    xdbg("xaudit_pligin::run audit-tx start auditor_ptr:%p", m_auditor.get());
    auditor_run(*m_auditor.get());
    xdbg("xaudit_pligin::run audit-tx success auditor_ptr:%p", m_auditor.get());
}

void xaudit_pligin::free() {
    auditor_free(*m_auditor.get());
}

void xaudit_pligin::str_free(const char * tx_hash) {
    str_free(tx_hash);
}

bool xaudit_pligin::send() {
    return false;
}

bool xaudit_pligin::async_send(const std::shared_ptr<top::xtxpool_v2::xtx_entry> & tx, const std::shared_ptr<top::xtxpool_v2::xtxpool_table_t> & table) {
    auto tx_ptr = tx->get_tx()->get_transaction();
    common::xaccount_address_t xaccount{tx_ptr->get_source_addr()};
    xdbg("xaudit_pligin::audit_data unaudit-tx-account-all:%s ", xaccount.to_string().c_str());
    auto ret = is_audit(xaccount);
    // If false ,return, Continue to execute the code after async_send()
    if (code(enum_audit_result::NotFound) == ret) {
        return false;
    }
    if (code(enum_audit_result::NullPoint) == ret) {
        xwarn("xaudit_pligin::do_work audit-tx is_audit() params nullpoint");
        return true;
    }
    if (code(enum_audit_result::Ok) != ret) {
        xwarn("xaudit_pligin::do_work audit-tx is_audit() fail,%d", ret);
        return true;
    }

    auto audit_data = tx_ptr->to_audit_str();
    auto tx_hash = tx_ptr->get_digest_hex_str();
    xdbg("xaudit_pligin::is_audit audit-tx-account:%s ", xaccount.to_string().c_str());
    auto f = [table, tx, tx_hash]() {
        xdbg("xaudit_pligin::audit_data audit-tx-callback start,tx_hash:%s ", tx_hash.c_str());
        if (table == nullptr) {
            xwarn("xaudit_pligin::audit_data audit-tx-callback fail, table is null");
            return;
        }
        if (tx == nullptr) {
            xwarn("xaudit_pligin::audit_data audit-tx-callback fail, tx is null:");
            return;
        }
        if (table->push_send_tx(tx) != xsuccess) {
            xwarn("xaudit_pligin::audit_data audit-tx-callback fail,tx_hash:%s ", tx_hash.c_str());
        } else {
            xinfo("xaudit_pligin::audit_data audit-tx-callback success,tx_hash:%s ", tx_hash.c_str());
        }
    };

    async_audit(tx_hash, audit_data, f);
    return true;
}

void xaudit_pligin::callback(const string & tx_hash, bool is_exec) {
    xdbg("xaudit_pligin::callback audit-tx tx_hash:%s ", tx_hash.c_str());
    auto it = callback_map.find(tx_hash);
    if (it == callback_map.end()) {
        xwarn("xaudit_pligin::callback can't find a function of tx_hash(%s)", tx_hash.c_str());
        return;
    }
    if (false == is_exec) {
        lock_guard<mutex> lock(map_mutex);
        callback_map.erase(tx_hash);
        return;
    }
    xdbg("xaudit_pligin::callback audit-tx tx_hash:%s ", tx_hash.c_str());
    auto f = *it->second.get();
    lock_guard<mutex> lock(map_mutex);
    callback_map.erase(tx_hash);
    f();
}

int32_t xaudit_pligin::is_audit(const common::xaccount_address_t & account) {
    return auditor_is_audit(*m_auditor.get(), account.to_string().c_str());
}

void xaudit_pligin::async_audit(string tx_hash, string data, std::function<void(void)> cb) {
    xdbg("xaudit_pligin::async_audit audit-tx-async tx_hash:%s ,data:%s", tx_hash.c_str(), data.c_str());
    auto ret = auditor_async_audit(*m_auditor.get(), data.c_str());
    if (code(enum_audit_result::AlreadyExisted) == ret) {
        xdbg("xaudit_pligin::async_audit audit-tx-async already existed tx_hash:%s", tx_hash.c_str());
        return;
    }
    if (code(enum_audit_result::Ok) == ret) {
        lock_guard<mutex> lock(map_mutex);
        callback_map[tx_hash] = make_shared<function<void(void)>>(cb);
        return;
    } else {
        xwarn("xaudit_pligin::async_audit audit-tx-async tx_hash:%s ,data:%s", tx_hash.c_str(), data.c_str());
    }
}

NS_END2