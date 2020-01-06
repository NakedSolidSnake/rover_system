cmake_minimum_required(VERSION 3.7.0)

project(rasp_rover)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

# This one is important
SET(CMAKE_SYSTEM_NAME Linux)

# Specify the cross compiler
#SET(CMAKE_C_COMPILER   arm-linux-gnueabihf-gcc)
#SET(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

set (CMAKE_C_FLAGS "-D_GNU_SOURCE -Wall -std=c99")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_BUILD_TYPE Debug)


function(add_test_unit BIN_NAME BIN SOURCE)
  add_executable(${BIN} ${SOURCE})
  set_target_properties(${BIN}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/test_lib
    )
    add_test(NAME ${BIN_NAME} COMMAND ${BIN})
endfunction()

enable_testing()

# ipc library
add_library(ipc "")
target_sources(ipc 
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/src/log/log.c
  ${CMAKE_CURRENT_LIST_DIR}/src/file/file.c
  ${CMAKE_CURRENT_LIST_DIR}/src/queue/queue.c
  ${CMAKE_CURRENT_LIST_DIR}/src/signal/signal.c
  ${CMAKE_CURRENT_LIST_DIR}/src/sharedmemory/sharedmemory.c
  ${CMAKE_CURRENT_LIST_DIR}/src/semaphore/semaphore.c
  )
target_include_directories(ipc 
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/include
  )

#comm library 
add_library(comm "")
target_link_libraries(comm PUBLIC pthread)
target_sources(comm 
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/src/server/tcp.c
  )
target_include_directories(comm 
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/include
  )

#util library 
add_library(util "")
target_sources(util 
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/src/util/time_meas.c
  )
target_include_directories(util 
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/include
  )

#------------------------------------------------------------------------------------------------------
#Tests
#------------------------------------------------------------------------------------------------------
add_test_unit(time_meas test_time_meas test_lib/test_time_meas.c)
target_link_libraries(test_time_meas PUBLIC util)

add_test_unit(server_tcp test_server_tcp test_lib/test_servertcp.c)
target_link_libraries(test_server_tcp PUBLIC comm)
