#include "logactionlistener.hpp"
#include <mqtt/delivery_token.h>
#include <iostream>
#include <spdlog/spdlog.h>

logactionlistener::logactionlistener(const std::string &name) : m_name(name) {}

void logactionlistener::on_failure(const mqtt::token &tok) {
  std::stringstream ss;
  ss << m_name << " failure";
  if (tok.get_message_id() != 0) {
    ss << " for token: [" << tok.get_message_id() << "]" << std::endl;
  }
  ss << " return code: " << tok.get_return_code() << std::endl;
  spdlog::error(ss.str());
}

void logactionlistener::on_success(const mqtt::token &tok) {
  std::stringstream ss;
  ss << m_name << " success";
  if (tok.get_message_id() != 0) {
    ss << " for token: [" << tok.get_message_id() << "]" << std::endl;
  }
  auto top = tok.get_topics();
  if (top && !top->empty()) {
    ss << "\ttoken topic: '" << (*top)[0] << "', ..." << std::endl;
  }
  ss << std::endl;
  spdlog::debug(ss.str());
}
