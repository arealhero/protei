#include "tcp_client_socket.h"

#include <iostream>

namespace client_side::sockets {
void tcp_client_socket::init(const std::string& address, uint16_t port)
{
  socket.open(SOCK_STREAM, IPPROTO_TCP);

  socket.set_option(SO_NOSIGPIPE, 1);

  socket.connect(address, port);

  std::cout << "Connected to " << address << ':' << port << std::endl;
}

void tcp_client_socket::send(const std::string& request)
{
  socket.send(request);
}

std::string tcp_client_socket::receive(size_t length)
{
  return socket.receive(length);
}
}
