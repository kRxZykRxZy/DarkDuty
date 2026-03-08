cmake_minimum_required(VERSION 3.16)
project(DarkDuty CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# =========================
# Source files
# =========================
file(GLOB_RECURSE SOURCES "src/*.cpp")
add_executable(DarkDuty ${SOURCES})

target_include_directories(DarkDuty PRIVATE src)

# =========================
# Manual library paths for SDL2, SDL2_mixer, SDL2_ttf, GLEW
# (keep your manual setup for these since you're managing them manually)
# =========================
set(SDL2_INCLUDE_DIR "C:/lib/sdl2/include")
set(SDL2_LIBRARY "C:/lib/sdl2/lib/x64/SDL2.lib")
set(SDL2_DLL "C:/lib/sdl2/bin/x64/SDL2.dll")

set(SDL2_MIXER_INCLUDE_DIR "C:/lib/sdl2_mixer/include")
set(SDL2_MIXER_LIBRARY "C:/lib/sdl2_mixer/lib/x64/SDL2_mixer.lib")
set(SDL2_MIXER_DLL "C:/lib/sdl2_mixer/bin/x64/SDL2_mixer.dll")

set(SDL2_TTF_INCLUDE_DIR "C:/lib/sdl2_ttf/include")
set(SDL2_TTF_LIBRARY "C:/lib/sdl2_ttf/lib/x64/SDL2_ttf.lib")
set(SDL2_TTF_DLL "C:/lib/sdl2_ttf/bin/x64/SDL2_ttf.dll")

set(GLEW_INCLUDE_DIR "C:/lib/glew/include")
set(GLEW_LIBRARIES "C:/lib/glew/lib/Release/x64/glew32.lib")
set(GLEW_DLL "C:/lib/glew/bin/Release/x64/glew32.dll")

# =========================
# Find packages via vcpkg toolchain
# =========================
# CURL, Threads, and OpenGL will be found via find_package with vcpkg support
find_package(CURL REQUIRED)
find_package(Threads REQUIRED)
find_package(OpenGL REQUIRED)

# =========================
# Include directories
# =========================
target_include_directories(DarkDuty PRIVATE
    ${SDL2_INCLUDE_DIR}
    ${SDL2_MIXER_INCLUDE_DIR}
    ${SDL2_TTF_INCLUDE_DIR}
    ${GLEW_INCLUDE_DIR}
    ${CURL_INCLUDE_DIRS}
    ${OPENGL_INCLUDE_DIRS}
)

# =========================
# Link libraries
# =========================
target_link_libraries(DarkDuty PRIVATE
    ${SDL2_LIBRARY}
    ${SDL2_MIXER_LIBRARY}
    ${SDL2_TTF_LIBRARY}
    ${GLEW_LIBRARIES}
    ${CURL_LIBRARIES}
    ${OPENGL_LIBRARIES}
    Threads::Threads

    # Windows dependencies
    ws2_32
    wldap32
    crypt32
    normaliz
    bcrypt
    advapi32
)

# =========================
# Compiler warnings
# =========================
if(MSVC)
    target_compile_options(DarkDuty PRIVATE /W3)
else()
    target_compile_options(DarkDuty PRIVATE -Wall -Wextra -Wno-unused-parameter)
endif()

# =========================
# Copy DLLs after build
# =========================
add_custom_command(TARGET DarkDuty POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_DLL} $<TARGET_FILE_DIR:DarkDuty>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_MIXER_DLL} $<TARGET_FILE_DIR:DarkDuty>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_TTF_DLL} $<TARGET_FILE_DIR:DarkDuty>
    COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GLEW_DLL} $<TARGET_FILE_DIR:DarkDuty>
    # If you want to copy curl DLL from manual install, uncomment next line. If vcpkg handles DLLs, you may not need this.
    # COMMAND ${CMAKE_COMMAND} -E copy_if_different "C:/vcpkg/installed/x64-windows/bin/libcurl.dll" $<TARGET_FILE_DIR:DarkDuty>
)
