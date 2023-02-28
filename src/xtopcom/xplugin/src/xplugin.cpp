// Copyright (c) 2018-2020 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#include "xplugin/xplugin.h"

NS_BEG2(top, data)
void xplugin::set_plugin_name(std::string plugin_name) {
    m_plugin_name = plugin_name;
};
std::string xplugin::plugin_name() {
    return m_plugin_name;
}
xdynamiclib xplugin::dylib() {
    return m_dylib;
}

NS_END2