#ifndef PROGRAM_H
#define PROGRAM_H

#include <string>
class Program {
 public:
  virtual ~Program() = 0;
  Program(std::string name) : m_Name{name} {};
  Program(const Program &other) = delete;
  Program(const Program &&other) = delete;
  Program &operator=(const Program &other) = delete;
  Program &operator=(const Program &&other) = delete;

  virtual void setup() = 0;
  virtual void loop() = 0;
  virtual void cleanup() = 0;
  virtual const std::string &getDescription() const = 0;
  const std::string &getName() const { return m_Name; };

 private:
  std::string m_Name;
};
inline Program::~Program() {}

#endif
