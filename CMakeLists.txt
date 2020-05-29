cmake_minimum_required(VERSION 3.16)
project(protei)

set(CMAKE_CXX_STANDARD 17)

include_directories(include/)

add_executable(server
        server_side/main.cc
        server_side/src/event_loop.cc
        server_side/src/handlers/sum_request_handler.cc
        server_side/src/server.cc)

add_executable(client
        client_side/main.cc
        client_side/src/sockets/tcp_client_socket.cc
        client_side/src/sockets/udp_client_socket.cc)
