cmake_minimum_required(VERSION 3.7.0)

project(rasp_rover)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

# This one is important
SET(CMAKE_SYSTEM_NAME Linux)

# Specify the cross compiler
#set(CMAKE_C_COMPILER   arm-linux-gnueabihf-gcc)
#set(CMAKE_CXX_COMPILER arm-linux-gnueabihf-g++)

set (CMAKE_C_FLAGS "-D_GNU_SOURCE -Wall -std=c99")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_INSTALL_PREFIX "/home/rover")
set(CMAKE_BUILD_TYPE Debug)

function(add_test_unit BIN_NAME BIN SOURCE)
  add_executable(${BIN} ${SOURCE})
  set_target_properties(${BIN}
        PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests
    )
    add_test(NAME ${BIN_NAME} COMMAND ${BIN})
endfunction()

enable_testing()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/driver)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/app)
# add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/libs)

#------------------------------------------------------------------------------------------------------
#Tests
#------------------------------------------------------------------------------------------------------
add_test_unit(servo test_servo tests/test_servo.c)
target_link_libraries(test_servo PUBLIC drivers)

add_test_unit(motors test_motors tests/test_motors.c)
target_link_libraries(test_motors PUBLIC drivers)

add_test_unit(ultrasonic test_ultrasonic tests/test_ultrasonic.c)
target_link_libraries(test_ultrasonic PUBLIC drivers)

#------------------------------------------------------------------------------------------------------
#create a .deb file
#------------------------------------------------------------------------------------------------------
set(CPACK_PACKAGE_DESCRIPTION "Rover package")
set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
set(CPACK_PACKAGE_VENDOR "Solid Company")
set(CPACK_PACKAGE_CONTACT "support@acme.corp")
set(CPACK_PACKAGING_INSTALL_PREFIX ${CMAKE_INSTALL_PREFIX}/rover)
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")
#set(CPACK_INSTALL_SCRIPT "{CMAKE_CURRENT_BINARY_DIR}/install.sh")
include (CPack)

configure_file("${CMAKE_CURRENT_SOURCE_DIR}/scripts/install.sh.in" "${CMAKE_CURRENT_BINARY_DIR}/install.sh" @ONLY)
configure_file("${CMAKE_CURRENT_SOURCE_DIR}/scripts/uninstall.sh.in" "${CMAKE_CURRENT_BINARY_DIR}/uninstall.sh" @ONLY)

# Create Symbolic links
install(FILES config/rover_syslog.conf DESTINATION dat)

install(CODE "execute_process(COMMAND {CMAKE_CURRENT_BINARY_DIR}/install.sh)")


#add_custom_command(TARGET ${CPACK_PACKAGE_FILE_NAME} POST_BUILD COMMAND bash install.sh WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )