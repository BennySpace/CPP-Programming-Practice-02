cmake_minimum_required(VERSION 4.00)
project(ShopQueue VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

set(HEADERS
        ${CMAKE_SOURCE_DIR}/include/Client.h
        ${CMAKE_SOURCE_DIR}/include/Cashier.h
        ${CMAKE_SOURCE_DIR}/include/DoublyLinkedList.h
        ${CMAKE_SOURCE_DIR}/include/GameManager.h
)
set(SOURCES
        ${CMAKE_SOURCE_DIR}/main.cpp
        ${CMAKE_SOURCE_DIR}/src/Client.cpp
        ${CMAKE_SOURCE_DIR}/src/Cashier.cpp
        ${CMAKE_SOURCE_DIR}/src/DoublyLinkedList.cpp
        ${CMAKE_SOURCE_DIR}/src/GameManager.cpp
)

add_executable(ShopQueue ${SOURCES} ${HEADERS})
target_compile_features(ShopQueue PRIVATE cxx_std_17)
target_include_directories(ShopQueue PRIVATE ${CMAKE_SOURCE_DIR}/include)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_compile_options(ShopQueue PRIVATE -Wall -Wextra -pedantic)
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_compile_options(ShopQueue PRIVATE -O3)
endif()