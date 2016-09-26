link_directories(
    ${GTKMM_LIBRARY_DIRS}  )

include_directories(
    ${GTKMM_INCLUDE_DIRS}  )

add_executable(khanar
    fenetre.cc)

target_link_libraries(khanar 
    ${GTKMM_LIBRARIES}  )
