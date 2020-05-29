#include <cstring>
#include <iostream>

#include "src/sockets/client_socket.h"
#include "src/sockets/tcp_client_socket.h"
#include "src/sockets/udp_client_socket.h"

#include "src/args_parser.h"
#include "src/protocol.h"

#include "../config.h"

void usage()
{
  std::cerr << "USAGE\n";
  std::cerr << "\tclient -p <tcp | udp>\n";
  std::cerr << "OR\n";
  std::cerr << "\tclient --protocol <tcp | udp>\n";
}

int main(int argc, char* argv[])
{
  client_side::args_parser parser{};
  std::shared_ptr<client_side::sockets::client_socket> client;

  auto result = parser.get_protocol(argc, argv);

  switch (result) {
    case client_side::protocol::tcp:
      client = std::make_shared<client_side::sockets::tcp_client_socket>();
      break;
    case client_side::protocol::udp:
      client = std::make_shared<client_side::sockets::udp_client_socket>();
      break;
    default:
      usage();
      return 1;
  }

  try {
    client->init("127.0.0.1", PORT);
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  std::string request;

  try {
    while (std::getline(std::cin, request)) {
      if (request.empty() && client->skip_empty_request()) {
        std::cout << "This type of connection cannot send an empty request."
                  << std::endl;
        continue;
      }

      client->send(request);

      size_t length = 2 * MESSAGE_MAX_LENGTH + 2;
      auto response = client->receive(length);

      if (!response.empty()) {
        std::cout << response << std::endl;
      }
    }

    client->close();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}
