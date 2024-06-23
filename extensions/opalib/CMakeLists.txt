set(target_name opalib)

#FILE(GLOB_RECURSE OPALIB_SOURCES *.h;*.cpp)

set (OPALIB_SOURCES
    src/opalib/Opalib.h
    src/opalib/Opalib.cpp
    src/opalib/Application/OPApplication.hpp
    src/opalib/Application/OPApplication.cpp
    src/opalib/Debug/OPDebug.hpp
    src/opalib/Debug/OPDebug.cpp
)

add_library(${target_name} STATIC ${OPALIB_SOURCES})

target_include_directories(
    ${target_name}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_include_directories(
    ${target_name}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/opalib
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src
    INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/src/opalib
)

setup_ax_extension_config(${target_name})
