cmake_minimum_required (VERSION 2.8.11) 

project (gwLora)

set(BINDIR  ${CMAKE_SOURCE_DIR}/bin)
set(AUTHOR_NAME "ywkim")

ADD_DEFINITIONS(-Wall )


include_directories(${CMAKE_SOURCE_DIR}/src)

file(GLOB SRCS
        src/msg.cpp
        src/task.cpp
        src/lora.cpp
        src/rs232.cpp
        src/thread.cpp
        src/server.cpp
)

add_executable(${PROJECT_NAME} ${SRCS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} pthread)

install(TARGETS ${PROJECT_NAME} DESTINATION ${BINDIR})
