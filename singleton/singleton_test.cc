#include "singleton.h"
#include <iostream>

class Person {
 public:

  void set_age(int age) { age_ = age; }
  void print_age() { std::cout << "age_: " << age_ << std::endl; }

 private:
  int age_;
  DECLARE_SINGLETON(Person);
};

// 必须要在类外执行
Person::Person() : age_(0) {}

int main() {
  Person::Instance()->set_age(10);
  Person::Instance()->print_age();
}