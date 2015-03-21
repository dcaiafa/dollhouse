#ifndef DOLLHOUSE_TEST_FAKE_STEPPER_H_
#define DOLLHOUSE_TEST_FAKE_STEPPER_H_

#include "stepper.h"

class FakeStepper : public Stepper {
 public:
  FakeStepper() 
      : automatic_(false),
        target_(0),
        position_(0) {}

  virtual void SetTarget(long target) {
    target_ = target;
  }

  virtual long GetPosition() {
    return position_;
  }

  virtual void Reset() {
    target_ = 0;
    position_ = 0;
  }

  virtual void Step() {
    if (automatic_ && target_ != position_) {
      SetPosition(target_);
    }
  }

  long target() {
    return target_;
  }

  void SetPosition(long position) {
    if (position == position_) {
      return;
    }
    position_ = position;
    if (position_ == target_) {
      DispatchEvent(this, EVENT_STOPPED);
    }
  }

  bool automatic() {
    return automatic_;
  }

  void set_automatic(bool automatic) {
    automatic_ = automatic;
  }

 private:
  bool automatic_;
  long target_;
  long position_;
};

#endif  // DOLLHOUSE_TEST_FAKE_STEPPER_H_
