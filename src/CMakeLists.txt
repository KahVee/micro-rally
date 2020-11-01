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
    "GameObject/GameObject.cpp"
    "GameObject/GameObject.hpp"
    "GameObject/DynamicObject.hpp"
	"GameObject/DynamicObject.cpp"
	"GameObject/Tire.cpp"
    "GameObject/Tire.hpp"
	"GameObject/Car.hpp"
	"GameObject/Car.cpp"
)

## If you want to link SFML statically
# set(SFML_STATIC_LIBRARIES TRUE)

## In most cases better set in the CMake cache
# set(SFML_DIR "<sfml root prefix>/lib/cmake/SFML")

# Find SFML library
find_package(SFML 2.5 COMPONENTS system window graphics audio network REQUIRED PATHS C:/Users/User/Desktop/cpp_libs/SFML-2.5.1/lib/cmake/SFML)
target_link_libraries(${EXECUTABLE_NAME} sfml-system sfml-window sfml-graphics sfml-audio sfml-network)

# Find Box2D library
find_package(box2d REQUIRED PATHS /usr/local/lib/cmake/box2d C:/Users/User/Desktop/cpp_libs/box2d/lib/cmake/box2d)
target_link_libraries(${EXECUTABLE_NAME} box2d::box2d)

# For installing targets (not mandatory)
#install (TARGETS ${EXECUTABLE_NAME} DESTINATION bin)
