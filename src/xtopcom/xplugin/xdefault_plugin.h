// Copyright (c) 2018-2020 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once
#include "xbase/xbase.h"
#include "xbase/xns_macro.h"
#include "xplugin/xplugin.h"

#include <cstring>
#include <functional>
#include <memory>
#include <string>


NS_BEG2(top, data)
class xdefault_pligin : public xplugin {
public:
    xdefault_pligin() = default;
    xdefault_pligin(const std::string & plugin_name);
    xdefault_pligin(xdefault_pligin const &) = delete;
    xdefault_pligin & operator=(xdefault_pligin const &) = delete;
    xdefault_pligin(xdefault_pligin &&) = default;
    xdefault_pligin & operator=(xdefault_pligin &&) = default;
    ~xdefault_pligin() override = default;

    bool load() override;
    void run() override;
    void free() override;
    void callback(const std::string & tx_hash);
    bool async_send(const std::shared_ptr<top::xtxpool_v2::xtx_entry> & tx, const std::shared_ptr<top::xtxpool_v2::xtxpool_table_t> & table) override;
    bool send() override;
};

NS_END2