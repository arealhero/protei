#ifndef PROTEI_CLIENT_SOCKETS_TCP_CLIENT_SOCKET_H_
#define PROTEI_CLIENT_SOCKETS_TCP_CLIENT_SOCKET_H_

#include "client_socket.h"

namespace client_side::sockets {
class tcp_client_socket : public client_socket
{
 public:
  void init(const std::string& address, uint16_t port) override;
  void send(const std::string& request) override;
  [[nodiscard]] std::string receive(size_t length) override;

  [[nodiscard]] bool skip_empty_request() const override
  {
    // from `man recv`:
    //  "For TCP sockets, the return value 0 means the peer has closed its half
    //  side of the connection"
    return true;
  }
};
}

#endif
