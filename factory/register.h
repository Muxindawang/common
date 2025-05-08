#pragma once

#include <map>
#include <string>
#include <vector>
#include <glog/logging.h>
class Any {
 public:
  Any() : content_(nullptr) {}

  // 模板化的构造函数，接受任意类型的值并存储
  template <typename ValueType>
  explicit Any(const ValueType& value) : content_(new Holder<ValueType>(value)) {}

  Any(const Any& other) : content_(other.content_ ? other.content_->Clone() : nullptr) {}

  ~Any() { delete content_; }

  template <typename ValueType>
  ValueType *AnyCast() {
    return content_ ? &(static_cast<Holder<ValueType> *>(content_)->held_) : nullptr;
  }

 private:
  class PlaceHolder {
   public:
    virtual ~PlaceHolder() {}
    virtual PlaceHolder *Clone() const = 0;
  };

  template <typename ValueType>
  class Holder : public PlaceHolder {
   public:
    explicit Holder(const ValueType& value) : held_(value) {}
    virtual ~Holder() {}
    virtual PlaceHolder *Clone() const { return new Holder(held_); }

    ValueType held_;
  };

  PlaceHolder *content_;    // 用于存储任意类型的数据
};


// 抽象基类，定义了工厂接口
class ObjectFactory {
 public:
  ObjectFactory() {}
  virtual ~ObjectFactory() {}
  // 创建对象实例
  virtual Any NewInstance() { return Any(); }
  ObjectFactory(const ObjectFactory &) = delete;
  ObjectFactory &operator=(const ObjectFactory &) = delete;

 private:
};

// 使用 BaseClassMap 和 FactoryMap，将基类名称和派生类名称映射到对应的工厂对象
typedef std::map<std::string, ObjectFactory *> FactoryMap;
typedef std::map<std::string, FactoryMap> BaseClassMap;
// 全局函数 GlobalFactoryMap 返回一个全局的工厂映射表。
BaseClassMap &GlobalFactoryMap();

// 获取某个基类下所有已注册的派生类名称。
bool GetRegisteredClasses(
    const std::string &base_class_name,     // 基类名称
    std::vector<std::string> *registered_derived_classes_names);    // 存储派生类名称的容器

// 功能：生成一个注册器类，用于管理基类 base_class 的派生类实例。
// 生成的类：
// base_class##Registerer：生成的注册器类名称，通过 ## 操作符将 base_class 和 Registerer 拼接。
// 静态方法：
// GetInstanceByName：通过派生类名称获取实例。
// GetAllInstances：获取所有已注册的派生类实例。
// GetUniqInstanceName：获取唯一实例的名称，要求注册表中只有一个派生类实例。
// GetUniqInstance：获取唯一实例，要求注册表中只有一个派生类实例。
// IsValid：检查派生类名称是否有效。
#define PERCEPTION_REGISTER_REGISTERER(base_class)                    \
  class base_class##Registerer {                                      \
   public:                                                            \
    static base_class *GetInstanceByName(const ::std::string &name) { \
      FactoryMap &map = GlobalFactoryMap()[#base_class];              \
      FactoryMap::iterator iter = map.find(name);                     \
      if (iter == map.end()) {                                        \
        for (auto c : map) {                                          \
          LOG(ERROR) << "Instance:" << c.first;                       \
        }                                                             \
        LOG(ERROR) << "Get instance " << name << " failed.";          \
        return nullptr;                                               \
      }                                                               \
      Any object = iter->second->NewInstance();                       \
      return *(object.AnyCast<base_class *>());                       \
    }                                                                 \
    static std::vector<base_class *> GetAllInstances() {              \
      std::vector<base_class *> instances;                            \
      FactoryMap &map = GlobalFactoryMap()[#base_class];              \
      instances.reserve(map.size());                                  \
      for (auto item : map) {                                         \
        Any object = item.second->NewInstance();                      \
        instances.push_back(*(object.AnyCast<base_class *>()));       \
      }                                                               \
      return instances;                                               \
    }                                                                 \
    static const ::std::string GetUniqInstanceName() {                \
      FactoryMap &map = GlobalFactoryMap()[#base_class];              \
      CHECK_EQ(map.size(), 1U) << map.size();                         \
      return map.begin()->first;                                      \
    }                                                                 \
    static base_class *GetUniqInstance() {                            \
      FactoryMap &map = GlobalFactoryMap()[#base_class];              \
      CHECK_EQ(map.size(), 1U) << map.size();                         \
      Any object = map.begin()->second->NewInstance();                \
      return *(object.AnyCast<base_class *>());                       \
    }                                                                 \
    static bool IsValid(const ::std::string &name) {                  \
      FactoryMap &map = GlobalFactoryMap()[#base_class];              \
      return map.find(name) != map.end();                             \
    }                                                                 \
  };

// 功能：注册派生类到工厂映射表中。
// 生成的类：
// ObjectFactory##name：生成的工厂类名称，通过 ## 操作符将 ObjectFactory 和 name 拼接。
// 静态方法：
// NewInstance：创建派生类的实例。
// 自动注册：
// 使用 __attribute__((constructor)) 确保在程序启动时自动调用注册函数。
// 将派生类的工厂类注册到全局工厂映射表中。
#define PERCEPTION_REGISTER_CLASS(clazz, name)                                \
  namespace {                                                                 \
  class ObjectFactory##name : public ObjectFactory {                          \
   public:                                                                    \
    virtual ~ObjectFactory##name() {}                                         \
    virtual Any NewInstance() { return Any(new name()); }                     \
  };                                                                          \
  __attribute__((constructor)) void RegisterFactory##name() {                 \
    FactoryMap &map = GlobalFactoryMap()[#clazz];                             \
    if (map.find(#name) == map.end()) map[#name] = new ObjectFactory##name(); \
  }                                                                           \
  }  // namespace