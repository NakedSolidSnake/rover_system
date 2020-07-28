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




