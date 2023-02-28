// Copyright (c) 2018-2020 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once
#include "xbase/xbase.h"
#include "xbase/xns_macro.h"
#include "xcommon/xaccount_address_fwd.h"
#include "xplugin/xplugin.h"
#include "xtxpool_v2/xtxpool_face.h"
#include "xtxpool_v2/xtxpool_table.h"

#include <cstring>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>

NS_BEG2(top, data)
using auditor_t = struct auditor;
enum class enum_audit_result {
    SystemError = -1,
    Ok = 0,
    NotFound = 1,
    InvalidParam = 2,
    NullPoint = 3,
    ParseJson = 4,
    SerializeJson = 5,
    LockMutex = 6,
    HttpServer = 7,
    AlreadyExisted = 8,
};

int32_t code(enum_audit_result result);
class xaudit_pligin : public xplugin {
public:
    xaudit_pligin(xaudit_pligin const &) = default;
    xaudit_pligin & operator=(xaudit_pligin const &) = default;
    xaudit_pligin(xaudit_pligin &&) = default;
    xaudit_pligin & operator=(xaudit_pligin &&) = default;
    ~xaudit_pligin() override = default;
    static xaudit_pligin & instance();

    bool load() override;
    void run() override;
    void free() override;

    bool send() override ;
    bool async_send(const std::shared_ptr<top::xtxpool_v2::xtx_entry> & tx,const std::shared_ptr<top::xtxpool_v2::xtxpool_table_t> & table) override;

public:
    void callback(const std::string & tx_hash, bool is_exec);
    int32_t is_audit(const common::xaccount_address_t & account);
    void async_audit(std::string tx_hash, std::string data, std::function<void(void)> callback);

private:
    xaudit_pligin();
    xaudit_pligin(const std::string & plugin_name);
    static xaudit_pligin * instance_;
    std::shared_ptr<auditor_t *> m_auditor;
    void str_free(const char *);
    mutable std::mutex map_mutex;
    std::unordered_map<std::string, std::shared_ptr<std::function<void(void)>>> callback_map;
};


NS_END2