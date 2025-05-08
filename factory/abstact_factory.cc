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

class KeyboardFactory {
 public:
  KeyboardFactory() = default;
  virtual ~KeyboardFactory() = default;

  virtual Keyboard* create_keyboard() = 0;
};

class HPKeyboardFactory : public KeyboardFactory {
 public:
  HPKeyboardFactory() = default;
  virtual ~HPKeyboardFactory() = default;

  virtual Keyboard* create_keyboard() override { return new HPKeyboard(); }
};

class DellKeyboardFactory : public KeyboardFactory {
 public:
 DellKeyboardFactory() = default;
  virtual ~DellKeyboardFactory() = default;
  virtual Keyboard* create_keyboard() override { return new DellKeyboard(); }
};

int main() {
  KeyboardFactory* keyboard_factory = new HPKeyboardFactory();
  Keyboard* keyboard = keyboard_factory->create_keyboard();
  if (keyboard != nullptr) {
    keyboard->print();
  }
}