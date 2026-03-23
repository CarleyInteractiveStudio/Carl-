cmake_minimum_required(VERSION 3.10)
project(CarlEngine)

set(CMAKE_CXX_STANDARD 17)

# Incluir directorios
include_directories(include)

# Definir ejecutable (las rutas deben ser relativas al CMakeLists.txt)
add_executable(carl_engine src/main.cpp)

# (Opcional) Agregar soporte para OpenBLAS/Eigen si el usuario lo solicita explícitamente más adelante
# target_link_libraries(carl_engine ...)
