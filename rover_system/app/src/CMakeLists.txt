# auxiliar
add_library(auxiliar "")
target_link_libraries(auxiliar PUBLIC app)
target_sources(auxiliar
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/tasks/libs/auxiliar.c
  )
target_include_directories(auxiliar
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/tasks/libs
  ${CMAKE_CURRENT_LIST_DIR}/tasks/include
  )

# control_interface
add_library(control_interface "")
target_link_libraries(control_interface PUBLIC app auxiliar)
target_sources(control_interface
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/tasks/libs/control_interface.c
  )
target_include_directories(control_interface
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/tasks/libs
  ${CMAKE_CURRENT_LIST_DIR}/tasks/include
  )


#------------------------------------------------------------------------------------------------------
#rover_client
#------------------------------------------------------------------------------------------------------
add_executable(rover_client "")
target_link_libraries(rover_client PUBLIC app)
target_sources(rover_client
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/rover_client.c
  )
target_include_directories(rover_client
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/include  
  )
install(TARGETS rover_client DESTINATION bin)



#rover_admin
add_executable(rover_admin "")
target_link_libraries(rover_admin PUBLIC ipc app)
target_sources(rover_admin
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/rover_admin.c
  )
target_include_directories(rover_admin
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/include
  )
install(TARGETS rover_admin DESTINATION bin)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/${MODE})




