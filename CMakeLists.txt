cmake_minimum_required(VERSION 3.16)
project(DarkDuty CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Source files
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(DarkDuty ${SOURCES})
target_include_directories(DarkDuty PRIVATE src)

# === Manual paths for Windows MSVC ===
set(SDL2_INCLUDE_DIR "C:/lib/sdl2/include")
set(SDL2_LIBRARY "C:/lib/sdl2/lib/SDL2.lib")
set(SDL2_DLL "C:/lib/sdl2/bin/SDL2.dll")

set(SDL2_MIXER_INCLUDE_DIR "C:/lib/sdl2_mixer/include")
set(SDL2_MIXER_LIBRARY "C:/lib/sdl2_mixer/lib/SDL2_mixer.lib")
set(SDL2_MIXER_DLL "C:/lib/sdl2_mixer/bin/SDL2_mixer.dll")

set(GLEW_INCLUDE_DIR "C:/lib/glew/include")
set(GLEW_LIBRARIES "C:/lib/glew/lib/Release/x64/glew32.lib")
set(GLEW_DLL "C:/lib/glew/bin/Release/x64/glew32.dll")

set(CURL_INCLUDE_DIR "C:/lib/curl/include")
set(CURL_LIBRARIES "C:/lib/curl/lib/libcurl.lib")
set(CURL_DLL "C:/lib/curl/bin/libcurl.dll")

# Threads
find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)

# Include directories
target_include_directories(DarkDuty PRIVATE
    ${SDL2_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${CURL_INCLUDE_DIR}
    ${OPENGL_INCLUDE_DIRS}
)

# Link libraries
target_link_libraries(DarkDuty PRIVATE
    ${SDL2_LIBRARY}
    ${SDL2_MIXER_LIBRARY}
    ${GLEW_LIBRARIES}
    ${CURL_LIBRARIES}
    ${OPENGL_LIBRARIES}
    Threads::Threads
)

# Windows-specific
if(WIN32)
    target_link_libraries(DarkDuty PRIVATE ws2_32)
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(DarkDuty PRIVATE /W3)
else()
    target_compile_options(DarkDuty PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()

# Optional: copy required DLLs to build folder after build
add_custom_command(TARGET DarkDuty POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_DLL} $<TARGET_FILE_DIR:DarkDuty>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_MIXER_DLL} $<TARGET_FILE_DIR:DarkDuty>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GLEW_DLL} $<TARGET_FILE_DIR:DarkDuty>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${CURL_DLL} $<TARGET_FILE_DIR:DarkDuty>
)