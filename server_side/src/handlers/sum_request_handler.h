#ifndef PROTEI_SERVER_HANDLERS_SUM_REQUEST_HANDLER_H_
#define PROTEI_SERVER_HANDLERS_SUM_REQUEST_HANDLER_H_

#include <string>
#include <vector>

#include "request_handler.h"

namespace server_side::handlers {
class sum_request_handler : public request_handler {
 public:
  [[nodiscard]]
  std::string handle(const std::string &request) override;

 private:
  [[nodiscard]]
  static std::vector<unsigned long> fetch_numbers(const std::string &request);

  [[nodiscard]]
  static std::string make_response(const std::vector<unsigned long> &numbers);

  [[nodiscard]]
  static bool is_valid_number(const std::string &token);

  static void generate_exception(const std::string &noun);

  template<typename T>
  [[nodiscard]] static inline bool sum_will_overflow(const T &lhs, const T &rhs) {
    return lhs > std::numeric_limits<T>::max() - rhs;
  }
};
}

#endif
