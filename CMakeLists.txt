cmake_minimum_required(VERSION 3.22)
project(kickle-clone CXX)

add_subdirectory(vendor/trollworks-sdk-backend-sdl)
add_subdirectory(vendor/physfs)
add_subdirectory(vendor/physfsrwops)

file(GLOB_RECURSE GAME_SOURCES
  ${PROJECT_SOURCE_DIR}/src/*.cpp
)

file(GLOB_RECURSE GAME_HEADERS
  ${PROJECT_SOURCE_DIR}/include/*.hpp
)

add_executable(${PROJECT_NAME}
  ${GAME_SOURCES}
  ${GAME_HEADERS}
)

if(WIN32)
  target_link_libraries(${PROJECT_NAME}
    mingw32
    SDL2main
    SDL2-static
    trollworks-sdk-backend-sdl
    physfs-static
    physfsrwops
    opengl32
    glu32
  )
elseif(APPLE)
  target_link_libraries(${PROJECT_NAME}
    SDL2-static
    trollworks-sdk-backend-sdl
    physfs-static
    physfsrwops
    opengl
    glu
  )
elseif(UNIX)
  target_link_libraries(${PROJECT_NAME}
    SDL2-static
    trollworks-sdk-backend-sdl
    physfs-static
    physfsrwops
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
