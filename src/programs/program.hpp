#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
#include "tea/types.hpp"
#include "error.hpp"
class Program {
 public:
  virtual ~Program() = 0;
  Program(std::string name, std::string description)
      : m_Name{name}, m_Description{description} {};
  Program(const Program &other) = delete;
  Program(const Program &&other) = delete;
  Program &operator=(const Program &other) = delete;
  Program &operator=(const Program &&other) = delete;

  virtual Res<void, Error> setup() = 0;
  virtual void loop() = 0;
  virtual void cleanup() = 0;
  [[nodiscard]] const std::string &getDescription() const {
    return m_Description;
  };
  [[nodiscard]] const std::string &getName() const { return m_Name; };

 private:
  std::string m_Name;
  std::string m_Description;
};
inline Program::~Program() {}

#endif
