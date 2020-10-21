project(openclUtils)
cmake_minimum_required(VERSION 2.8.11)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c++14  -Wall -O3")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wall -O3")

if(APPLE)
    add_definitions(-DMAC_OS)
else()
    if(ANDROID)
        add_definitions(-DANDROID)
    endif()
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
find_package(PkgConfig REQUIRED)

find_package(Eigen REQUIRED)
list(APPEND USER_INC  ${EIGEN_INCLUDE_DIR})
message("-- Found eigen in ${EIGEN_INCLUDE_DIR}")

find_package(OpenGL REQUIRED)
message("-- Found opengl in ${OPENGL_INCLUDE_DIR}")
list(APPEND USER_INC  ${OPENGL_INCLUDE_DIR})
list(APPEND LINK_LIBS ${OPENGL_LIBRARIES})

pkg_search_module(GLEW REQUIRED glew)
message("-- Found glew in ${GLEW_INCLUDE_DIRS}")
list(APPEND USER_INC  ${GLEW_INCLUDE_DIRS})
list(APPEND LINK_LIBS ${GLEW_LIBRARIES})

find_package(OpenCV REQUIRED)
list(APPEND USER_INC  ${OpenCV_INCLUDE_DIRS})

include_directories(${USER_INC})
link_directories(${OpenCV_LIBS_DIRS} ${GLEW_LIBRARY_DIRS})


list(APPEND OPENGL_UTILS_SRC
        src/file_utils.cpp
        src/glsl.cpp
        src/glBufferData.cpp
        src/glFrameBuffer.cpp
        src/glRenderBuffer.cpp
        src/glTexture.cpp
        src/opengl_render_state.cpp
        src/shader.cpp
        src/uniform.cpp
        src/transformFeedback.cpp
        )

add_library(fgl_utils SHARED
        ${OPENGL_UTILS_SRC}
        )

target_link_libraries(fgl_utils ${LINK_LIBS} opencv_core opencv_highgui)