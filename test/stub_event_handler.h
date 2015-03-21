#ifndef DOLLHOUSE_TEST_STUB_EVENT_HANDLER_H_
#define DOLLHOUSE_TEST_STUB_EVENT_HANDLER_H_

#include "event_handler.h"
#include "linked_list.h"
#include "utils.h"

class StubEventHandler : public EventHandler {
 public:
  StubEventHandler() : event_count_(0) {}

  virtual ~StubEventHandler() {
    Reset();
  }

  virtual void OnEvent(void* source, Event event) {
    EventInfo* event_info = new EventInfo(source, event);
    events_.PushBack(&event_info->link_node);
    event_count_++;
  }

  size_t event_count() {
    return event_count_;
  }

  bool HasEvent(void* source, Event event) {
    return CountEvent(source, event) != 0;
  }
  
  bool HasUniqueEvent(void* source, Event event) {
    return CountEvent(source, event) == 1;
  }
  
  size_t CountEvent(void* source, Event event) {
    size_t count = 0;
    for (LinkNode* n = events_.head(); n != events_.end(); n = n->next) {
      EventInfo* event_info = CONTAINER_OF(n, EventInfo, link_node);
      if (event_info->source == source && event_info->event == event) {
        count++;
      }
    }
    return count;
  }

  void Reset() {
    while(!events_.empty()) {
      LinkNode* node = events_.PopFront();
      EventInfo* event_info = CONTAINER_OF(node, EventInfo, link_node);
      delete event_info;
    }
  }

 private:
  struct EventInfo {
    EventInfo(void* source, Event event) : source(source), event(event) {}

    LinkNode link_node;
    void* source;
    Event event;
  };

  LinkedList events_;
  size_t event_count_;
};

#endif  // DOLLHOUSE_TEST_STUB_EVENT_HANDLER_H_

