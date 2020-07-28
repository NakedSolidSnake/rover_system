add_library(app "")
target_link_libraries(app PUBLIC ipc)
target_sources(app 
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/lib/memory_init.c 
  ${CMAKE_CURRENT_LIST_DIR}/lib/protocol.c 
  )
target_include_directories(app 
  PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/include
  )

  add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/src)