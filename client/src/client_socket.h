#ifndef PROTEI_CLIENT_CLIENT_SOCKET_H_
#define PROTEI_CLIENT_CLIENT_SOCKET_H_

#include <cstdint>
#include <string>

#include <protei/network/socket.hpp>

class client_socket {
 public:
  virtual ~client_socket() = default;

  virtual void init(const std::string& address, uint16_t port) = 0;
  virtual void send(const std::string& request) = 0;
  virtual std::string receive(size_t length) = 0;

  [[nodiscard]]
  virtual bool skip_empty_request() const = 0;

 protected:
  protei::network::socket socket;
};

#endif
