cmake_minimum_required(VERSION 3.16)
project(myserver C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB CORE_SRC CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/core/*.c")
file(GLOB NET_SRC  CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/net/*.c")

add_executable(server
  server.c
  helper.c
  ${CORE_SRC}
  ${NET_SRC}
)

target_include_directories(server PRIVATE
  ${CMAKE_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}/core
  ${CMAKE_SOURCE_DIR}/net
)

find_package(Threads REQUIRED)
target_link_libraries(server PRIVATE Threads::Threads)

