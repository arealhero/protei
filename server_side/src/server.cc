#include "server.h"

namespace server_side {
void server::init_tcp_socket(uint32_t address, uint16_t port) {
  tcp_master.open(SOCK_STREAM, IPPROTO_TCP);

  tcp_master.set_option(SO_REUSEADDR, 1);
  tcp_master.set_option(SO_REUSEPORT, 1);
  tcp_master.set_option(SO_NOSIGPIPE, 1);

  tcp_master.set_non_block();

  tcp_master.bind(address, port);
  tcp_master.listen();
}

void server::init_udp_socket(uint32_t address, uint16_t port) {
  udp_master.open(SOCK_DGRAM, IPPROTO_UDP);

  udp_master.set_non_block();

  udp_master.bind(address, port);
}

void server::set_handler(std::unique_ptr<handlers::request_handler> &&handler) {
  request_handler.swap(handler);
}

void server::update_event_loop() {
  loop.clear();

  loop.watch(tcp_master);
  loop.watch(udp_master);

  for (auto &slave : slaves) {
    loop.watch(slave);
  }
}

void server::wait_for_events() {
  loop.wait_for_events();
}

void server::accept_tcp_connections() {
  if (loop.has_events(tcp_master)) {
    auto slave = tcp_master.accept();
    if (slave.descriptor() != -1) {
      slave.set_option(SO_NOSIGPIPE, 1);
      slave.set_non_block();

      slaves.push_back(slave);

      std::cout << "Client connected (descriptor: " << slave.descriptor() << ")." << std::endl;
    }
  }
}

void server::handle_tcp_events() {
  for (auto it = slaves.begin(); it != slaves.end(); ++it) {
    auto &slave = *it;

    if (loop.has_events(slave)) {
      auto request = slave.receive(MESSAGE_MAX_LENGTH);

      if (request.empty()) {
        if (errno != EAGAIN) {
          std::cout << "Client disconnected (descriptor: " << slave.descriptor() << ")." << std::endl;

          slave.close();

          it = slaves.erase(it);
          --it;
        }
      } else {
        auto response = request_handler->handle(request);
        slave.send(response);
      }
    }
  }
}

void server::handle_udp_events() {
  if (loop.has_events(udp_master)) {
    auto[request, socket_address] = udp_master.receive_from(MESSAGE_MAX_LENGTH);

    auto response = request_handler->handle(request);
    udp_master.send_to(response, socket_address);
  }
}
}
