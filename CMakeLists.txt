cmake_minimum_required(VERSION 3.10)
project(BinaryAnalyzer VERSION 1.0.0)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Add executable
add_executable(binary_analyzer app3.cpp)

# Set properties for the executable
set_target_properties(binary_analyzer PROPERTIES
    OUTPUT_NAME "binary_analyzer"
    RUNTIME_OUTPUT_NAME "binary_analyzer"
)

# Platform-specific settings
if(WIN32)
    # Windows-specific settings
    target_compile_definitions(binary_analyzer PRIVATE _CRT_SECURE_NO_WARNINGS)
    set_target_properties(binary_analyzer PROPERTIES
        RUNTIME_OUTPUT_NAME "binary_analyzer.exe"
    )
elseif(APPLE)
    # macOS-specific settings
    set_target_properties(binary_analyzer PROPERTIES
        RUNTIME_OUTPUT_NAME "binary_analyzer"
    )
else()
    # Linux-specific settings
    set_target_properties(binary_analyzer PROPERTIES
        RUNTIME_OUTPUT_NAME "binary_analyzer"
    )
endif()

# Installation
install(TARGETS binary_analyzer
    RUNTIME DESTINATION bin
)

# Copy README and LICENSE to build directory
configure_file(${CMAKE_SOURCE_DIR}/README.md ${CMAKE_BINARY_DIR}/README.md COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/LICENSE ${CMAKE_BINARY_DIR}/LICENSE COPYONLY)

# Print configuration info
message(STATUS "Binary Analyzer ${PROJECT_VERSION}")
message(STATUS "C++ Standard: ${CMAKE_CXX_STANDARD}")
message(STATUS "Build Type: ${CMAKE_BUILD_TYPE}")
message(STATUS "Output Directory: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
