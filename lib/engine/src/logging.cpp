#include "tea/logging.hpp"

#include <spdlog/sinks/stdout_color_sinks.h>

void Logger::init() {
  m_Initialized = true;

  s_Logger = spdlog::stdout_color_mt("TEA");
  s_Logger->set_pattern("[%T %z] %n [Thread %t] [%l]: %v%$");

  s_ClientLogger = spdlog::stdout_color_mt("APP");
  s_ClientLogger->set_pattern("[%T %z] %n [Thread %t] [%l]: %v%$");
}

Handle<spdlog::logger>& Logger::getEngineLogger() { return s_Logger; }
Handle<spdlog::logger>& Logger::getClientLogger() { return s_ClientLogger; }
