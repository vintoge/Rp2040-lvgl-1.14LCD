message("Fetching lvgl v8.3.8 from github...")

set(LV_CONF_PATH ${CMAKE_SOURCE_DIR}/ToyMain/include/ToyMain/lv_conf.h CACHE STRING "" FORCE) 
# set(LV_LVGL_H_INCLUDE_SIMPLE OFF)
# set(LV_CONF_INCLUDE_SIMPLE OFF)

include(FetchContent)
FetchContent_Declare(lvgl            
        GIT_REPOSITORY  https://github.com/lvgl/lvgl.git
        GIT_TAG         v8.3.8
        SOURCE_DIR      ${CMAKE_SOURCE_DIR}/lib/lvgl
)
FetchContent_MakeAvailable(lvgl)



