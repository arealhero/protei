#ifndef PROTEI_CLIENT_ARGS_PARSER_H_
#define PROTEI_CLIENT_ARGS_PARSER_H_

#include <cstring>
#include <getopt.h>
#include <string>

#include "protocol.h"

namespace client_side {
class args_parser
{
 public:
  protocol get_protocol(int argc, char* argv[])
  {
    protocol result = protocol::unspecified;

    int c;
    while ((c = getopt_long(argc, argv, "p:", long_options, nullptr)) != -1) {
      if (c == 'p') {
        if (strcmp(optarg, "tcp") == 0) {
          result = protocol::tcp;
        } else if (strcmp(optarg, "udp") == 0) {
          result = protocol::udp;
        }
      }
    }

    return result;
  }

 private:
  option long_options[2] = { { "protocol", required_argument, nullptr, 'p' },
                             { nullptr, 0, nullptr, 0 } };
};
}

#endif
