#ifndef LOGGING_H
#define LOGGING_H

#include <spdlog/spdlog.h>
#include "tea/types.hpp"

class Logger {
 public:
  static void init();
  static Handle<spdlog::logger>& getEngineLogger();
  static Handle<spdlog::logger>& getClientLogger();

 private:
  static bool m_Initialized;
  static Handle<spdlog::logger> s_Logger;
  static Handle<spdlog::logger> s_ClientLogger;
};

#define TEA_CORE_TRACE(...) Logger::getEngineLogger()->trace(__VA_ARGS__)
#define TEA_CORE_INFO(...) Logger::getEngineLogger()->info(__VA_ARGS__)
#define TEA_CORE_WARN(...) Logger::getEngineLogger()->warn(__VA_ARGS__)
#define TEA_CORE_ERROR(...) Logger::getEngineLogger()->error(__VA_ARGS__)
#define TEA_CORE_CRITICAL(...) Logger::getEngineLogger()->critical(__VA_ARGS__)

// Client log macros
#define TEA_TRACE(...) Logger::getClientLogger()->trace(__VA_ARGS__)
#define TEA_INFO(...) Logger::getClientLogger()->info(__VA_ARGS__)
#define TEA_WARN(...) Logger::getClientLogger()->warn(__VA_ARGS__)
#define TEA_ERROR(...) Logger::getClientLogger()->error(__VA_ARGS__)
#define TEA_CRITICAL(...) Logger::getClientLogger()->critical(__VA_ARGS__)

#endif
