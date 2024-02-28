cmake_minimum_required(VERSION 3.10)
project(UDPReceiver VERSION 1 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INSTALL_RPATH $ORIGIN)
# verbose output - useful for debugging, but turned off by default
set(CMAKE_VERBOSE_MAKEFILE OFF CACHE BOOL "Verbose")
set(APP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/app)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "None" "Debug" "Release")
endif()

# Find threads package
find_package(Threads REQUIRED)

set(HEADER_FILES
    ${APP_DIR}/inc/UDPReceiver.hpp
    ${APP_DIR}/inc/Message.hpp
    ${APP_DIR}/inc/MessageProcessor.hpp
    ${APP_DIR}/inc/MessageManager.hpp
)

# Add source files
set(SOURCE_FILES
    ${APP_DIR}/src/main.cpp
    ${APP_DIR}/src/Message.cpp
    ${APP_DIR}/src/UDPReceiver.cpp
    ${APP_DIR}/src/MessageProcessor.cpp
    ${APP_DIR}/src/MessageManager.cpp
)

# Generate the executable
add_executable(udp_receiver ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(udp_receiver PRIVATE ${APP_DIR}/inc)

# Link the Threads package
target_link_libraries(udp_receiver PRIVATE Threads::Threads)

#compilation options
set_target_properties(udp_receiver
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    CXX_VISIBILITY_PRESET hidden
)

target_compile_options(udp_receiver
    PRIVATE "-Wall"
    PRIVATE "-Wextra"
    PRIVATE "-Wfloat-equal"
    PRIVATE "-pedantic"
    PRIVATE "$<$<NOT:$<CONFIG:Debug>>:-O3>"
)