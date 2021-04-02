cmake_minimum_required(VERSION 3.8)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules")

find_package(Alembic)
find_package(OpenEXR)

add_executable(abc_time_echo main.cc)

target_include_directories(abc_time_echo PUBLIC ${INCLUSIONS_ALEMBIC} ${INCLUSIONS_OPENEXR})
target_link_libraries(abc_time_echo ${BIBLIOTHEQUES_ALEMBIC} ${BIBLIOTHEQUES_OPENEXR})
