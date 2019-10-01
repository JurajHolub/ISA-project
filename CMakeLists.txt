cmake_minimum_required(VERSION 3.14)
project(project)

set(CMAKE_CXX_STANDARD 17)

SET(GCC_COVERAGE_LINK_FLAGS "-lresolv -ldns")

include(FindBIND.cmake)

add_executable( project main.cpp
        tcp_socket.h tcp_socket.cpp
        udp_socket.h udp_socket.cpp
        dns_query.h dns_query.cpp
        )

target_link_libraries(project ${BIND_LIBRARY})