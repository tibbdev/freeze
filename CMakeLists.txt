cmake_minimum_required(VERSION 3.14)
# set(CMAKE_POLICY_VERSION_MINIMUM 3.5)

project(freeze LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# --- 1. Global Build Settings (Forces everything to be Static) ---
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

# SDL3 Settings
#set(SDL_STATIC ON CACHE BOOL "" FORCE)
#set(SDL_SHARED OFF CACHE BOOL "" FORCE)
#set(SDL_TEST OFF CACHE BOOL "" FORCE)

# SDL3_mixer Settings - Force Static and disable dependencies
#set(SDL3MIXER_VENDORED ON CACHE BOOL "" FORCE)
#set(SDL3MIXER_DEPS_SHARED OFF CACHE BOOL "" FORCE)
#set(SDL3MIXER_OPUS OFF CACHE BOOL "" FORCE)
#set(SDL3MIXER_FLAC OFF CACHE BOOL "" FORCE)
#set(SDL3MIXER_MIDI ON CACHE BOOL "" FORCE)
#set(SDL3MIXER_MOD OFF CACHE BOOL "" FORCE)

# SDL_ttf Settings
#set(SDL3TTF_VENDORED ON CACHE BOOL "" FORCE)
#set(SDL3TTF_SAMPLES OFF CACHE BOOL "" FORCE)
#set(SDL3TTF_HARFBUZZ OFF CACHE BOOL "" FORCE)

# PhysFS Settings
set(PHYSFS_BUILD_TEST OFF CACHE BOOL "" FORCE)
set(PHYSFS_BUILD_SHARED OFF CACHE BOOL "" FORCE)
set(PHYSFS_BUILD_STATIC ON CACHE BOOL "" FORCE)

# --- 2. Fetching ---
FetchContent_Declare(SDL3 
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git 
    GIT_TAG release-3.4.2)

FetchContent_Declare(SDL3_mixer 
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_mixer.git 
    GIT_TAG main) 

FetchContent_Declare(SDL_ttf 
    GIT_REPOSITORY https://github.com/libsdl-org/SDL_ttf.git 
    GIT_TAG main)

FetchContent_Declare(glm 
    GIT_REPOSITORY https://github.com/g-truc/glm.git 
    GIT_TAG 1.0.1)

FetchContent_Declare(physfs 
    GIT_REPOSITORY https://github.com/icculus/physfs.git 
    GIT_TAG release-3.2.0)

FetchContent_MakeAvailable(SDL3 SDL3_mixer SDL_ttf glm physfs)

FetchContent_MakeAvailable(SDL3 SDL3_mixer SDL_ttf glm physfs)

find_package(OpenGL REQUIRED)

# --- 3. Source Management ---
file(GLOB_RECURSE FREEZE_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/freeze/*.cpp")
file(GLOB_RECURSE PLATFORM_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/platform/*.cpp")
# file(GLOB_RECURSE GRAPHICS_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/graphics/*.cpp")

# if (NOT CMAKE_BUILD_TYPE)
#     set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "" FORCE)
# endif()
# message("Build Type : ${CMAKE_BUILD_TYPE}")

add_executable(${PROJECT_NAME}
    ${FREEZE_SOURCES}
    ${PLATFORM_SOURCES} 
#    ${GRAPHICS_SOURCES}
)

# --- 4. Compiler Settings ---
if(MSVC)
    add_compile_options(/arch:AVX2)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

# --- 5. Include Directories ---
target_include_directories(${PROJECT_NAME} PRIVATE 
    ${sdl3_SOURCE_DIR}/include
    ${sdl3_mixer_SOURCE_DIR}/include
    ${sdl_ttf_SOURCE_DIR}/include
    ${glm_SOURCE_DIR}
    ${physfs_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/freeze
    ${CMAKE_CURRENT_SOURCE_DIR}/platform
)

# --- 6. Linking ---
target_link_libraries(${PROJECT_NAME} PRIVATE
    SDL3::SDL3-static
#    SDL3_main-static
    SDL3_mixer::SDL3_mixer
    SDL3_ttf::SDL3_ttf
    physfs-static
#    OpenGL::GL
)

# --- 7. Resource Management & Binary Packing ---
if(WIN32)
    set(APPEND_CMD cmd /c "copy /b $<TARGET_FILE_BASE_NAME:${PROJECT_NAME}>_raw.exe + data.zip $<TARGET_FILE_NAME:${PROJECT_NAME}>")
else()
    set(APPEND_CMD sh -c "cat $<TARGET_FILE_BASE_NAME:${PROJECT_NAME}>_raw.exe data.zip > $<TARGET_FILE_NAME:${PROJECT_NAME}> && chmod +x $<TARGET_FILE_NAME:${PROJECT_NAME}>")
endif()

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    # Sync assets folder
    COMMAND ${CMAKE_COMMAND} -E copy_directory 
            "${CMAKE_CURRENT_SOURCE_DIR}/assets" 
            "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets"
    
    # Zip the assets
    COMMAND ${CMAKE_COMMAND} -E tar cf data.zip --format=zip "assets"
    
    # Create the 'raw' backup
    COMMAND ${CMAKE_COMMAND} -E copy "$<TARGET_FILE:${PROJECT_NAME}>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/$<TARGET_FILE_BASE_NAME:${PROJECT_NAME}>_raw.exe"

    # Execute the OS-specific append command
    COMMAND ${APPEND_CMD}
    
    WORKING_DIRECTORY "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
    COMMENT "[POST_BUILD] Appending assets to binary..."
    VERBATIM
)
