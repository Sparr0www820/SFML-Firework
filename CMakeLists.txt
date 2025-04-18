cmake_minimum_required(VERSION 3.30)
project(CMakeSFMLProject LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include(FetchContent)
FetchContent_Declare(SFML
        GIT_REPOSITORY https://github.com/SFML/SFML.git
        GIT_TAG 3.0.0
        GIT_SHALLOW ON
        EXCLUDE_FROM_ALL
        SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(firework src/main.cpp
)
target_compile_features(firework PRIVATE cxx_std_20)
target_include_directories(firework PRIVATE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(firework PRIVATE SFML::Graphics SFML::Audio)
