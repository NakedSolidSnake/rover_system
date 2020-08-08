add_definitions(-DTHREAD)

#rover_server 
add_library(rover_server "")
target_link_libraries(rover_server PUBLIC  ipc app comm auxiliar)
target_sources(rover_server
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/src/rover_server.c
  )
target_include_directories(rover_server 
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/include  
  )

#rover_manager
add_library(rover_manager "")
target_link_libraries(rover_manager PUBLIC ipc app auxiliar)
target_sources(rover_manager
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/src/rover_manager.c
  )
target_include_directories(rover_manager
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/include
  )

#rover_motor_control
add_library(rover_motor_control "")
target_link_libraries(rover_motor_control PUBLIC ipc drivers control_interface)
target_sources(rover_motor_control
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/src/rover_motor_control.c
  )
target_include_directories(rover_motor_control
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/include
  ${CMAKE_CURRENT_LIST_DIR}/driver/include
  )

#rover_servo_control
add_library(rover_servo_control "")
target_link_libraries(rover_servo_control PUBLIC ipc drivers control_interface)
target_sources(rover_servo_control
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/src/rover_servo_control.c
  )
target_include_directories(rover_servo_control
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/include
  ${CMAKE_CURRENT_LIST_DIR}/driver/include
  )

#rover_ultrasound_control
add_library(rover_ultrasound_control "")
target_link_libraries(rover_ultrasound_control PUBLIC ipc drivers control_interface)
target_sources(rover_ultrasound_control
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/src/rover_ultrasound_control.c
  )
target_include_directories(rover_ultrasound_control
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/include
  ${CMAKE_CURRENT_LIST_DIR}/driver/include
  )

#rover_lcd16_control
add_library(rover_lcd16_control "")
target_link_libraries(rover_lcd16_control PUBLIC ipc drivers control_interface)
target_sources(rover_lcd16_control
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/src/rover_lcd16_control.c
  )
target_include_directories(rover_lcd16_control
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/include
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/libs
  ${CMAKE_CURRENT_LIST_DIR}/driver/include
  
  )

#rover_mqtt
add_library(rover_mqtt "")
target_link_libraries(rover_mqtt PUBLIC paho-mqtt3c app auxiliar)
target_sources(rover_mqtt
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/src/rover_mqtt.c
  )
target_include_directories(rover_mqtt
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/include
  )

# rover_webserver 
add_library(rover_webserver "")
target_link_libraries(rover_webserver PUBLIC  ipc app comm auxiliar)
target_sources(rover_webserver
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/src/rover_webserver.c
  )
target_include_directories(rover_webserver 
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/../tasks/include  
  )

  # thread
add_executable(rover_thread "")
target_link_libraries(rover_thread
 PUBLIC
  rover_webserver
  rover_mqtt
  rover_lcd16_control
  rover_ultrasound_control
  rover_servo_control
  rover_motor_control
  rover_manager
  rover_server
  pthread
  )
target_sources(rover_thread
PUBLIC 
${CMAKE_CURRENT_LIST_DIR}/rover_thread.c
)
target_include_directories(rover_thread 
PUBLIC 
${CMAKE_SOURCE_DIR}/src/tasks
)
install(TARGETS rover_thread DESTINATION bin)

 # thread_kill
add_executable(rover_thread_kill "")
target_link_libraries(rover_thread_kill PUBLIC  app)
target_sources(rover_thread_kill
PUBLIC 
${CMAKE_CURRENT_LIST_DIR}/rover_thread_kill.c
)
target_include_directories(rover_thread_kill 
PUBLIC 
${CMAKE_SOURCE_DIR}/src/tasks
${CMAKE_CURRENT_LIST_DIR}/../../include
)
install(TARGETS rover_thread_kill DESTINATION bin)