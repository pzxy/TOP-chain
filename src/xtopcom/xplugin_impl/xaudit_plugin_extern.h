
// Copyright (c) 2018-2020 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.
#pragma once
#include "xbase/xbase.h"
#include "xplugin_impl/xaudit_plugin.h"

#include <iostream>

extern "C" void callback(const char * tx_hash, bool is_exec) {
    top::data::xaudit_pligin::instance().callback(tx_hash, is_exec);
};

extern "C" top::data::auditor_t * auditor_new(void);

extern "C" void auditor_run(top::data::auditor_t *);

extern "C" int32_t auditor_is_audit(top::data::auditor_t *, const char *);

extern "C" int32_t auditor_async_audit(top::data::auditor_t *, const char *);

extern "C" void auditor_free(top::data::auditor_t *);

extern "C" void str_free(const char *);
