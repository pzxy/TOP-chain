# cmake_minimum_required(VERSION 3.8)
# add_compile_options(-static)

aux_source_directory(./src src_dir)
add_library(xplugin_impl ${src_dir})
if (NOT DISABLE_PLUGIN)
    target_link_libraries(xplugin_impl INTERFACE auditx xhttp)
    target_link_libraries(xplugin_impl PRIVATE xtxpool_v2 xstatestore)
else()
    target_link_libraries(xplugin_impl)
endif()

