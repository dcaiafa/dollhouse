#include <iostream>

#include "elevator.h"
#include "fake_stepper.h"
#include "scoped_ptr.h"
#include "stub_event_handler.h"
#include "test_utils.h"
#include "fake_button.h"

namespace {

const unsigned long kStopInterval = 10;

const long kPositionFirstFloor = 30;
const long kPositionSecondFloor = 20;
const long kPositionThirdFloor = 10;

const size_t kFirstFloor = 0;
const size_t kSecondFloor = 1;
const size_t kThirdFloor = 2;

const long g_test_floors[] = {
  kPositionFirstFloor,
  kPositionSecondFloor,
  kPositionThirdFloor
};

const size_t kFloorCount = sizeof(g_test_floors) / sizeof(g_test_floors[0]);

const long kSlowZoneSize = 2;

const ElevatorInfo g_test_elevator_info = {
  kFloorCount,
  kSlowZoneSize,
  kStopInterval,
  g_test_floors
};

struct TestContext {
  TestContext(bool sensor_pressed = false)
      : stepper(new FakeStepper),
        sensor(new FakeButton(sensor_pressed)),
        elevator(new Elevator(&g_test_elevator_info, this->stepper,
                              this->sensor)) {
    elevator->SetEventHandler(&event_handler);
  }

  FakeStepper* stepper;
  FakeButton* sensor;
  StubEventHandler event_handler;
  scoped_ptr<Elevator> elevator;
};

#define WAIT_FOR(C, X) { \
  bool success = false; \
  for (size_t i = 0; i < 10; i++) { \
    (C)->elevator->Step(); \
    if (X) { \
      success = true; \
      break; \
    } \
  } \
  if (!success) { \
    fprintf(stderr, "Condition never became true: %s", #X); \
    return false; \
  } \
}

bool CalibrateElevator(TestContext* c) {
  if (!c->sensor->IsPressed()) {
    WAIT_FOR(c, c->stepper->target() < 0);

    c->stepper->SetPosition(-1);
    c->sensor->SetPressed(true);
  }

  WAIT_FOR(c, c->stepper->target() == kPositionFirstFloor &&
           c->stepper->GetPosition() == 0);
  ASSERT(!c->event_handler.HasEvent(c->elevator.get(), EVENT_STOPPED));
  c->sensor->SetPressed(false);

  c->stepper->SetPosition(kPositionFirstFloor);
  WAIT_FOR(
      c,c->event_handler.HasUniqueEvent(c->elevator.get(), EVENT_STOPPED));
  ASSERT(c->elevator->current_floor() == kFirstFloor);
  c->event_handler.Reset();
  AdvanceTimeForTesting(kStopInterval);
  c->stepper->set_automatic(true);

  return true;
}

}  // namespace

TEST(ElevatorCalibration) {
  TestContext c;
  return CalibrateElevator(&c);
}

TEST(ElevatorCalibrationAlreadyAtOrigin) {
  TestContext c(true);
  c.sensor->SetPressed(true);
  return CalibrateElevator(&c);
}

TEST(ElevatorMove) {
  TestContext c;
  ASSERT(CalibrateElevator(&c));

  c.elevator->SignalFloor(kThirdFloor);
  WAIT_FOR(&c, c.event_handler.HasUniqueEvent(c.elevator.get(), EVENT_STOPPED));
  ASSERT(c.elevator->current_floor() == kThirdFloor);
  c.event_handler.Reset();
  return true;
}

TEST(ElevatorMoveUpAndDown) {
  TestContext c;
  ASSERT(CalibrateElevator(&c));

  ASSERT(c.elevator->current_floor() == kFirstFloor);

  c.elevator->SignalFloor(kSecondFloor);
  c.elevator->SignalFloor(kThirdFloor);

  WAIT_FOR(&c, c.event_handler.HasUniqueEvent(c.elevator.get(), EVENT_STOPPED));
  ASSERT(c.elevator->current_floor() == kSecondFloor);
  c.event_handler.Reset();
  AdvanceTimeForTesting(kStopInterval);

  c.elevator->SignalFloor(kFirstFloor);

  WAIT_FOR(&c, c.event_handler.HasUniqueEvent(c.elevator.get(), EVENT_STOPPED));
  ASSERT(c.elevator->current_floor() == kThirdFloor);
  c.event_handler.Reset();
  AdvanceTimeForTesting(kStopInterval);

  WAIT_FOR(&c, c.event_handler.HasUniqueEvent(c.elevator.get(), EVENT_STOPPED));
  ASSERT(c.elevator->current_floor() == kFirstFloor);
  c.event_handler.Reset();
  AdvanceTimeForTesting(kStopInterval);
  return true;
}
