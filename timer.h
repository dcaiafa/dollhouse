#ifndef DOLLHOUSE_TIMER_H_
#define DOLLHOUSE_TIMER_H_

#include <stdlib.h>

#if !defined(BUILD_TEST)
#include <Arduino.h>
#else
// In tests, |micros()| will be provided by the test driver.
unsigned long micros();
#endif

class Timer {
 public:
  Timer() : event_handler_(NULL), before_(0), interval_(0), started_(false) {}

  void SetEventHandler(EventHandler* event_handler) {
    event_handler_ = event_handler;
  }

  void SetInterval(unsigned long interval) {
    interval_ = interval;
  }

  void Start() {
    started_ = true;
    before_ = micros();
  }

  void Stop() {
    started_ = false;
  }

  void Step() {
    if (!started_) {
      return;
    }

    unsigned long now = micros();
    if (now - before_ < interval_) {
      return;
    }

    started_ = false;
    if (event_handler_) {
      event_handler_->OnEvent(this, EVENT_TICK);
    }
  }

 private:
  EventHandler* event_handler_;
  unsigned long before_;
  unsigned long interval_;
  bool started_;
};

#endif  // DOLLHOUSE_TIMER_H_
