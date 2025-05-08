#include "template_factory.h"
#include <string>

class Base {
 public:
  virtual std::string Name() { return "Base"; }
};

class Derived : public Base {
 public:
  virtual std::string Name() { return "Derived"; }
};


class ArgConstructor {
 public:
  explicit ArgConstructor(const std::string& name) : name_(name) {}
  ArgConstructor(const std::string &name, int value) : name_(name), value_(value) {}
  std::string Name() const { return name_; }
  int Value() const { return value_; }
 private:
  std::string name_;
  int value_ = 0;
};

int main(int argc, char *argv) {
  google::InitGoogleLogging(argv);
  FLAGS_colorlogtostderr = 1;
  FLAGS_alsologtostderr = 1;
  {
    Factory<std::string, Base> factory;
    auto ret = factory.Register("derived_class",
                                []() -> Base* { return new Derived(); });
    auto derived_ptr = factory.CreateObject("derived_class");
    if (derived_ptr != nullptr) {
      LOG(INFO) << derived_ptr->Name();
    } else {
      LOG(WARNING) << "derived_ptr is nullptr";
    }
  }

  {
    Factory<std::string, ArgConstructor, ArgConstructor* (*)(const std::string&)> factory;
    auto ret = factory.Register(
      "arg_1", [](const std::string& arg) { return new ArgConstructor(arg); });
    auto ptr = factory.CreateObject("arg_1", "name_1");
    if (nullptr != ptr) {
      LOG(INFO) << ptr->Name();
      LOG(INFO) << ptr->Value();
    }
  }

  {
    Factory<std::string, ArgConstructor, ArgConstructor* (*)(const std::string&, int)> factory2;
    auto ret2 = factory2.Register("arg_2", [](const std::string &arg, int value) {return new ArgConstructor(arg, value); });
    auto ptr2 = factory2.CreateObject("arg_2", "name_2", 2);
    if (nullptr != ptr2) {
      LOG(INFO) << ptr2->Name();
      LOG(INFO) << ptr2->Value();
    }
  }
}