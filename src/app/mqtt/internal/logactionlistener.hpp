#pragma once
#include <mqtt/iaction_listener.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <string>

// Callbacks for the success or failures of requested actions.
// This could be used to initiate further action, but here we just log the
// results to the console.
class logactionlistener : public virtual mqtt::iaction_listener {
public:
  explicit logactionlistener(const std::string &name);

private:
  std::string m_name;

  void on_failure(const mqtt::token &tok) override;
  void on_success(const mqtt::token &tok) override;
};
