cmake_minimum_required(VERSION 3.5.0)
project(LearnOpenGL VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)

# 增加预定义宏
add_definitions(-DDEBUG)


file(GLOB ASSETS assets)
# 将资源拷贝到CMAKE工程编译目录
file(COPY ${ASSETS} DESTINATION ${CMAKE_BINARY_DIR})

set(GLFW_PATH dependency/GLFW)
link_directories(${GLFW_PATH}/lib-x86_64)

set(GLEW_PATH dependency/GLEW)
link_directories(${GLEW_PATH}/lib)

set(ASSIMP_PATH dependency/assimp)
link_directories(${ASSIMP_PATH}/lib)

file(GLOB vendor_head vendor)

file(GLOB imgui_src_list vendor/imgui/*.*)

include_directories(    
    ${GLFW_PATH}/include
    ${GLEW_PATH}/include
    ${ASSIMP_PATH}/include
    ${vendor_head})

find_library(OpenGL_LIBRARY OpenGL)
find_library(Cocoa_LIBRARY Cocoa)
find_library(IOKit_LIBRARY IOKit)

#将application文件纳入编译系统中
add_subdirectory(application)
add_subdirectory(glframework)

add_executable(LearnOpenGL 
               ${imgui_src_list} 
               GLHead.cpp 
               main.cpp)

target_link_libraries(LearnOpenGL 
    glfw3
    GLEW
    assimp
    zlibstatic
    app
    fw
    ${OpenGL_LIBRARY}
    ${Cocoa_LIBRARY}
    ${IOKit_LIBRARY})