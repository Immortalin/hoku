#
# Require CMake Version 2.8.10
#
cmake_minimum_required(VERSION 2.8.10)

#
# Include Directory for Header Files
#
include_directories(${CMAKE_SOURCE_DIR}/include)

#
# Add Library SQLite
#
add_library(Sqlite3 ${CMAKE_SOURCE_DIR}/lib/third-party/sqlite3/sqlite3.c
        ${CMAKE_SOURCE_DIR}/include/third-party/sqlite/sqlite3.h)
if(NOT WIN32)
	target_link_libraries(Sqlite3 dl)
endif()

#
# Build Library SQLiteCPP
#
FILE(GLOB SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/sqlite-cpp/*.cpp)
FILE(GLOB INCLUDES ${CMAKE_SOURCE_DIR}/sqlite-cpp/*.h)
add_library(SQLiteCpp STATIC ${SOURCES} ${INCLUDES})
install(TARGETS SQLiteCpp DESTINATION lib)
install(FILES ${INCLUDES} DESTINATION include)