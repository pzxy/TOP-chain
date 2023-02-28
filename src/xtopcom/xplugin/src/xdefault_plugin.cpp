// Copyright (c) 2018-2020 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "xplugin/xdefault_plugin.h"

NS_BEG2(top, data)

xdefault_pligin::xdefault_pligin(const std::string & plugin_name) {
    set_plugin_name(plugin_name);
}

bool xdefault_pligin::load() {
    return true;
}
void xdefault_pligin::run() {
    return;
}
void xdefault_pligin::free() {
    return;
}
void xdefault_pligin::callback(const std::string & tx_hash) {
    return;
}

bool async_send(const std::shared_ptr<top::xtxpool_v2::xtx_entry> & tx, const std::shared_ptr<top::xtxpool_v2::xtxpool_table_t> & table) {
    return false;
}

bool send() {
    return false;
}

NS_END2