#include "event_source.h"

#include "event_handler.h"

EventSource::EventSource() : event_handler_(NULL) {}

EventSource::~EventSource() {}

void EventSource::SetEventHandler(EventHandler* event_handler) {
  event_handler_ = event_handler;
}

void EventSource::DispatchEvent(void* source, Event event) {
  if (event_handler_) {
    event_handler_->OnEvent(source, event);
  }
}
