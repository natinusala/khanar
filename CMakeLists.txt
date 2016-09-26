project(khanar)
set (CMAKE_CXX_STANDARD 11)
cmake_minimum_required(VERSION 2.8)

find_package(PkgConfig)

pkg_check_modules(GTKMM REQUIRED gtkmm-3.0) 


add_subdirectory(src) 
