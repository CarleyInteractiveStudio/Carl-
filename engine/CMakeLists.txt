cmake_minimum_required(VERSION 3.10)
project(CarlEngine)

set(CMAKE_CXX_STANDARD 17)

# Incluir directorios
include_directories(include)

# El Cerebro de Carl: Librería compartida para Python
add_library(carl_neuronal SHARED src/carl_neuronal.cpp)

# Motor de Inferencia Principal
add_executable(carl_engine src/main.cpp)
target_link_libraries(carl_engine carl_neuronal)

# Motor de Entrenamiento de Alto Rendimiento
add_executable(carl_trainer src/trainer.cpp)
target_link_libraries(carl_trainer carl_neuronal)

# (Opcional) Agregar soporte para OpenBLAS/Eigen si el usuario lo solicita explícitamente más adelante
# target_link_libraries(carl_engine ...)
