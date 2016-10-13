project(khanar)
set (CMAKE_CXX_STANDARD 11)
set (CMAKE_BUILD_TYPE Debug)
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

SET(RESOURCE_COMPILER xxd)
FILE(GLOB_RECURSE COMPILED_RESOURCES "assets/*")
FOREACH(INPUT_FILE ${COMPILED_RESOURCES})
    get_filename_component(INPUT_DIRECTORY ${INPUT_FILE} DIRECTORY)
    get_filename_component(INPUT_NAME ${INPUT_FILE} NAME)
    get_filename_component(INPUT_NAME_WE ${INPUT_FILE} NAME_WE)
    SET(OUTPUT_FILE ../src/compiled_assets/${INPUT_NAME}.hex)
    ADD_CUSTOM_COMMAND(
        OUTPUT ${OUTPUT_FILE}
      COMMAND cd ${INPUT_DIRECTORY} && echo "\\#ifndef ${INPUT_NAME_WE}_hex" > ${OUTPUT_FILE} && echo "\\#define ${INPUT_NAME_WE}_hex" >> ${OUTPUT_FILE} && ${RESOURCE_COMPILER} -i ${INPUT_NAME} >> ${OUTPUT_FILE} && echo "\\#endif" >> ${OUTPUT_FILE}
        COMMENT "Compiling ${INPUT_NAME} to binary")
    LIST(APPEND COMPILED_RESOURCES ${OUTPUT_FILE})
ENDFOREACH()

add_executable(khanar ${khanar_src} ${COMPILED_RESOURCES})

target_link_libraries(khanar
    ${GTKMM_LIBRARIES}  )
