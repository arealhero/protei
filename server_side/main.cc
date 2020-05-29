#include <iostream>

#include <protei/network/socket.hpp>

#include "src/handlers/sum_request_handler.h"
#include "src/server.h"

int main(int argc, char* argv[])
{
  server_side::server server;

  std::unique_ptr<server_side::handlers::request_handler> handler =
    std::make_unique<server_side::handlers::sum_request_handler>();
  server.set_handler(std::move(handler));

  try {
    server.init_tcp_socket(INADDR_ANY, PORT);
    server.init_udp_socket(INADDR_ANY, PORT);
  } catch (const std::exception& e) {
    std::cerr << "Failed to init server: " << e.what() << std::endl;
    return 1;
  }

  std::cout << "Listening on port " << PORT << "." << std::endl;

  try {
    while (true) {
      server.update_event_loop();
      server.wait_for_events();

      server.accept_tcp_connections();
      server.handle_tcp_events();
      server.handle_udp_events();
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}
