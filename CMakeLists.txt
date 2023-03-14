cmake_minimum_required(VERSION 3.10) # Set minimum required CMake version
project(madPanner) # Executable name
file(GLOB SOURCES "src/*.cpp") # Store all .cpp files in folder 'src' to variable 'SOURCES'
include_directories(/isr/include/jack) # Include the jack header file
include_directories(include) # Include all the project header files
include_directories(libs) # Include all the library header files
add_executable(madPanner ${SOURCES}) # Create executable 'madPanner'
target_link_libraries(madPanner /usr/lib/x86_64-linux-gnu/libjack.so) # Tell the linker where to find the jack library
