#------------------------------------------------------------------------------------------------------
#lib drivers
#------------------------------------------------------------------------------------------------------
add_library(drivers "")

if(${ARCH} STREQUAL "RASPBERRY")
  message("-- Created ${ARCH} compilation.")  
  target_link_libraries(drivers PUBLIC wiringPi)
  set (FOLDER "raspberry")  
endif()

if(${ARCH} STREQUAL "PC")
  message("-- Created ${ARCH} compilation.")  
  set (FOLDER "pc")

endif()

target_sources(drivers 
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/arch/${FOLDER}/src/hc_sr04.c
  ${CMAKE_CURRENT_LIST_DIR}/arch/${FOLDER}/src/motors.c
  ${CMAKE_CURRENT_LIST_DIR}/arch/${FOLDER}/src/servo.c
  ${CMAKE_CURRENT_LIST_DIR}/src/servo_action.c
  ${CMAKE_CURRENT_LIST_DIR}/src/motors_action.c
  ${CMAKE_CURRENT_LIST_DIR}/src/ultrasound_action.c
)

target_include_directories(drivers 
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/include
  )
