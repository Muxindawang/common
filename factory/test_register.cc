#include "register.h"
#include <glog/logging.h>
#include <string>
#include <vector>

class Operator {
 public:
  Operator() = default;
  virtual ~Operator() = default;

  virtual void set_name(const std::string &name) {}

  virtual std::string name() const { return name_; }

 private:
  std::string name_ = "Operator";
};
PERCEPTION_REGISTER_REGISTERER(Operator);

class CameraOperator : public Operator {
 public:
  CameraOperator() = default;
  ~CameraOperator() = default;
  void set_name(const std::string &name) override { name_ = name; }
  virtual std::string name() const { return name_; }

 private:
  std::string name_ = "CameraOperator";
};
PERCEPTION_REGISTER_CLASS(Operator, CameraOperator);

class LidarOperator : public Operator {
 public:
  LidarOperator() = default;
  ~LidarOperator() = default;
  void set_name(const std::string &name)  override { name_ = name; }
  virtual std::string name() const { return name_; }

 private:
  std::string name_ = "LidarOperator";
};
PERCEPTION_REGISTER_CLASS(Operator, LidarOperator);

int main(int argc, char* argv) {
  google::InitGoogleLogging(argv);
  FLAGS_alsologtostderr = 1;
  FLAGS_colorlogtostderr = 1;

  Operator* operator_ptr = nullptr;
  // 从工厂中获取派生类实例,每次获取的都是一个新的实例，并不是单例
  operator_ptr = OperatorRegisterer::GetInstanceByName("CameraOperator");
  operator_ptr->set_name("new name");
  LOG(INFO) << operator_ptr->name();

  auto *pp = OperatorRegisterer::GetInstanceByName("CameraOperator");
  LOG(INFO) << pp->name();

  std::vector<Operator *> ptrs;
  // 获取所有已经注册的派生类实例
  ptrs = OperatorRegisterer::GetAllInstances();
  for (auto& p : ptrs) {
    LOG(INFO) << "ptr name: " << p->name();
  }
}