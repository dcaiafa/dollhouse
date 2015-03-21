#ifndef DOLLHOUSE_STEPPER_H_
#define DOLLHOUSE_STEPPER_H_

#include "event_source.h"

class Stepper : public EventSource {
 public:
  virtual ~Stepper() {}
  virtual void SetTarget(long target) = 0;
  virtual long GetPosition() = 0;
  virtual void Reset() = 0;
  virtual void Step() = 0;
};

#endif  // DOLLHOUSE_STEPPER_H_
