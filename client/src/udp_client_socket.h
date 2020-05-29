#ifndef PROTEI_CLIENT_UDP_CLIENT_SOCKET_H_
#define PROTEI_CLIENT_UDP_CLIENT_SOCKET_H_

#include "client_socket.h"

class udp_client_socket : public client_socket
{
 public:
  void init(const std::string& address, uint16_t port) override
  {
    server_address = address;
    server_port = port;

    socket.open(SOCK_DGRAM, IPPROTO_UDP);
  }

  void send(const std::string& request) override
  {
    socket.send_to(request, server_address, server_port);
  }

  std::string receive(size_t length) override
  {
    auto [message, socket_address] = socket.receive_from(length);
    return message;
  }

  [[nodiscard]] bool skip_empty_request() const override
  {
    return true;
  }

 private:
  std::string server_address;
  uint16_t server_port;
};

#endif
