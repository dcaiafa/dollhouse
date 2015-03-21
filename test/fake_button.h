#ifndef DOLLHOUSE_TEST_FAKE_BUTTON_H_
#define DOLLHOUSE_TEST_FAKE_BUTTON_H_

#include "button.h"

class FakeButton : public Button {
 public:
  FakeButton(bool pressed = false) : pressed_(pressed) {}

  virtual bool IsPressed() {
    return pressed_;
  }

  virtual void Step() {}

  void SetPressed(bool pressed) {
    if (pressed == pressed_) {
      return;
    }
    pressed_ = pressed;
    DispatchEvent(this, (pressed_ ? EVENT_DOWN : EVENT_UP));
  }

 private:
  bool pressed_;
};

#endif  // DOLLHOUSE_TEST_FAKE_BUTTON_H_

