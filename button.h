#ifndef DOLLHOUSE_BUTTON_H_
#define DOLLHOUSE_BUTTON_H_

#include "event_source.h"

class Button : public EventSource {
 public:
  virtual ~Button() {}
  virtual bool IsPressed() = 0;
  virtual void Step() = 0;
};

#endif  // DOLLHOUSE_BUTTON_H_

