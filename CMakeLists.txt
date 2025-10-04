cmake_minimum_required(VERSION 3.15)
project(eclipse VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Library sources
set(ECLIPSE_SOURCES
    src/memory.cpp
    src/pattern.cpp
    src/module.cpp
)

# Library headers
set(ECLIPSE_HEADERS
    include/eclipse/eclipse.hpp
    include/eclipse/memory.hpp
    include/eclipse/pattern.hpp
    include/eclipse/module.hpp
    include/eclipse/types.hpp
)

# Create library
add_library(eclipse STATIC ${ECLIPSE_SOURCES} ${ECLIPSE_HEADERS})

# Include directories
target_include_directories(eclipse PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

# Compiler options
if(MSVC)
    target_compile_options(eclipse PRIVATE /W4)
else()
    target_compile_options(eclipse PRIVATE -Wall -Wextra)
endif()

# Example executable (optional, only if building standalone)
if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    add_executable(eclipse_example examples/basic_usage.cpp)
    target_link_libraries(eclipse_example PRIVATE eclipse)
endif()
