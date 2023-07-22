cmake_minimum_required(VERSION 3.10)
project(MySDLProject)

# Establecer la versión de C++
set(CMAKE_CXX_STANDARD 11)

set(SOURCES
    WinMain.cpp
)


# Ruta a los archivos de encabezado de SDL
include_directories(src)

set(CMAKE_WIN32_EXECUTABLE TRUE)

# Agregar el archivo fuente principal
add_executable(MySDLProject ${SOURCES})


# Ruta a las bibliotecas de SDL
target_link_libraries(MySDLProject PRIVATE SDL2)