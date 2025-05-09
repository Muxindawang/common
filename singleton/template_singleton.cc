#include <iostream>
#include <memory>
#include <mutex>

template <typename T>
class Singleton {
 public:
  static std::unique_ptr<T>& GetInstance() {
    static std::once_flag s_flag;
    std::call_once(s_flag, [&]() { instance_.reset(new T()); });

    return instance_;
  }

  ~Singleton() = default;

  void PrintAddress() const { std::cout << this << std::endl; }

 private:
  Singleton() = default;

  Singleton(const Singleton&) = delete;
  Singleton& operator=(const Singleton&) = delete;

 private:
  static std::unique_ptr<T> instance_;
};
template <typename T>
std::unique_ptr<T> Singleton<T>::instance_;

class Person {
 public:
  void init() { age_ = 8; }
  int age_;
};

int main() {
  Singleton<Person>::GetInstance()->init();
  std::cout << Singleton<Person>::GetInstance()->age_ << std::endl;
  return 0;
}
