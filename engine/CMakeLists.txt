cmake_minimum_required(VERSION 3.10)
project(CarlEngine)

set(CMAKE_CXX_STANDARD 17)

# Incluir directorios
include_directories(include)

# Crear librería compartida para que Python pueda usar los músculos C++
add_library(carl_math SHARED src/carl_math.cpp)

# Definir ejecutable (las rutas deben ser relativas al CMakeLists.txt)
add_executable(carl_engine src/main.cpp)
target_link_libraries(carl_engine carl_math)

# Definir entrenador C++
add_executable(carl_trainer src/trainer.cpp)
target_link_libraries(carl_trainer carl_math)

# (Opcional) Agregar soporte para OpenBLAS/Eigen si el usuario lo solicita explícitamente más adelante
# target_link_libraries(carl_engine ...)
