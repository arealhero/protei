#ifndef PROTEI_NETWORK_SOCKET_ERROR_HPP_
#define PROTEI_NETWORK_SOCKET_ERROR_HPP_

#include <stdexcept>
#include <cerrno>
#include <string>

namespace protei::network {
class socket_error : public std::runtime_error {
 public:
  explicit socket_error(const std::string& what_arg)
    : std::runtime_error(what_arg + " (" + std::strerror(errno) + ")")
  {}
};
}

#endif
