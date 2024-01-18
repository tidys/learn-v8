set(app "learn-v8")
cmake_minimum_required(VERSION 3.5.0)
set(v8_dir ${CMAKE_CURRENT_LIST_DIR}/libs/include/v8/)
project(${app} VERSION 1.0.0)
file(GLOB v8_headers
    ${CMAKE_CURRENT_LIST_DIR}/libs/include/v8/*.h
    ${CMAKE_CURRENT_LIST_DIR}/libs/include/v8/libplatform/*.h
)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR}/libs FILES ${v8_headers})
set(app_src main.cpp main.js cpp.h jsb.h)
source_group(TREE ${CMAKE_CURRENT_LIST_DIR} FILES ${app_src})
add_executable(${app} ${app_src} ${v8_headers})

add_custom_command(TARGET ${app} PRE_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/libs/lib $(outdir))
include_directories(${v8_dir} ${CMAKE_CURRENT_LIST_DIR})

set(v8_libs v8 v8_libbase v8_libplatform)

foreach(lib ${v8_libs})
    message(${lib})

    add_library(${lib} SHARED IMPORTED GLOBAL)
    set_target_properties(${lib} PROPERTIES
        IMPORTED_IMPLIB ${CMAKE_CURRENT_LIST_DIR}/libs/lib/${lib}.dll.lib
        IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/libs/lib/${lib}.dll
    )

    target_link_libraries(${app} PUBLIC ${lib})
endforeach()

set_target_properties(${app} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}")
