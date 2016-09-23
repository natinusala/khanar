link_directories(
    ${GTKMM_LIBRARY_DIRS}  )

include_directories(
    ${GTKMM_INCLUDE_DIRS}  )

add_executable(untitled-file-manager
    fenetre.cc)

target_link_libraries(untitled-file-manager 
    ${GTKMM_LIBRARIES}  )
