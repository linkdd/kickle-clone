cmake_minimum_required(VERSION 3.22)
project(kickle-clone CXX)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(tinyxml2 REQUIRED)

find_library(trollworks_sdk_backend_sdl_LIBRARY trollworks-sdk-backend-sdl REQUIRED)
find_library(trollworks_sdk_imgui_sdl_LIBRARY trollworks-sdk-imgui-sdl REQUIRED)
find_library(physfs_LIBRARY physfs REQUIRED)
find_library(physfsrwops_LIBRARY physfsrwops REQUIRED)

file(GLOB_RECURSE GAME_SOURCES
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE GAME_HEADERS
  ${PROJECT_SOURCE_DIR}/include/*.hpp
)

file(GLOB_RECURSE GAME_ASSET_FILES
  ${PROJECT_SOURCE_DIR}/assets/*.png
  ${PROJECT_SOURCE_DIR}/assets/*.json
  ${PROJECT_SOURCE_DIR}/assets/sprites/*.png
  ${PROJECT_SOURCE_DIR}/assets/sprites/*.json
  ${PROJECT_SOURCE_DIR}/assets/tilemaps/*.tsx
  ${PROJECT_SOURCE_DIR}/assets/tilemaps/*.tmx
)

add_executable(${PROJECT_NAME}
  ${GAME_SOURCES}
  ${GAME_HEADERS}
)

add_custom_command(
  OUTPUT "${CMAKE_BINARY_DIR}/data.zip"
  COMMAND ${CMAKE_COMMAND} -E tar "cfv" "${CMAKE_BINARY_DIR}/data.zip" --format=zip -- ${GAME_ASSET_FILES}
  WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}/assets
  DEPENDS ${GAME_ASSET_FILES}
  COMMENT "Creating data.zip archive"
)

add_custom_target(
  ${PROJECT_NAME}-assets ALL
  DEPENDS "${CMAKE_BINARY_DIR}/data.zip"
)

add_dependencies(${PROJECT_NAME} ${PROJECT_NAME}-assets)

if(WIN32)
  target_link_libraries(${PROJECT_NAME}
    mingw32
    SDL2::SDL2-static
    ${trollworks_sdk_backend_sdl_LIBRARY}
    SDL2_image::SDL2_image-static
    ${trollworks_sdk_imgui_sdl_LIBRARY}
    ${physfs_LIBRARY}
    ${physfsrwops_LIBRARY}
    tinyxml2::tinyxml2
    opengl32
    glu32
  )

  target_link_options(${PROJECT_NAME} PRIVATE
    -static-libstdc++
  )
elseif(APPLE)
  target_link_libraries(${PROJECT_NAME}
    SDL2::SDL2-static
    ${trollworks_sdk_backend_sdl_LIBRARY}
    SDL2_image::SDL2_image-static
    ${trollworks_sdk_imgui_sdl_LIBRARY}
    ${physfs_LIBRARY}
    ${physfsrwops_LIBRARY}
    tinyxml2::tinyxml2
    GL
  )
elseif(UNIX)
  target_link_libraries(${PROJECT_NAME}
    SDL2::SDL2-static
    ${trollworks_sdk_backend_sdl_LIBRARY}
    SDL2_image::SDL2_image-static
    ${trollworks_sdk_imgui_sdl_LIBRARY}
    ${physfs_LIBRARY}
    ${physfsrwops_LIBRARY}
    tinyxml2::tinyxml2
    GL
  )
else()
  message(FATAL_ERROR "Platform not supported")
endif()

target_include_directories(${PROJECT_NAME}
  PRIVATE ${PROJECT_SOURCE_DIR}/include
  PRIVATE ${CMAKE_INSTALL_PREFIX}/include
  PRIVATE ${CMAKE_INSTALL_PREFIX}/include/SDL2
)

target_compile_options(${PROJECT_NAME} PRIVATE
  -O2 -Wall -Werror -Wextra -std=c++23
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)
install(FILES "${CMAKE_BINARY_DIR}/data.zip" DESTINATION bin)
