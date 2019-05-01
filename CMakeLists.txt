cmake_minimum_required(VERSION 3.11)
project(mcircuit)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Threads REQUIRED)

add_subdirectory(mcircuit)

set(QMCIRCUIT_SOURCES
        src/main_window.cpp
        src/renderer.cpp
        src/ui_component.cpp
        src/schematic_view.cpp
        src/widgets/property_editor.cpp
        src/ui_components/not_gate.cpp
        src/ui_components/switch.cpp
        src/ui_components/led.cpp
        src/ui_components/basic_gate.cpp
        src/ui_component_registry.cpp
        src/controllers/wire_drawing_controller.cpp
        src/ui_components/led_matrix.cpp
        src/ui_components/custom_component.cpp
        src/schematic.cpp
        src/ui_components/pin.cpp)

set(QMCIRCUIT_HEADERS
        include/main_window.h
        include/renderer.h
        include/ui_component.h
        include/schematic_view.h
        include/context.h
        include/widgets/property_editor.h
        include/ui_components/not_gate.h
        include/ui_components/switch.h
        include/ui_components/led.h
        include/ui_components/basic_gate.h
        include/ui_component_registry.h
        include/controllers/wire_drawing_controller.h
        include/ui_components/led_matrix.h
        include/ui_components/custom_component.h
        include/schematic.h
        include/ui_components/pin.h)

add_executable(qmcircuit ${QMCIRCUIT_SOURCES} src/main.cpp ${QMCIRCUIT_HEADERS})

target_include_directories(qmcircuit PUBLIC ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(qmcircuit Qt5::Widgets Threads::Threads mcircuit)
