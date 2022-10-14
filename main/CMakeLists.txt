set(target main)

add_executable(${target} main.cpp)
target_link_libraries(${target} ${OpenCV_LIBS})
set_target_properties(${target} PROPERTIES
    OUTPUT_NAME "${target}"
    PROJECT_LABEL "${target}"
    RUNTIME_OUTPUT_DIRECTORY "../"
)
	
# REPORT
message( STATUS "")
message( STATUS "General configuration for ${target}")
message( STATUS "OK")
message( STATUS "")
