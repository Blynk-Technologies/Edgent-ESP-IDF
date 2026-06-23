option(EDGENT_DEV_MODE "Enable dev mode, build library from sources" OFF)
file(GLOB_RECURSE EDGENT_SOURCES src/*.c)

set(EDGENT_CORE_LIB edgent_core_${IDF_TARGET}_${IDF_VER})
set(PREBUILT_LIB_PATH ${COMPONENT_DIR}/lib/${EDGENT_CORE_LIB}.a)
set(EDGENT_CORE_REQUIRES
        idf::bootloader_support
        idf::esp_event
        idf::esp_netif
        idf::esp_wifi
        idf::esp_https_ota
        idf::esp_partition
        idf::efuse
        idf::nvs_flash
        idf::espressif__mqtt
        idf::espressif__cjson
        idf::espressif__ethernet_init
        idf::bt 
        idf::console 
        idf::esp_driver_uart
        idf::esp_driver_gpio
        idf::esp_eth
        idf::tcp_transport
        idf::espcoredump
        idf::http_parser
)

idf_component_register(
    SRCS ${EDGENT_SOURCES}
    INCLUDE_DIRS "include" "src"
)

if(NOT ${EDGENT_DEV_MODE} AND NOT EXISTS ${PREBUILT_LIB_PATH})
    message(FATAL_ERROR "Edgent: prebuilt binary is missing for selected target: ${PREBUILT_LIB_PATH}")
endif()

if(${EDGENT_DEV_MODE})
    message(STATUS "Edgent: building library from sources")
    add_subdirectory(src_lib)
else()
    message(STATUS "Edgent: using prebuilt ${PREBUILT_LIB_PATH}")
    add_prebuilt_library(${EDGENT_CORE_LIB} ${PREBUILT_LIB_PATH} REQUIRES ${EDGENT_CORE_REQUIRES})
endif()

target_link_libraries(${COMPONENT_LIB} PRIVATE ${EDGENT_CORE_LIB})