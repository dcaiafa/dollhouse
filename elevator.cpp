#include "elevator.h"

#include "limits.h"

#include "stepper.h"

Elevator::Elevator(
    const ElevatorInfo* elevator_info,
    Stepper* stepper,
    Button* calibration_sensor)
    : info_(elevator_info),
      stepper_(stepper),
      calibration_sensor_(calibration_sensor),
      pending_floors_(new bool[elevator_info->floor_count]),
      direction_(1),
      state_(STATE_INIT),
      next_floor_(0),
      next_floor_pos_(0),
      current_floor_(0) {
  stepper_->SetEventHandler(this);
  calibration_sensor_->SetEventHandler(this);
  timer_.SetEventHandler(this);
};

Elevator::~Elevator() {
  delete[] pending_floors_;
}
  
void Elevator::SignalFloor(size_t floor) {
  pending_floors_[floor] = true;
  ChooseDestination();
}

void Elevator::ChooseDestination() {
  if (state_ != STATE_IDLE && state_ != STATE_MOVING) {
    return;
  }

  size_t next_floor = INVALID_FLOOR;
  if (!FindFloor(current_floor_, direction_, &next_floor)) {
    if (!FindFloor(current_floor_, -direction_, &next_floor)) {
      if (current_floor_ != 0) {
        next_floor = 0;
      }
    }
  }

  if (next_floor == INVALID_FLOOR) {
    return;
  }
  
  if (state_ == STATE_MOVING && next_floor == next_floor_) {
    return;
  }
  
  state_ = STATE_MOVING;
  next_floor_ = next_floor;
  direction_ = (next_floor_ > current_floor_) ? 1 : -1;
  next_floor_pos_ = info_->floors[next_floor];
  stepper_->SetTarget(info_->floors[next_floor]);
  DispatchEvent(this, EVENT_STARTED);
}

bool Elevator::FindFloor(size_t from_floor, int dir, size_t* next_floor) {
  for (size_t floor = from_floor + dir;
       floor < info_->floor_count;
       floor += dir) {
    if (pending_floors_[floor]) {
      *next_floor = floor;
      return true;
    }
  }
  return false;
}

void Elevator::Step() {
  if (state_ == STATE_INIT) {
    if (calibration_sensor_->IsPressed()) {
      OnCalibrationSensorTriggered();
    } else {
      stepper_->SetTarget(LONG_MIN);
      state_ = STATE_CALIBRATING;
    }
  }

  stepper_->Step();
  timer_.Step();
}

void Elevator::OnEvent(void* source, Event event) {
  switch (event) {
    case EVENT_STOPPED:
      if (state_ != STATE_MOVING ||
          next_floor_pos_ != stepper_->GetPosition()) {
        state_ = STATE_ERROR;
        return;
      }
      state_ = STATE_STOPPED;
      current_floor_ = next_floor_;
      pending_floors_[next_floor_] = false;
      timer_.SetInterval(info_->stop_interval);
      timer_.Start();
      DispatchEvent(this, EVENT_STOPPED);
      break;

    case EVENT_TICK:
      if (state_ != STATE_STOPPED) {
        state_ = STATE_ERROR;
        return;
      }
      state_ = STATE_IDLE;
      ChooseDestination();
      break;

    case EVENT_DOWN:
      if (source != calibration_sensor_.get() || state_ != STATE_CALIBRATING) {
        stepper_->Reset();
        state_ = STATE_ERROR;
        return;
      }
      OnCalibrationSensorTriggered();
      break;

    default:
      break;
  }
}

void Elevator::OnCalibrationSensorTriggered() {
  stepper_->Reset();
  state_ = STATE_IDLE;
  current_floor_ = INVALID_FLOOR;
  SignalFloor(0);
}
