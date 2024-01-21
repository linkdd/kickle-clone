cmake_minimum_required(VERSION 3.22)
project(kickle-clone CXX)

add_subdirectory(vendor/trollworks-sdk-backend-sdl)
add_subdirectory(vendor/physfs)
add_subdirectory(vendor/physfsrwops)
add_subdirectory(vendor/tinyxml2)

file(GLOB_RECURSE GAME_SOURCES
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE GAME_HEADERS
  ${PROJECT_SOURCE_DIR}/include/*.hpp
)

set(GAME_ASSET_FILES
  "${PROJECT_SOURCE_DIR}/assets/spritesheet.png"
  "${PROJECT_SOURCE_DIR}/assets/spritesheet.json"
  "${PROJECT_SOURCE_DIR}/assets/tilemaps/spritesheet.tsx"
  "${PROJECT_SOURCE_DIR}/assets/tilemaps/level1.tmx"
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
    SDL2main
    SDL2-static
    trollworks-sdk-backend-sdl
    physfs-static
    physfsrwops
    tinyxml2
    opengl32
    glu32
  )
elseif(APPLE)
  target_link_libraries(${PROJECT_NAME}
    SDL2-static
    trollworks-sdk-backend-sdl
    physfs-static
    physfsrwops
    tinyxml2
    opengl
    glu
  )
elseif(UNIX)
  target_link_libraries(${PROJECT_NAME}
    SDL2-static
    trollworks-sdk-backend-sdl
    physfs-static
    physfsrwops
    tinyxml2
    opengl
    glu
  )
else()
  message(FATAL_ERROR "Platform not supported")
endif()

target_include_directories(${PROJECT_NAME}
  PRIVATE ${PROJECT_SOURCE_DIR}/include
  PRIVATE ${PROJECT_SOURCE_DIR}/vendor/trollworks-sdk-core/include
  PRIVATE "$<TARGET_PROPERTY:trollworks-sdk-backend-sdl,INTERFACE_INCLUDE_DIRECTORIES>"
  PRIVATE "$<TARGET_PROPERTY:SDL2-static,INTERFACE_INCLUDE_DIRECTORIES>"
  PRIVATE "$<TARGET_PROPERTY:SDL2_image,INTERFACE_INCLUDE_DIRECTORIES>"
  PRIVATE "$<TARGET_PROPERTY:nlohmann_json::nlohmann_json,INTERFACE_INCLUDE_DIRECTORIES>"
  PRIVATE "$<TARGET_PROPERTY:physfs-static,INTERFACE_INCLUDE_DIRECTORIES>"
  PRIVATE "$<TARGET_PROPERTY:physfsrwops,INTERFACE_INCLUDE_DIRECTORIES>"
  PRIVATE "$<TARGET_PROPERTY:tinyxml2,INTERFACE_INCLUDE_DIRECTORIES>"
)

target_compile_options(${PROJECT_NAME} PRIVATE
  -O2 -Wall -Werror -Wextra -std=c++23
)

install(TARGETS ${PROJECT_NAME} DESTINATION bin)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/dist/debug")
else()
  set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/dist/release")
endif()
