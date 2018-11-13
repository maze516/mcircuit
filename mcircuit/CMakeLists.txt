cmake_minimum_required(VERSION 3.11)
project(mcircuit)

set(CMAKE_CXX_STANDARD 17)

set(MCIRCUIT_SOURCES
        src/pin.cpp
        src/component.cpp
        src/wire_manager.cpp
        src/algorithm.cpp
        src/components/not_gate.cpp
        src/components/basic_gate.cpp
        src/components/custom_component.cpp
        src/components/switch.cpp
        src/components/variable_input_gate.cpp
        src/components/led.cpp
        src/components/led_matrix.cpp)

set(MCIRCUIT_HEADERS
        include/mcircuit/pin.h
        include/mcircuit/component.h
        include/mcircuit/wire_manager.h
        include/mcircuit/algorithm.h
        include/mcircuit/components/not_gate.h
        include/mcircuit/components/basic_gate.h
        include/mcircuit/components/custom_component.h
        include/mcircuit/components/switch.h
        include/mcircuit/components/variable_input_gate.h
        include/mcircuit/components/led.h
        include/mcircuit/components/led_matrix.h)

find_package(Boost COMPONENTS unit_test_framework)

add_library(mcircuit ${MCIRCUIT_SOURCES} ${MCIRCUIT_HEADERS})
target_include_directories(mcircuit PUBLIC include)
