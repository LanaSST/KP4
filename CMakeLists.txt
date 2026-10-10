cmake_minimum_required(VERSION 3.22.0)
project(MyProject)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(MyProject Source.cpp thread_pool.cpp)
target_link_libraries(MyProject PRIVATE Threads::Threads)
find_package(Threads REQUIRED)

# Создаём динамическую библиотеку (SHARED)
#set(HEADERS_FILES Functions.h)
#set(SOURCES_FILES Functions.cpp)
#set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
#add_library(Functions STATIC ${HEADERS_FILES} ${SOURCES_FILES})

# Линкуем динамическую библиотеку к приложению
#target_link_libraries(MyProject PRIVATE Functions)