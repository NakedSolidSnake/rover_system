cmake_minimum_required(VERSION 3.7.0)

project(rover_rasp)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

# This one is important
SET(CMAKE_SYSTEM_NAME Linux)

set (CMAKE_C_FLAGS "-D_GNU_SOURCE -Wall -std=c99")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_INSTALL_PREFIX "/home/$ENV{USER}")
set(CMAKE_BUILD_TYPE Debug)

macro(install_symlink filepath sympath)
    install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ${filepath} ${sympath})")
    install(CODE "message(\"-- Created symlink: ${sympath} -> ${filepath}\")")
endmacro(install_symlink)

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
include (CPack)

# Copy Files to dat dir
install(FILES config/rover_syslog.conf DESTINATION dat)
install(FILES config/rover.service DESTINATION dat)

install_symlink(${CMAKE_INSTALL_PREFIX}/rover/dat/rover.service /etc/systemd/system/rover.service)
install_symlink(${CMAKE_INSTALL_PREFIX}/rover/dat/rover_syslog.conf /etc/rsyslog.d/rover_syslog.conf)