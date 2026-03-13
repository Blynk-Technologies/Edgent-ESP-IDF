file(GLOB_RECURSE EDGENT_SOURCES src/*.c)

set(EDGENT_CORE_LIB edgent_core_${IDF_TARGET}_${IDF_VER})
set(PREBUILT_LIB_PATH ${COMPONENT_DIR}/lib/${EDGENT_CORE_LIB}.a)
set(EDGENT_CORE_REQUIRES
        idf::esp_event
        idf::esp_netif
        idf::esp_wifi
        idf::esp_https_ota
        idf::esp_partition
        idf::efuse
        idf::nvs_flash
        idf::mqtt
        idf::json
        idf::bt 
        idf::console 
        idf::esp_driver_uart
        idf::esp_driver_gpio
        idf::esp_eth
)

idf_component_register(
    SRCS ${EDGENT_SOURCES}
    INCLUDE_DIRS "include" "src"
)

if(EXISTS ${PREBUILT_LIB_PATH})
    message(STATUS "Edgent: using prebuilt ${PREBUILT_LIB_PATH}")
    add_prebuilt_library(${EDGENT_CORE_LIB} ${PREBUILT_LIB_PATH} REQUIRES ${EDGENT_CORE_REQUIRES})
else()
    message(STATUS "Edgent: prebuilt not found, building from sources")
    add_subdirectory(src_lib)
endif()

target_link_libraries(${COMPONENT_LIB} PRIVATE ${EDGENT_CORE_LIB})