project(khanar)
set (CMAKE_CXX_STANDARD 11)
cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)

pkg_check_modules(GTKMM REQUIRED gtkmm-3.0) 

link_directories(
    ${GTKMM_LIBRARY_DIRS}  )

include_directories(
    ${GTKMM_INCLUDE_DIRS}  )
    
file(GLOB_RECURSE khanar_src
    "src/*.*"
)

add_executable(khanar ${khanar_src})

target_link_libraries(khanar 
    ${GTKMM_LIBRARIES}  )
