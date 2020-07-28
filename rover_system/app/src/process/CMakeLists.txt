add_definitions(-DPROCESS)
  
#rover_server 
add_executable(rover_server "")
target_link_libraries(rover_server PUBLIC  ipc app comm)
target_sources(rover_server
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/src/rover_server.c
  )
target_include_directories(rover_server 
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/include  
  )
install(TARGETS rover_server DESTINATION bin)

#rover_manager
add_executable(rover_manager "")
target_link_libraries(rover_manager PUBLIC ipc app)
target_sources(rover_manager
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/src/rover_manager.c
  )
target_include_directories(rover_manager
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/include
  )
install(TARGETS rover_manager DESTINATION bin)

#rover_motor_control
add_executable(rover_motor_control "")
target_link_libraries(rover_motor_control PUBLIC ipc drivers app)
target_sources(rover_motor_control
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/src/rover_motor_control.c
  )
target_include_directories(rover_motor_control
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/include
  ${CMAKE_CURRENT_LIST_DIR}/driver/include
  )
install(TARGETS rover_motor_control DESTINATION bin)

#rover_servo_control
add_executable(rover_servo_control "")
target_link_libraries(rover_servo_control PUBLIC ipc drivers app)
target_sources(rover_servo_control
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/src/rover_servo_control.c
  )
target_include_directories(rover_servo_control
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/include
  ${CMAKE_CURRENT_LIST_DIR}/driver/include
  )
install(TARGETS rover_servo_control DESTINATION bin)

#rover_ultrasound_control
add_executable(rover_ultrasound_control "")
target_link_libraries(rover_ultrasound_control PUBLIC ipc drivers app)
target_sources(rover_ultrasound_control
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/src/rover_ultrasound_control.c
  )
target_include_directories(rover_ultrasound_control
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/include
  ${CMAKE_CURRENT_LIST_DIR}/driver/include
  )
install(TARGETS rover_ultrasound_control DESTINATION bin)

#rover_lcd16_control
add_executable(rover_lcd16_control "")
target_link_libraries(rover_lcd16_control PUBLIC ipc drivers app)
target_sources(rover_lcd16_control
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/src/rover_lcd16_control.c
  )
target_include_directories(rover_lcd16_control
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/include
  ${CMAKE_CURRENT_LIST_DIR}/driver/include
  )
install(TARGETS rover_lcd16_control DESTINATION bin)

#rover_launch
add_executable(rover_launch "")
target_link_libraries(rover_launch PUBLIC ipc app)
target_sources(rover_launch
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/rover_launch.c
  )
target_include_directories(rover_launch
  PUBLIC   
  ${CMAKE_CURRENT_LIST_DIR}/include
  )
install(TARGETS rover_launch DESTINATION bin)

#rover_unlaunch
add_executable(rover_unlaunch "")
target_link_libraries(rover_unlaunch PUBLIC ipc app)
target_sources(rover_unlaunch
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/rover_unlaunch.c
  )
target_include_directories(rover_unlaunch
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/include
  )
install(TARGETS rover_unlaunch DESTINATION bin)


#rover_mqtt
add_executable(rover_mqtt "")
target_link_libraries(rover_mqtt PUBLIC paho-mqtt3c app)
target_sources(rover_mqtt
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/src/rover_mqtt.c
  )
target_include_directories(rover_mqtt
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/include
  )
install(TARGETS rover_mqtt DESTINATION bin)


# rover_webserver 
add_executable(rover_webserver "")
target_link_libraries(rover_webserver PUBLIC  ipc app comm)
target_sources(rover_webserver
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/src/rover_webserver.c
  )
target_include_directories(rover_webserver 
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/include  
  )
install(TARGETS rover_webserver DESTINATION bin)

# rover_monitor
add_executable(rover_monitor "")
target_link_libraries(rover_monitor PUBLIC ipc app)
target_sources(rover_monitor
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/src/rover_monitor.c
  )
target_include_directories(rover_monitor 
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/include  
  )
install(TARGETS rover_monitor DESTINATION bin)