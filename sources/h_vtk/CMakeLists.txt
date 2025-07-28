set(LIBRARY_NAME h_vtk)
file(GLOB_RECURSE SRC_FILES "src/*.cpp" "src/*.h")

add_executable(${LIBRARY_NAME} ${SRC_FILES})
target_include_directories(${LIBRARY_NAME} PRIVATE ${OpenCASCADE_INCLUDE_DIRS})

# message(STATUS "OpenCASCADE_INCLUDE_DIRS: ${OpenCASCADE_LIBRARY_DIR}")

foreach(module ${OpenCASCADE_MODULES})
    target_link_libraries(${LIBRARY_NAME} ${OpenCASCADE_${module}_LIBRARIES})
    # message(STATUS "Linking module ${module} with libraries: ${OpenCASCADE_${module}_LIBRARIES}")
endforeach()


set_property(TARGET ${LIBRARY_NAME} PROPERTY VS_DEBUGGER_ENVIRONMENT 
    "PATH=${OpenCASCADE_BINARY_DIR};E:\\99Software_d\\OCCT\\OCCT_SDK\\3rdparty-vc14-64\\3rdparty-all_dlls;%PATH%"
)