cmake_minimum_required(VERSION 3.16)
project(DarkDuty CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(DarkDuty ${SOURCES})
target_include_directories(DarkDuty PRIVATE src)

find_package(SDL2 REQUIRED)
find_package(SDL2_ttf QUIET)
find_package(SDL2_mixer QUIET)
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    if(NOT SDL2_ttf_FOUND)
        pkg_check_modules(SDL2_TTF REQUIRED SDL2_ttf)
    endif()
    if(NOT SDL2_mixer_FOUND)
        pkg_check_modules(SDL2_MIXER REQUIRED SDL2_mixer)
    endif()
endif()

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(CURL REQUIRED)
find_package(Threads REQUIRED)

target_include_directories(DarkDuty PRIVATE
    ${SDL2_INCLUDE_DIRS}
    ${SDL2_TTF_INCLUDE_DIRS}
    ${SDL2_MIXER_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
)

target_link_libraries(DarkDuty PRIVATE
    ${SDL2_LIBRARIES}
    ${SDL2_TTF_LIBRARIES}
    ${SDL2_MIXER_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    ${CURL_LIBRARIES}
    Threads::Threads
)

if(WIN32)
    target_link_libraries(DarkDuty PRIVATE ws2_32)
endif()

if(MSVC)
    target_compile_options(DarkDuty PRIVATE /W3)
else()
    target_compile_options(DarkDuty PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()
