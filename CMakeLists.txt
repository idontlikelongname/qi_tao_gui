cmake_minimum_required(VERSION 3.1.0)

project(dan_fa_qi_tao)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O3")

# will generate in the <AUTOGEN_BUILD_DIR>/include
# automatically added to the target's INCLUDE_DIRECTORIES
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
# <basename>.ui file is searched for in the following places
# <source_dir>/<basename>.ui 
# <source_dir>/<path><basename>.ui 
# <AUTOUIC_SEARCH_PATHS>/<basename>.ui 
# <AUTOUIC_SEARCH_PATHS>/<path><basename>.ui
# ui_*.h are placed in the <AUTOGEN_BUILD_DIR>/include which is automatically added to the target's INCLUDE_DIRECTORIES
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets Core SerialPort REQUIRED)

# OpenCV
find_package(OpenCV)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(dan_fa_qi_tao
    main.cpp 
    chukujiemian.cpp 
    dialog.cpp 
    qitaojiemian.cpp
    wigglywidget.cpp 
    ruku.cpp 
    data_model.cpp
    yilantable.cpp
    res.qrc  
    qitaojiemian.ui     
    chukujiemian.ui
    ruku.ui
    yilantable.ui
)

target_link_libraries(dan_fa_qi_tao 
    ${OpenCV_LIBRARIES} 
    Qt5::Widgets 
    Qt5::Core
    Qt5::SerialPort
)


add_executable(serial_test
    serial_test.cpp
)

target_link_libraries(serial_test
    ${OpenCV_LIBRARIES} 
    Qt5::Widgets 
    Qt5::Core
    Qt5::SerialPort
)