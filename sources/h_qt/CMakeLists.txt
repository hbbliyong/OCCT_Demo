set(LIBRARY_NAME OCC_QT)


file(GLOB_RECURSE HEADER_FILES "include/*.h*")
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}/include
    PREFIX "Header Files"
    FILES ${HEADER_FILES})

source_group(
    TREE ${CMAKE_CURRENT_SOURCE_DIR}/src
    PREFIX "Source Files"
    FILES ${SRC_FILES})

find_package(Qt6 COMPONENTS Core Gui OpenGL Xml Widgets REQUIRED)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(${LIBRARY_NAME} ${SRC_FILES} ${HEADER_FILES})
target_include_directories(${LIBRARY_NAME} PRIVATE ${OpenCASCADE_INCLUDE_DIRS})


foreach(module ${OpenCASCADE_MODULES})
    target_link_libraries(${LIBRARY_NAME} ${OpenCASCADE_${module}_LIBRARIES})
    # message(STATUS "Linking module ${module} with libraries: ${OpenCASCADE_${module}_LIBRARIES}")
endforeach()



target_link_libraries(${LIBRARY_NAME} Qt6::Core Qt6::Gui Qt6::OpenGL Qt6::Xml Qt6::Widgets)

target_include_directories(${LIBRARY_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
set_property(TARGET ${LIBRARY_NAME} PROPERTY VS_DEBUGGER_ENVIRONMENT
    "PATH=${OpenCASCADE_BINARY_DIR};E:\\99Software_d\\OCCT\\OCCT_SDK\\3rdparty-vc14-64\\3rdparty-all_dlls;%PATH%"
)
