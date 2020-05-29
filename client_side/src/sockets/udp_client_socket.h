#ifndef PROTEI_CLIENT_SOCKETS_UDP_CLIENT_SOCKET_H_
#define PROTEI_CLIENT_SOCKETS_UDP_CLIENT_SOCKET_H_

#include "client_socket.h"

namespace client_side::sockets {
class udp_client_socket : public client_socket
{
 public:
  void init(const std::string& address, uint16_t port) override;
  void send(const std::string& request) override;
  [[nodiscard]] std::string receive(size_t length) override;

  void close() override;

  [[nodiscard]] bool skip_empty_request() const override
  {
    // Unlike TCP, UDP have no problems dealing with an empty request
    // because the return value 0 does not mean the peer has closed its half
    // side of the connection
    return false;
  }

 private:
  std::string server_address;
  uint16_t server_port;
};
}

#endif
