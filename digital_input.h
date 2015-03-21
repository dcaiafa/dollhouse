#ifndef DOLLHOUSE_DIGITAL_INPUT_H_
#define DOLLHOUSE_DIGITAL_INPUT_H_

class DigitalInput {
 public:
  static const unsigned long DEBOUNCE_INTERVAL = 50;

  DigitalInput(int input_pin, int initial_state)
      : pin_(input_pin),
        state_(initial_state),
        last_time_(millis()) {
    pinMode(input_pin, INPUT);
  }

  int Read() {
    return Debounce(digitalRead(pin_));
  }

 private:
  int Debounce(int state) {
    if (state != last_state_) {
      last_time_ = millis();
    } else if((millis() - last_time_) > DEBOUNCE_INTERVAL) {
      state_ = state;
    }
    last_state_ = state;
    return state_;
  }

  int pin_;
  int state_;
  int last_state_;
  unsigned long last_time_;
};

#endif  // DOLLHOUSE_DIGITAL_INPUT_H_
