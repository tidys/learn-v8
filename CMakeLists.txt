set(app "learn-v8")
cmake_minimum_required(VERSION 3.5.0)
project(${app} VERSION 1.0.0)
add_executable(${app} main.cpp)

include_directories(${CMAKE_CURRENT_LIST_DIR}/libs/v8/)

set(v8_libs v8 v8_libbase v8_libplatform)

foreach(lib ${v8_libs})
    message(${lib})

    add_library(${lib} SHARED IMPORTED GLOBAL)
    set_target_properties(${lib} PROPERTIES
        IMPORTED_IMPLIB ${CMAKE_CURRENT_LIST_DIR}/libs/${lib}.dll.lib
        IMPORTED_LOCATION ${CMAKE_CURRENT_LIST_DIR}/libs/${lib}.dll
    )

    target_link_libraries(${app} PUBLIC ${lib})
endforeach()
