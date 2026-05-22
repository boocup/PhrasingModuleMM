cmake_minimum_required(VERSION 3.22)

# ====================== SDK Path ======================
if(NOT "${METAMODULE_SDK_DIR}" STREQUAL "")
    message(STATUS "METAMODULE_SDK_DIR set by CMake variable: ${METAMODULE_SDK_DIR}")
elseif(DEFINED ENV{METAMODULE_SDK_DIR})
    set(METAMODULE_SDK_DIR "$ENV{METAMODULE_SDK_DIR}")
    message(STATUS "METAMODULE_SDK_DIR set by environment: ${METAMODULE_SDK_DIR}")
else()
    set(METAMODULE_SDK_DIR "${CMAKE_CURRENT_LIST_DIR}/../metamodule-plugin-sdk")
    message(STATUS "METAMODULE_SDK_DIR set to default: ${METAMODULE_SDK_DIR}")
endif()

include(${METAMODULE_SDK_DIR}/plugin.cmake)

# ====================== Project ======================
project(PhrasingModuleMM 
    VERSION 0.1 
    DESCRIPTION "Phrasing Module for MetaModule" 
    LANGUAGES C CXX
)

add_library(PhrasingModuleMM STATIC)

target_sources(PhrasingModuleMM PRIVATE
    src/Phrasing.cpp
    src/plugin.cpp
)

target_include_directories(PhrasingModuleMM PRIVATE src)

# ====================== Create Plugin ======================
create_plugin(
    SOURCE_LIB      PhrasingModuleMM
    PLUGIN_NAME     PhrasingModuleMM
    PLUGIN_JSON     ${CMAKE_CURRENT_LIST_DIR}/plugin.json
    SOURCE_ASSETS   ${CMAKE_CURRENT_LIST_DIR}/assets
    DESTINATION     ${CMAKE_CURRENT_LIST_DIR}/metamodule-plugins
)