cmake_minimum_required(VERSION 3.24)
project(Pong)

set(CMAKE_CXX_STANDARD 17)

set(SDL2_LIB_DIR C:/LibSDL/VC/SDL2-2.26.3/lib/x64)
set(SDL2_INCLUDE_DIR C:/LibSDL/VC/SDL2-2.26.3/include)

set(SDL2MIXER_LIB_DIR "C:/LibSDL/VC/SDL2_mixer-2.6.3/lib/x64")
set(SDL2MIXER_INCLUDE_DIR "C:/LibSDL/VC/SDL2_mixer-2.6.3/include")

set(SDL2_IMAGE_LIB_DIR "C:/SDL2_image-2.6.3/lib/x64")
set(SDL2_IMAGE_INCLUDE_DIR "C:/SDL2_image-2.6.3/include")

set(SDL2TTF_LIB_DIR "C:/LibSDL/VC/SDL2_ttf-2.20.2/lib/x64")
set(SDL2TTF_INCLUDE_DIR "C:/LibSDL/VC/SDL2_ttf-2.20.2/include")

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} C:/LibSDL/VC/SDL2-2.26.3/lib/x64/SDL2.dll")

include_directories(${SDL2_INCLUDE_DIR} ${SDL2MIXER_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR})
add_executable(${PROJECT_NAME} src/main.cpp Src/Objects/objects.h Src/Textures/TextureManager.cpp Src/Textures/TextureManager.h Src/Core/Game.h Src/Core/Game.cpp Src/Core/Engine.cpp)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIB_DIR}/SDL2.lib ${SDL2_LIB_DIR}/SDL2main.lib ${SDL2MIXER_LIB_DIR}/SDL2_mixer.lib ${SDL2_IMAGE_LIB_DIR}/SDL2_image.lib ${SDL2TTF_LIB_DIR}/SDL2_ttf.lib)