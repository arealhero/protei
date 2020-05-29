#include "event_loop.h"

#include <algorithm>
#include <sys/select.h>

namespace server_side {
void event_loop::clear()
{
  FD_ZERO(&fds);
  max_descriptor = -1;
}

void event_loop::watch(const protei::network::socket& socket)
{
  auto fd = socket.descriptor();

  FD_SET(fd, &fds);
  max_descriptor = std::max(max_descriptor, fd);
}

int event_loop::wait_for_events()
{
  int result = select(max_descriptor + 1, &fds, nullptr, nullptr, nullptr);
  if (result == -1) {
    throw std::runtime_error{ "Failed to select" };
  }
  return result;
}

bool event_loop::has_events(const protei::network::socket& socket) const
{
  return FD_ISSET(socket.descriptor(), &fds);
}
}