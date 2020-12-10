cmake_minimum_required(VERSION 3.1)

# Set the name of the executable to be produced
set(EXECUTABLE_NAME MicroRally)

# This is needed for threads (atleast on linux)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# These might also be needed
set(CMAKE_THREAD_LIBS_INIT "-lpthread")
set(CMAKE_HAVE_THREADS_LIBRARY 1)
set(CMAKE_USE_WIN32_THREADS_INIT 0)
set(CMAKE_USE_PTHREADS_INIT 1)
set(THREADS_PREFER_PTHREAD_FLAG ON)

# Set project name
project (EXECUTABLE_NAME)

# Add source files
add_executable(${EXECUTABLE_NAME} "")
target_sources(${EXECUTABLE_NAME} PRIVATE
    "main.cpp"
    "constants.hpp"
    "game/Game.hpp"
    "game/Game.cpp"
    "game/GameObject.cpp"
    "game/GameObject.hpp"
    "game/DynamicObject.hpp"
    "game/DynamicObject.cpp"
    "game/GameMap.hpp"
    "game/GameMap.cpp"
    "game/MapTile.hpp"
    "game/MapTile.cpp"
    "game/RaceLine.hpp"
    "game/RaceLine.cpp"
    "game/GameMapDrawable.hpp"
    "game/ContactListener.cpp"
    "game/ContactListener.hpp"
    "lib/json.hpp"
    "game/Tire.cpp"
    "game/Tire.hpp"
    "game/Car.hpp"
    "game/Car.cpp"
    "game/CarData.hpp"
    "game/Box.hpp"
    "game/Box.cpp"
    "game/TireStack.hpp"
    "game/TireStack.cpp"
    "game/OilSpill.hpp"
    "game/OilSpill.cpp"
    "game/Boost.hpp"
    "game/Boost.cpp"
    "scene/SceneManager.hpp"
    "scene/SceneManager.cpp"
    "scene/Scene.hpp"
    "scene/MenuScene.hpp"
    "scene/MenuScene.cpp"
    "scene/GameScene.hpp"
    "scene/GameScene.cpp"
    "scene/SceneComponent.hpp"
    "scene/ButtonSceneComponent.hpp"
    "scene/ButtonSceneComponent.cpp"
    "scene/TextSceneComponent.hpp"
    "scene/TextSceneComponent.cpp"
    "scene/PictureSceneComponent.hpp"
    "scene/PictureSceneComponent.cpp"
    "scene/TextInputSceneComponent.hpp"
    "scene/TextInputSceneComponent.cpp"
    "scene/TableSceneComponent.hpp"
    "scene/TableSceneComponent.cpp"
    "scene/SliderSceneComponent.hpp"
    "scene/SliderSceneComponent.cpp"
    "scene/ListSelectorSceneComponent.hpp"
    "scene/ListSelectorSceneComponent.cpp"
    "network/HostService.hpp"
    "network/HostService.cpp"
    "network/ClientService.hpp"
    "network/ClientService.cpp"
    "settings/Settings.cpp"
    "settings/Settings.hpp"
)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

# Find SFML library
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/usr/share/SFML/cmake/Modules") only needed if program is tested on aalto computers
find_package(SFML 2.5 COMPONENTS system window graphics audio network REQUIRED PATHS C:/Users/User/Desktop/cpp_libs/SFML-2.5.1/lib/cmake/SFML)
target_link_libraries(${EXECUTABLE_NAME} sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

# Find Box2D library 
if(MINGW)
    # for detecting MinGW compiler
    find_package(box2d REQUIRED PATHS C:/Users/User/Desktop/cpp_libs/box2d/lib/cmake/box2d)
    target_link_libraries(${EXECUTABLE_NAME} box2d::box2d)
endif()

if(UNIX AND NOT APPLE)
    # for detecting Linux compilers
    find_package(Box2D REQUIRED)
    target_link_libraries(${EXECUTABLE_NAME} Box2D)
endif()

# For installing targets (not mandatory)
#install (TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
