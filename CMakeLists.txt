cmake_minimum_required(VERSION 3.21)
project(prjtritium)

set(CMAKE_CXX_STANDARD 20)
add_library(tritium src/device.cpp src/bel.cpp inc/cista.h src/wire.cpp src/pip.cpp src/belpin.cpp )
add_executable(dbgen src/device.cpp src/bel.cpp inc/cista.h src/wire.cpp src/pip.cpp src/belpin.cpp dbgen/BelCommon.cpp dbgen/BelCommon.h dbgen/GridCell.cpp dbgen/GridCell.h inc/types.h dbgen/EFLGenerator.cpp dbgen/EFLGenerator.h dbgen/RoutingGenerator.cpp dbgen/RoutingGenerator.h dbgen/EFTGenerator.cpp dbgen/EFTGenerator.h dbgen/dbgen.cpp)
target_include_directories(tritium PUBLIC inc)
target_include_directories(dbgen PUBLIC inc)

find_package(fmt)
target_link_libraries(tritium fmt::fmt)
target_link_libraries(dbgen fmt::fmt)
