cmake_minimum_required(VERSION 2.6)
project( IPC_Serial )
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}  -std=c++0x -O2 -g3 -fPIC")

set(SYSROOT_DIR /home/fan/Qt5.10.0/5.10.0/gcc_64)#your sysroot path
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/include/)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/include/QtCore)
INCLUDE_DIRECTORIES(${SYSROOT_DIR}/include/QtDBus)
INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/src/)
link_directories(${SYSROOT_DIR}/lib/ /usr/lib/)
AUX_SOURCE_DIRECTORY(${CMAKE_SOURCE_DIR}/src/ SRC_LIST_1)


ADD_EXECUTABLE(IPC_Serial  ${SRC_LIST_1} )

TARGET_LINK_LIBRARIES(IPC_Serial Qt5Core Qt5DBus pthread )


