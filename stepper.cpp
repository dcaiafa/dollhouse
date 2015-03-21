#include "stepper.h"

#include <stdlib.h>
#include <Arduino.h>

#include "event_handler.h"

const unsigned long MOTOR_STEP_INTERVAL = 400;

class StepperImpl : public Stepper {
 public:
  StepperImpl(int dir_pin, int step_pin);

  // Stepper implementation.
  virtual void SetTarget(long target);
  virtual long GetPosition();
  virtual void Step();
  virtual void Reset();

 private:
  int dir_pin_;
  int step_pin_;
  long pos_;
  int dir_;
  long target_;
  unsigned long time_last_step_;
};

StepperImpl::StepperImpl(int dir_pin, int step_pin)
  : dir_pin_(dir_pin),
    step_pin_(step_pin),
    pos_(0),
    target_(0),
    dir_(0),
    time_last_step_(0) {
  pinMode(dir_pin_, OUTPUT);
  pinMode(step_pin_, OUTPUT);
}

void StepperImpl::SetTarget(long target) {
  target_ = target;
  time_last_step_ = micros() - MOTOR_STEP_INTERVAL;
}

long StepperImpl::GetPosition() {
  return pos_;
}

void StepperImpl::Step() {
  if (target_ == pos_) {
    if (dir_ != 0) {
      DispatchEvent(this, EVENT_STOPPED);
    }
    return;
  }

  unsigned long now = micros();
  if (now - time_last_step_ < MOTOR_STEP_INTERVAL)
    return;

  int target_dir = target_ < pos_ ? -1 : 1;
  if (target_dir != dir_) {
    dir_ = target_dir;
    digitalWrite(dir_pin_, dir_ < 0 ? LOW : HIGH);
  }

  digitalWrite(step_pin_, LOW);
  digitalWrite(step_pin_, HIGH);

  pos_ += dir_;
  time_last_step_ = now;
}

void StepperImpl::Reset() {
  pos_ = 0;
  target_ = 0;
}
