#include <iostream>

class Keyboard {
 public:
  Keyboard() = default;
  virtual ~Keyboard() = default;

  virtual void print() = 0;
};

class HPKeyboard : public Keyboard {
 public:
  HPKeyboard() = default;
  virtual ~HPKeyboard() = default;

  virtual void print() override {
    std::cout << "HP Keyboard" << std::endl;
  }
};

class DellKeyboard : public Keyboard {
 public:
  DellKeyboard() = default;
  virtual ~DellKeyboard() = default;

  virtual void print() override { 
    std::cout << "Dell Keyboard" << std::endl; 
  }
};

enum class KeyboardType {
  HP = 1,
  Dell = 2
};

class KeyboardFactory {
public:
  Keyboard* create_keyboard(const KeyboardType &type) {
    switch (type)
    {
    case KeyboardType::HP:
      return new HPKeyboard();
      break;
    case KeyboardType::Dell:
      return new DellKeyboard();
      break;
    default:
      return nullptr;
      break;
    }
  }
};

int main() {
  KeyboardFactory keyboard_factory;
  Keyboard* keyboard = keyboard_factory.create_keyboard(KeyboardType::HP);
  if (keyboard != nullptr) {
    keyboard->print();
  }
}