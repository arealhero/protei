#ifndef PROTEI_SERVER_HANDLERS_REQUEST_HANDLER_H_
#define PROTEI_SERVER_HANDLERS_REQUEST_HANDLER_H_

#include <string>

namespace server_side::handlers {
class request_handler {
 public:
  virtual ~request_handler() = default;

  [[nodiscard]]
  virtual std::string handle(const std::string& request) = 0;
};
}

#endif
