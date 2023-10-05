cmake_minimum_required(VERSION 3.10)

project(
  EnTT-skpacman
  VERSION 0.1.0
  LANGUAGES CXX
)

set(CMAKE_EXE_LINKER_FLAGS "-Wl,-subsystem,console")

if(MSVC)
  # 设置UTF-8编码选项
  add_compile_options(/utf-8)
endif()

include(cmake/utility.cmake)
include(cmake/FindSDL2.cmake)
include(cmake/FindSDL2_ttf.cmake)
include(cmake/copydll.cmake)
include(cmake/copyresources.cmake)

if(WIN32)
  include(cmake/fetch_sdl.cmake)
  option(FETCH_SDL "fetch SDL2 SDL2_image SDL2_ttf from web" FALSE)

  if(FETCH_SDL)
    FetchSDL()
  endif()
endif()

set(GAME_NAME Entt_skpacman)
file(GLOB_RECURSE HEADER "src/*.hpp")
file(GLOB_RECURSE SRC "src/*.cpp")
add_executable(${GAME_NAME} ${SRC} ${HEADER})

target_include_directories(${GAME_NAME} PRIVATE src)
target_link_libraries(${GAME_NAME} PRIVATE SDL2)
target_compile_features(${GAME_NAME} PRIVATE cxx_std_17)
target_precompile_headers(${GAME_NAME} PRIVATE src/pch.hpp)

CopyResources(${GAME_NAME})
CopyDLL(${GAME_NAME})

add_subdirectory(3rdlibs)

target_link_libraries(${GAME_NAME} PRIVATE EnTT)