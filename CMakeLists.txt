cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 17)
project(PuyoPuyo)

include(cmake/IncludeFiles.cmake)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2)
include_directories(${SDL2_INCLUDE_DIR})
find_package(SDL2_image)
include_directories(${SDL2_IMAGE_INCLUDE_DIR})
find_package(SDL2_mixer)
include_directories(${SDL2_MIXER_INCLUDE_DIR})

add_executable(PuyoPuyo ${Includes})

set_target_properties(PuyoPuyo PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/output)

target_include_directories(PuyoPuyo PRIVATE ${CMAKE_SOURCE_DIR}/src)
target_compile_options(PuyoPuyo PRIVATE -Wall)
target_link_options(PuyoPuyo PRIVATE -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer)

target_link_libraries(PuyoPuyo -lstdc++fs ${SDL2_LIBRARIES} ${SDL2_IMAGE_LIBRARIES} ${SDL2_MIXER_LIBRARIES})
