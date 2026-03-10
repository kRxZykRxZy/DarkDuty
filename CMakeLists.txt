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

if(WIN32)
    # =========================
    # Manual library paths for SDL2, SDL2_mixer, SDL2_ttf, GLEW
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
else()
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)
    pkg_check_modules(SDL2_MIXER REQUIRED IMPORTED_TARGET SDL2_mixer)
    pkg_check_modules(SDL2_TTF REQUIRED IMPORTED_TARGET SDL2_ttf)
    pkg_check_modules(GLEW REQUIRED IMPORTED_TARGET glew)
endif()

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
if(WIN32)
    target_include_directories(DarkDuty PRIVATE
        ${SDL2_INCLUDE_DIR}
        ${SDL2_MIXER_INCLUDE_DIR}
        ${SDL2_TTF_INCLUDE_DIR}
        ${GLEW_INCLUDE_DIR}
        ${CURL_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
    )
endif()

# =========================
# Link libraries
# =========================
target_link_libraries(DarkDuty PRIVATE
    ${CURL_LIBRARIES}
    OpenGL::GL
    Threads::Threads
)

if(WIN32)
    target_link_libraries(DarkDuty PRIVATE
        ${SDL2_LIBRARY}
        ${SDL2_MIXER_LIBRARY}
        ${SDL2_TTF_LIBRARY}
        ${GLEW_LIBRARIES}
        ws2_32
        wldap32
        crypt32
        normaliz
        bcrypt
        advapi32
    )
else()
    target_link_libraries(DarkDuty PRIVATE
        PkgConfig::SDL2
        PkgConfig::SDL2_MIXER
        PkgConfig::SDL2_TTF
        PkgConfig::GLEW
    )
endif()

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
if(WIN32)
    add_custom_command(TARGET DarkDuty POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_DLL} $<TARGET_FILE_DIR:DarkDuty>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_MIXER_DLL} $<TARGET_FILE_DIR:DarkDuty>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${SDL2_TTF_DLL} $<TARGET_FILE_DIR:DarkDuty>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${GLEW_DLL} $<TARGET_FILE_DIR:DarkDuty>
        # If you want to copy curl DLL from manual install, uncomment next line. If vcpkg handles DLLs, you may not need this.
        # COMMAND ${CMAKE_COMMAND} -E copy_if_different "C:/vcpkg/installed/x64-windows/bin/libcurl.dll" $<TARGET_FILE_DIR:DarkDuty>
    )
endif()

# In CMakeLists.txt, after add_executable
if(MSVC)
    set_target_properties(DarkDuty PROPERTIES
        LINK_FLAGS "/SUBSYSTEM:CONSOLE"
    )
endif()
