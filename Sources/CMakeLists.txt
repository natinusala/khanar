link_directories(
    ${GTKMM_LIBRARY_DIRS}  )

include_directories(
    ${GTKMM_INCLUDE_DIRS}  )

add_executable(example # name of the executable on Windows will be example.exe 
    fenetre.cc)

target_link_libraries(example 
    ${GTKMM_LIBRARIES}  )
