#include "sum_request_handler.h"

#include <algorithm>
#include <limits>
#include <sstream>
#include <string>
#include <vector>

namespace server_side::handlers {
std::string sum_request_handler::handle(const std::string& request)
{
  try {
    std::vector<unsigned long> numbers = fetch_numbers(request);
    std::sort(numbers.begin(), numbers.end());
    return make_response(numbers);
  } catch (const std::runtime_error& e) {
    return e.what();
  }
}

std::vector<unsigned long> sum_request_handler::fetch_numbers(
  const std::string& request)
{
  std::vector<unsigned long> numbers;

  size_t pos;
  size_t previous = 0;

  const std::string delimiter = " ";
  std::string token;
  do {
    pos = request.find(delimiter, previous);
    token = request.substr(previous, pos - previous);

    try {
      if (is_valid_number(token)) {
        numbers.emplace_back(std::stoul(token));
      }
    } catch (const std::out_of_range& e) {
      generate_exception("numbers");
    }

    previous = pos + 1;
  } while (pos != std::string::npos);

  return numbers;
}

std::string sum_request_handler::make_response(
  const std::vector<unsigned long>& numbers)
{
  std::stringstream response;

  if (!numbers.empty()) {
    response << numbers[0];
    for (size_t i = 1; i < numbers.size(); ++i) {
      response << ' ' << numbers[i];
    }
  }

  response << '\n';

  unsigned long sum = 0;
  for (auto& number : numbers) {
    if (sum_will_overflow(sum, number)) {
      generate_exception("sum");
    }

    sum += number;
  }

  response << sum;

  return response.str();
}

bool sum_request_handler::is_valid_number(const std::string& token)
{
  if (token.empty()) {
    return false;
  }

  return std::all_of(token.begin(), token.end(), [](unsigned char c) {
    return std::isdigit(c);
  });
}

void sum_request_handler::generate_exception(const std::string& noun)
{
  auto min = std::numeric_limits<unsigned long>::min();
  auto max = std::numeric_limits<unsigned long>::max();

  auto message =
    std::string{ "Error: " + noun + " must be between " + std::to_string(min) +
                 " and " + std::to_string(max) };

  throw std::runtime_error{ message };
}
}