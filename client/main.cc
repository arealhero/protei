#include <cstring>
#include <iostream>

#include "src/client_socket.h"
#include "src/tcp_client_socket.h"
#include "src/udp_client_socket.h"

#include "../config.h"

void print_help()
{
  std::cerr << "usage: client -p [tcp/udp]\n";
}

bool check_options(int argc, char* argv[])
{
  if (argc != 3)
    return false;
  if (strcmp(argv[1], "-p") != 0)
    return false;
  return !(strcmp(argv[2], "tcp") != 0 && strcmp(argv[2], "udp") != 0);
}

int main(int argc, char* argv[])
{
  // TODO: use getopt_long
  if (!check_options(argc, argv)) {
    print_help();
    return 1;
  }

  std::shared_ptr<client_socket> client;
  if (strcmp(argv[2], "tcp") == 0) {
    client = std::make_shared<tcp_client_socket>();
  } else {
    client = std::make_shared<udp_client_socket>();
  }

  try {
    client->init("127.0.0.1", PORT);
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  std::string request;

  try {
    while (true) {
      std::getline(std::cin, request);

      if (request.empty() && client->skip_empty_request()) {
        std::cout << "This type of connection cannot send an empty request." << std::endl;
        continue;
      }

      client->send(request);

      size_t length = 2 * MESSAGE_MAX_LENGTH + 2;
      auto response = client->receive(length);

      if (!response.empty()) {
        std::cout << response << std::endl;
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}
