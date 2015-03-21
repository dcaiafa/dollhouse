#ifndef DOLLHOUSE_EVENT_HANDLER_H_
#define DOLLHOUSE_EVENT_HANDLER_H_

#include "event.h"

class EventHandler {
 public:
  virtual ~EventHandler() {}
  virtual void OnEvent(void* source, Event event) = 0;
};

#endif  // DOLLHOUSE_EVENT_HANDLER_H_
