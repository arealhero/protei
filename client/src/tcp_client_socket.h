#ifndef PROTEI_CLIENT_TCP_CLIENT_SOCKET_H_
#define PROTEI_CLIENT_TCP_CLIENT_SOCKET_H_

#include <iostream>

#include "client_socket.h"

class tcp_client_socket : public client_socket {
 public:
  void init(const std::string &address, uint16_t port) override {
    socket.open(SOCK_STREAM, IPPROTO_TCP);

    socket.set_option(SO_NOSIGPIPE, 1);

    socket.connect(address, port);

    std::cout << "Connected to " << address << ':' << port << std::endl;
  }

  void send(const std::string &request) override {
    socket.send(request);
  }

  std::string receive(size_t length) override {
    auto response = socket.receive(length);
    return response;
  }

  [[nodiscard]]
  bool skip_empty_request() const override {
    // from `man recv`:
    //  "For TCP sockets, the return value 0 means the peer has closed its half side of the connection"
    return true;
  }
};

#endif
