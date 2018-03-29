set(target ${DATA_STRUCTURES_WRAPPER_LIBRARY})

include_directories(${DATA_STRUCTURES_WRAPPER_INCLUDE_DIR})

set(hdrs "${DATA_STRUCTURES_WRAPPER_INCLUDE_DIR}/module_info.h"
    "${DATA_STRUCTURES_WRAPPER_INCLUDE_DIR}/environment_variable.h")
set(srcs module_info.cpp
    environment_variable.cpp)

add_library(${target} STATIC ${srcs} ${hdrs})

# copy static library to the binary directory
add_custom_command(TARGET ${DATA_STRUCTURES_WRAPPER_LIBRARY} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_CURRENT_BINARY_DIR}/\$\(Configuration\)"
        "${CMAKE_CURRENT_BINARY_DIR}/../../bin")
