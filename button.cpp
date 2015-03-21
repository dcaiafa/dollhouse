#include "button.h"

#include <Arduino.h>

#include "event_handler.h"
#include "digital_input.h"

class ButtonImpl : public Button {
 public:
  ButtonImpl(int pin);
  virtual ~ButtonImpl();

  virtual bool IsPressed();
  virtual void Step();

 private:
  DigitalInput input_;
  int state_;
  EventHandler* event_handler_;
};

ButtonImpl::ButtonImpl(int pin)
    : input_(pin, LOW),
      state_(LOW),
      event_handler_(NULL) {
}

ButtonImpl::~ButtonImpl() {}

bool ButtonImpl::IsPressed() {
  return state_ == HIGH;
}

void ButtonImpl::Step() {
  int new_state = input_.Read();
  if (new_state == state_) {
    return;
  }
  state_ = new_state;
  DispatchEvent(this, (new_state == HIGH) ?  EVENT_DOWN : EVENT_UP);
}
