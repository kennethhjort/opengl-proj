cmake_minimum_required(VERSION 3.8)
project(ecs)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/build/bin)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

file(GLOB_RECURSE sources
    "${CMAKE_SOURCE_DIR}/src/*.cpp"
    "extern/glad/src/glad.c"
)

include_directories(ecs PRIVATE 
    "${CMAKE_SOURCE_DIR}/include"
)

include_directories( SYSTEM
    "extern/glad/include"
    "extern/glfw/include"
)

# Assimp
include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${PROJECT_SOURCE_DIR}/libs CACHE PATH "Missing description." FORCE)
FetchContent_Declare(assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG master
)
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)

set(CMAKE_BUILD_TYPE_TEMP ${CMAKE_BUILD_TYPE})
set(CMAKE_BUILD_TYPE Release)

FetchContent_MakeAvailable(assimp)

set(CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE_TEMP})

add_subdirectory(extern/glfw)

add_executable(ecs ${sources})

target_include_directories(ecs PRIVATE libs/assimp-src/include)
target_link_libraries(ecs assimp)

target_link_libraries(ecs glfw ${GLFW_LIBRARIES})

set_target_properties(ecs PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

set(CMAKE_BUILD_TYPE Debug)
