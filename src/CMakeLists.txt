cmake_minimum_required(VERSION 3.1)

# Set the name of the executable to be produced
set(EXECUTABLE_NAME MyGame)

# This is needed for threads (atleast on linux)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

# These might also be needed
# set(CMAKE_THREAD_LIBS_INIT "-lpthread")
# set(CMAKE_HAVE_THREADS_LIBRARY 1)
# set(CMAKE_USE_WIN32_THREADS_INIT 0)
# set(CMAKE_USE_PTHREADS_INIT 1)
# set(THREADS_PREFER_PTHREAD_FLAG ON)

# Set project name
project (EXECUTABLE_NAME)

# Add source files
add_executable(${EXECUTABLE_NAME} "")
target_sources(${EXECUTABLE_NAME} PRIVATE
    "main.cpp"
    "constants.hpp"
    "GameObject/Game.hpp"
    "GameObject/Game.cpp"
    "GameObject/GameObject.cpp"
    "GameObject/GameObject.hpp"
    "GameObject/DynamicObject.hpp"
    "GameObject/DynamicObject.cpp"
    "GameObject/Tire.cpp"
    "GameObject/Tire.hpp"
    "GameObject/Car.hpp"
    "GameObject/Car.cpp"
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
    "network/HostService.hpp"
    "network/HostService.cpp"
    "network/ClientService.hpp"
    "network/ClientService.cpp"
)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

# Find SFML library
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "/usr/share/SFML/cmake/Modules")
find_package(SFML 2.5 COMPONENTS system window graphics audio network)
target_link_libraries(${EXECUTABLE_NAME} sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

# Find Box2D library
find_package(box2d REQUIRED PATHS /usr/local/lib/cmake/box2d C:/Users/User/Desktop/cpp_libs/box2d/lib/cmake/box2d)
target_link_libraries(${EXECUTABLE_NAME} box2d::box2d)

# For installing targets (not mandatory)
#install (TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
