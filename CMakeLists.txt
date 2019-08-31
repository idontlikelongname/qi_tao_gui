cmake_minimum_required(VERSION 3.1.0)

project(dan_fa_qi_tao)

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

find_package(Qt5 COMPONENTS Widgets Core REQUIRED)

add_executable(dan_fa_qi_tao
    main.cpp 
    chukujiemian.cpp 
    dialog.cpp 
    qitaojiemian.cpp
    wigglywidget.cpp  
    res.qrc  
    qitaojiemian.ui     
    chukujiemian.ui
)

target_link_libraries(dan_fa_qi_tao Qt5::Widgets Qt5::Core)