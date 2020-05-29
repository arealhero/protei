#ifndef PROTEI_NETWORK_SOCKET_HPP_
#define PROTEI_NETWORK_SOCKET_HPP_

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fcntl.h>

#include <type_traits>
#include <memory>

#include "socket_error.hpp"

namespace protei::network {
class socket {
 public:
  socket() = default;
  socket(const socket &) = default;
  socket(socket &&) noexcept = default;

  explicit socket(int fd)
      : fd(fd) {}

  socket &operator=(const socket &) = default;
  socket &operator=(socket &&) noexcept = default;

  virtual ~socket() = default;

  void open(int type, int protocol) {
    fd = ::socket(AF_INET, type, protocol);
    if (fd == -1) {
      throw socket_error{"Failed to create socket"};
    }
  }
  void close() const {
    invoke(::close, "Failed to close socket", fd);
  }

  template<typename T>
  void set_option(const int option, const T &value) const {
    invoke(::setsockopt, "Failed to set socket option", fd, SOL_SOCKET, option, &value, sizeof(value));
  }

  void set_non_block() const {
    int flags = ::fcntl(fd, F_GETFL, 0);
    if (flags == -1) {
      flags = 0;
    }

    invoke(::fcntl, "Failed to set socket's I/O to non-blocking", fd, F_SETFL, flags | O_NONBLOCK);
  }

  void connect(const sockaddr_in &socket_address) const {
    invoke(::connect, "Failed to connect", fd, (sockaddr *) &socket_address, sizeof(socket_address));
  }
  void connect(const std::string &address, const uint16_t port) const {
    auto socket_address = to_socket_address(address, port);
    connect(socket_address);
  }

  void bind(const sockaddr_in &socket_address) const {
    invoke(::bind, "Failed to bind", fd, (sockaddr *) &socket_address, sizeof(socket_address));
  }
  void bind(const uint32_t address, const uint16_t port) const {
    auto socket_address = to_socket_address(address, port);
    bind(socket_address);
  }

  void listen(int max = SOMAXCONN) const {
    invoke(::listen, "Failed to listen", fd, max);
  }

  [[nodiscard]]
  socket accept() const {
    int accepted_fd = ::accept(fd, nullptr, nullptr);
    if (accepted_fd == -1 && errno != EAGAIN) {
      throw socket_error{"Failed to accept connection"};
    }

    return socket{accepted_fd};
  }

  void send(const std::string &message, int flags = 0) const {
    invoke(::send, "Failed to send message", fd, message.c_str(), message.length(), flags);
  }
  void send_to(const std::string &message, const sockaddr_in &socket_address, int flags = 0) const {
    invoke(::sendto, "Failed to send message", fd, message.c_str(), message.length(), flags,
           (sockaddr *) &socket_address, sizeof(socket_address));
  }
  void send_to(const std::string& message, const std::string& address, const uint16_t port, int flags = 0) const {
    auto socket_address = to_socket_address(address, port);
    send_to(message, socket_address, flags);
  }

  [[nodiscard]]
  std::string receive(size_t length, int flags = 0) const {
    std::shared_ptr<char> buffer(new char[length]);

    int count = ::recv(fd, buffer.get(), length, flags);
    if (count == -1) {
      if (errno == EAGAIN) {
        count = 0; // If we got EAGAIN then return an empty string (count = 0).
      } else {
        throw socket_error{"Failed to receive message"};
      }
    }

    auto result = std::string{buffer.get(), static_cast<size_t>(count)};
    return result;
  }

  [[nodiscard]]
  std::pair<std::string, sockaddr_in> receive_from(size_t length, int flags = 0) const {
    std::shared_ptr<char> buffer(new char[length]);

    sockaddr_in socket_address{};
    socklen_t address_length = sizeof(socket_address);

    int count = ::recvfrom(fd, buffer.get(), length, flags, (sockaddr *) &socket_address, &address_length);
    if (count == -1) {
      if (errno == EAGAIN) {
        count = 0; // If we got EAGAIN then return an empty string (count = 0).
      } else {
        throw socket_error{"Failed to receive message"};
      }
    }

    auto result = std::string{buffer.get(), static_cast<size_t>(count)};
    return { result, socket_address };
  }

  [[nodiscard]]
  inline int descriptor() const {
    return fd;
  }

 private:
  int fd = -1;

  template<typename Function, typename... Args>
  static void invoke(const Function& func, const std::string& failure_message, Args... args) {
    auto result = func(args...);
    if (result == -1) {
      throw socket_error{failure_message};
    }
  }

  static sockaddr_in to_socket_address(const std::string &address, const uint16_t port) {
    sockaddr_in socket_address{};
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = inet_addr(address.c_str());
    socket_address.sin_port = htons(port);

    return socket_address;
  }
  static sockaddr_in to_socket_address(const uint32_t address, const uint16_t port) {
    sockaddr_in socket_address{};
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = htonl(address);
    socket_address.sin_port = htons(port);

    return socket_address;
  }
};
}

#endif
