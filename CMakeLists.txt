cmake_minimum_required(VERSION 3.10)
project(RobotVacuumSimulator)

set(CMAKE_CXX_STANDARD 17)

# Attempt to find SFML 3
find_package(SFML 3 COMPONENTS System Window Graphics Network Audio REQUIRED)

# Include directories
include_directories(include)

# Add executable from all source files
file(GLOB SOURCES "src/*.cpp")
if(NOT SOURCES)
    set(SOURCES "src/main.cpp")
endif()

add_executable(VacuumSim ${SOURCES})

# Link SFML libraries (SFML 3 uses SFML:: naming convention)
target_link_libraries(VacuumSim PRIVATE SFML::System SFML::Window SFML::Graphics SFML::Network SFML::Audio)
