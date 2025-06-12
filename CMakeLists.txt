cmake_minimum_required(VERSION 3.16)
project(RoyalCarRental)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql PrintSupport)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/qcustomplot
)

file(GLOB_RECURSE SOURCES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
    ${CMAKE_SOURCE_DIR}/qcustomplot/*.cpp
)

file(GLOB_RECURSE HEADERS
    ${CMAKE_SOURCE_DIR}/include/*.h
    ${CMAKE_SOURCE_DIR}/qcustomplot/*.h
)

## Add all ui files from the Forms directory
#file(GLOB_RECURSE UIS
#    ${CMAKE_SOURCE_DIR}/Forms/*.ui
#)
qt_add_resources(RESOURCES resources.qrc)

add_executable(${PROJECT_NAME}
    ${SOURCES}
    ${HEADERS}
   # ${UIS}
)

#set_target_properties(${PROJECT_NAME} PROPERTIES
#    AUTOUIC_SEARCH_PATHS "${CMAKE_SOURCE_DIR}/Forms"
#)

# ✅ Link resources
target_sources(${PROJECT_NAME} PRIVATE ${RESOURCES})

target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
    Qt6::PrintSupport
)
