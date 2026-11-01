cmake_minimum_required(VERSION 3.16)

project(PointCloudApp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOUIC_SEARCH_PATHS "${CMAKE_CURRENT_SOURCE_DIR}/ui")

find_package(Qt6 REQUIRED COMPONENTS Widgets OpenGLWidgets)
find_package(VTK 9 REQUIRED COMPONENTS
    CommonCore
    CommonDataModel
    FiltersSources
    GUISupportQt
    InteractionStyle
    RenderingCore
    RenderingOpenGL2
)

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/MainWindow.h
    src/MainWindow.cpp
    src/PointCloudViewer.h
    src/PointCloudViewer.cpp
    ui/MainWindow.ui
)

target_include_directories(${PROJECT_NAME} PRIVATE src include)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt6::Widgets
    Qt6::OpenGLWidgets
    ${VTK_LIBRARIES}
)

vtk_module_autoinit(
    TARGETS ${PROJECT_NAME}
    MODULES ${VTK_LIBRARIES}
)

if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /utf-8)
endif()
