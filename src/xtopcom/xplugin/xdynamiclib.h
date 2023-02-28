// Copyright (c) 2018-2020 Telos Foundation & contributors
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#pragma once

#include "xbase/xbase.h"
#include "xbase/xns_macro.h"

#include <dirent.h>
#include <dlfcn.h>

#include <cstring>
#include <iostream>
#include <string>
NS_BEG2(top, data)
class xdynamiclib {
public:
    xdynamiclib() = default;
    ~xdynamiclib() = default;

    bool load(const std::string & plugin_name);
    void free();
    // # examples of calling method ‘extern "C" foo* new()’
    // typedef struct foo foo_t;
    // typedef foo_t * (*f_new_t)(void);
    // f_new_t f_new = xdynamiclib.function<f_new_t>("new");
    // if (nullptr != f_new) {
    //     (f_new)();
    // }
    template <typename Func>
    Func function(const std::string & funcname) {
        void * func_ptr = dlsym(m_hlib, funcname.c_str());
        if (nullptr == func_ptr) {
            xwarn("xdynamiclib::function failed to get function(%s) from dynamic library, error:%s", funcname.c_str(), dlerror());
            return nullptr;
        }
        return reinterpret_cast<Func>(func_ptr);
    }

private:
    void * m_hlib;
};
NS_END2