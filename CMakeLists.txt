cmake_minimum_required(VERSION 2.8)

project(Physfs4Cpp)
  if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_CXX_FLAGS "--std=gnu++11 ${CMAKE_CXX_FLAGS}")
    endif ()
  else ()
    set (CMAKE_CXX_STANDARD 11)
  endif ()

#set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")
find_package(PhysFS REQUIRED)
set(LIBS ${LIBS} ${PHYSFS_LIBRARY})
  
# Find source files
file(GLOB SOURCES "src/*.cpp")

# Include header files
include_directories("include")

#Link libraries

# Create shared library
add_library(${PROJECT_NAME} SHARED ${SOURCES})
target_link_libraries(Physfs4Cpp ${LIBS})

# Install library
install(TARGETS ${PROJECT_NAME} DESTINATION lib/${PROJECT_NAME})

# Install library headers
install(DIRECTORY "include/" DESTINATION include/${PROJECT_NAME})
