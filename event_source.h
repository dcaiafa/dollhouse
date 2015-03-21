#ifndef DOLLHOUSE_EVENT_SOURCE_H_
#define DOLLHOUSE_EVENT_SOURCE_H_

#include <stddef.h>

#include "event.h"

class EventHandler;

class EventSource {
 public:
  EventSource();
  virtual ~EventSource();

  void SetEventHandler(EventHandler* event_handler);

 protected:
  void DispatchEvent(void* source, Event event);

 private:
  EventHandler* event_handler_;
};

#endif  // DOLLHOUSE_EVENT_SOURCE_H_

