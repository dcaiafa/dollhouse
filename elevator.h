#ifndef DOLLHOUSE_ELEVATOR_H_
#define DOLLHOUSE_ELEVATOR_H_

#include <stdlib.h>

#include "button.h"
#include "event_handler.h"
#include "event_source.h"
#include "scoped_ptr.h"
#include "timer.h"

class Stepper;

const size_t INVALID_FLOOR = (size_t)-1;

struct ElevatorInfo {
  size_t floor_count;
  long slow_zone_size;
  unsigned long stop_interval;
  const long* floors;
};

class Elevator : public EventSource, public EventHandler {
 public:
  // |elevator_info| must outlive this object.
  // |stepper| passed.
  // |calibration_sensor| passed.
  Elevator(
      const ElevatorInfo* elevator_info,
      Stepper* stepper,
      Button* calibration_sensor);
  virtual ~Elevator();

  void SignalFloor(size_t floor);

  void Step();

  // EventHandler implementation.
  virtual void OnEvent(void* source, Event event);
  
  size_t current_floor() const {
    return current_floor_;
  }

 private:
  enum State {
    STATE_INIT,
    STATE_CALIBRATING,
    STATE_IDLE,
    STATE_STOPPED,
    STATE_MOVING,
    STATE_ERROR
  };

  void ChooseDestination();
  bool FindFloor(size_t from_floor, int dir, size_t* next_floor);
  void OnCalibrationSensorTriggered();

  const ElevatorInfo* info_;
  scoped_ptr<Stepper> stepper_;
  scoped_ptr<Button> calibration_sensor_;
  bool* pending_floors_;
  int direction_;
  State state_;
  size_t next_floor_;
  long next_floor_pos_;
  size_t current_floor_;
  Timer timer_;
};

#endif  // DOLLHOUSE_ELEVATOR_H_
