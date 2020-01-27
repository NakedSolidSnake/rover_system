cmake_minimum_required(VERSION 3.0)

project(roverclient)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

 set (CMAKE_PREFIX_PATH $ENV{HOME}/Qt/5.12.1/gcc_64)


find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)

add_executable(RoverClient main.cpp)

target_sources(RoverClient
 PUBLIC
 ${CMAKE_CURRENT_LIST_DIR}/widget.ui 
 ${CMAKE_CURRENT_LIST_DIR}/main.cpp 
 ${CMAKE_CURRENT_LIST_DIR}/persistconfig.cpp
 ${CMAKE_CURRENT_LIST_DIR}/protocol.cpp
 ${CMAKE_CURRENT_LIST_DIR}/widget.cpp
 )

 target_include_directories(RoverClient
 PUBLIC
 ${CMAKE_CURRENT_LIST_DIR}/
 )

target_link_libraries(RoverClient Qt5::Core Qt5::Widgets Qt5::Network) 

#------------------------------------------------------------------------------------------------------
#create a .deb file
#------------------------------------------------------------------------------------------------------
set(CPACK_PACKAGE_DESCRIPTION "Rover Client package")
set(CPACK_PACKAGE_VERSION_MAJOR "${VERSION_MAJOR}")
set(CPACK_PACKAGE_VERSION_MINOR "${VERSION_MINOR}")
set(CPACK_PACKAGE_VERSION_PATCH "${VERSION_PATCH}")
set(CPACK_PACKAGE_VENDOR "Solid Company")
set(CPACK_PACKAGE_CONTACT "support@acme.corp")
set(CPACK_GENERATOR "DEB")
set(CPACK_PACKAGE_FILE_NAME "${CMAKE_PROJECT_NAME}-${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

    set(CPACK_INSTALL_PREFIX "/")
    set(CPACK_PACKAGE_INSTALL_DIRECTORY "bin")
    set(CMAKE_INSTALL_PREFIX ${})
# set (CPACK_SET_DESTDIR "/")
# INSTALL(FILES roverclient DESTINATION "bin")

include (CPack)
