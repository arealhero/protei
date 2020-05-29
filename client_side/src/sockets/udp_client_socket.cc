#include "udp_client_socket.h"

namespace client_side::sockets {
void udp_client_socket::init(const std::string& address, uint16_t port)
{
  server_address = address;
  server_port = port;

  socket.open(SOCK_DGRAM, IPPROTO_UDP);
}

void udp_client_socket::send(const std::string& request)
{
  socket.send_to(request, server_address, server_port);
}

std::string udp_client_socket::receive(size_t length)
{
  auto [message, socket_address] = socket.receive_from(length);
  return message;
}

void udp_client_socket::close() {
  socket.close();
}
}
