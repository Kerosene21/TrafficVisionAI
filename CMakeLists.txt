cmake_minimum_required(VERSION 3.21)

project(TrafficVisionAI
    VERSION 0.1.0
    DESCRIPTION "Sistema de vision artificial para el analisis de escenas de trafico"
    LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# El proyecto se compila unicamente para 64 bits (x64).
if(NOT CMAKE_SIZEOF_VOID_P EQUAL 8)
    message(FATAL_ERROR
        "TrafficVisionAI requiere una configuracion de 64 bits (x64). "
        "Configura el proyecto con un compilador x64.")
endif()

# OpenCV se resuelve mediante vcpkg en modo manifiesto.
# El toolchain de vcpkg se pasa desde CMakePresets.json usando $env{VCPKG_ROOT},
# de modo que aqui no aparece ninguna ruta absoluta.
find_package(OpenCV CONFIG REQUIRED COMPONENTS core imgcodecs videoio highgui)

add_executable(TrafficVisionAI src/main.cpp)

target_link_libraries(TrafficVisionAI
    PRIVATE
        opencv_core
        opencv_imgcodecs
        opencv_videoio
        opencv_highgui)

if(MSVC)
    target_compile_options(TrafficVisionAI PRIVATE /W4 /utf-8)
    set_property(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}"
        PROPERTY VS_STARTUP_PROJECT TrafficVisionAI)
endif()

message(STATUS "TrafficVisionAI: OpenCV ${OpenCV_VERSION} encontrado en ${OpenCV_DIR}")
