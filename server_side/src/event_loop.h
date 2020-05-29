#ifndef PROTEI_SERVER_EVENT_LOOP_H_
#define PROTEI_SERVER_EVENT_LOOP_H_

#include <protei/network/socket.hpp>

namespace server_side {
class event_loop {
 public:
  void clear();

  void watch(const protei::network::socket &socket);

  int wait_for_events();

  [[nodiscard]]
  bool has_events(const protei::network::socket &socket) const;

 private:
  fd_set fds;
  int max_descriptor;
};
}

#endif
