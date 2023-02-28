// Copyright (c) 2018-2020 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "xplugin/xdynamiclib.h"

using namespace std;

NS_BEG2(top, data)

bool xdynamiclib::load(const string & plugin_name) {
    std::string strName = "lib/Linux/lib" + plugin_name + ".so";
    if (nullptr != m_hlib) {
        dlclose(m_hlib);
    }
    m_hlib = dlopen(strName.c_str(), RTLD_NOW);
    if (m_hlib == nullptr) {
        xwarn("xdynamiclib::load plugin(%s) not found", strName.c_str());
        return false;
    }
    return true;
}

void xdynamiclib::free() {
    dlclose(m_hlib);
}

NS_END2