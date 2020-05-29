#ifndef PROTEI_SERVER_SERVER_H_
#define PROTEI_SERVER_SERVER_H_

#include <list>
#include <cstdint>
#include <memory>
#include <iostream>

#include <protei/network/socket.hpp>

#include "event_loop.h"
#include "handlers/request_handler.h"

#include "../../config.h"

namespace server_side {
class server {
 public:
  void init_tcp_socket(uint32_t address, uint16_t port);
  void init_udp_socket(uint32_t address, uint16_t port);

  void set_handler(std::unique_ptr<handlers::request_handler>&& handler);

  void update_event_loop();
  void wait_for_events();

  void accept_tcp_connections();

  void handle_tcp_events();
  void handle_udp_events();

 private:
  std::list<protei::network::socket> slaves;

  event_loop loop;

  protei::network::socket tcp_master;
  protei::network::socket udp_master;

  std::unique_ptr<handlers::request_handler> request_handler;
};
}

#endif
