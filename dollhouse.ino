#include "event.h"
#include "event_handler.h"
#include "timer.h"
#include "stepper.h"
#include "digital_input.h"

const int ELEVATOR_MOTOR_DIR_PIN = 2;
const int ELEVATOR_MOTOR_STEP_PIN = 3;

void setup() {
  Serial.begin(9600);
  Serial.println("init");
}

void loop() {
}
