cmake_minimum_required(VERSION 3.25)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED 23)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

project(shipBuilder)

find_package(OpenAL REQUIRED)

file(GLOB_RECURSE SOURCE_DIRS LIST_DIRECTORIES true "${CMAKE_SOURCE_DIR}/src/*")

set(SOURCES "")

foreach(ITEM ${SOURCE_DIRS})
    if(NOT IS_DIRECTORY ${ITEM})
        list(APPEND SOURCES ${ITEM})
    endif()
endforeach()

add_executable(${PROJECT_NAME} ${SOURCES})

add_subdirectory(vk-bootstrap)

target_link_libraries(${PROJECT_NAME} PRIVATE assimp vulkan SDL3 vk-bootstrap::vk-bootstrap glm OpenAL::OpenAL)