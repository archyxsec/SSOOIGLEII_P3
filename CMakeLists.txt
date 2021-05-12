cmake_minimum_required(VERSION 3.10)
project(SSOO_P3)

set(CMAKE_CXX_STANDARD 11)

add_executable(SSOO_P3 src/Buscador.cpp include/Buscador.h include/PaymentSystem.h src/semaphoreI.cpp include/semaphoreI.h include/Client_Management.h src/Client_Management.cpp include/colors.h include/Text.h src/Text.cpp include/Coincidence_Format.h src/Coincidence_Format.cpp include/Client_Normal.h src/Client_Normal.cpp include/Client_Premium.h include/Client_Premium_Limit.h src/Client_Premium.cpp src/Client_Premium_Limit.cpp)