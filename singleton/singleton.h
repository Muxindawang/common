#pragma once
#include <memory>
#include <mutex>
#include <utility>

// 检查一个类型是否具有特定的成员函数
// 如果 Class 有这个成员函数 fun，decltype(&Class::func) 是有效的，因此 Test 返回 true。
#define DEFINE_TYPE_TRAIT(name, func)                     \
  template <typename T>                                   \
  struct name {                                           \
    template <typename Class>                             \
    static constexpr bool Test(decltype(&Class::func)*) { \
      return true;                                        \
    }                                                     \
    template <typename>                                   \
    static constexpr bool Test(...) {                     \
      return false;                                       \
    }                                                     \
                                                          \
    static constexpr bool value = Test<T>(nullptr);       \
  };                                                      \
                                                          \
  template <typename T>                                   \
  constexpr bool name<T>::value;

DEFINE_TYPE_TRAIT(HasShutdown, Shutdown)
  template <typename T>
  typename std::enable_if<HasShutdown<T>::value>::type CallShutdown(T *instance) {
      instance->Shutdown();
  }

template <typename T>
typename std::enable_if<!HasShutdown<T>::value>::type CallShutdown(
        T *instance) {
    (void)instance;
}

#define DISALLOW_COPY_AND_ASSIGN(class_name) \
  class_name(const class_name&) = delete;    \
  class_name& operator=(const class_name&) = delete;

#define DECLARE_SINGLETON(class_name)                                        \
 public:                                                                     \
  static class_name* Instance(bool create_if_needed = true) {                \
    static class_name* instance = nullptr;                                   \
    if (instance == nullptr && create_if_needed) {                           \
      static std::once_flag flag;                                            \
      std::call_once(flag,                                                   \
                     [&]() { instance = new (std::nothrow) class_name(); }); \
    }                                                                        \
    return instance;                                                         \
  }                                                                          \
                                                                             \
  static void cleanup() {                                                    \
    auto instance = Instance();                                              \
    if (instance != nullptr) {                                               \
      CallShutdown(instance);                                                \
    }                                                                        \
  }                                                                          \
                                                                             \
 private:                                                                    \
  class_name();                                                              \
  DISALLOW_COPY_AND_ASSIGN(class_name)