const int ELEVATOR_MOTOR_DIR_PIN = 2;
const int ELEVATOR_MOTOR_STEP_PIN = 3;

const unsigned long MOTOR_STEP_INTERVAL = 400;

enum Event {
  EVENT_DOWN = 0,
  EVENT_UP = 1,
  EVENT_STOPPED = 2
};

class EventHandler {
 public:
  virtual ~EventHandler() {}
  virtual void OnEvent(void* source, Event event) = 0;
};

class IStepper {
 public:
  virtual void SetEventHandler(EventHandler* event_handler) = 0;
  virtual void SetTarget(long target) = 0;
  virtual void Step() = 0;
};

class Stepper : public IStepper {
 public:
  Stepper(int dir_pin, int step_pin) 
    : dir_pin_(dir_pin),
      step_pin_(step_pin),
      pos_(0),
      target_(0),
      dir_(0),
      time_last_step_(0),
      event_handler_(NULL) {
    pinMode(dir_pin_, OUTPUT);
    pinMode(step_pin_, OUTPUT);
  }
  
  virtual void SetEventHandler(EventHandler* event_handler) {
    event_handler_ = event_handler;  
  }
  
  virtual void SetTarget(long target) {
    target_ = target;
    time_last_step_ = micros() - MOTOR_STEP_INTERVAL;  
  }

  virtual void Step() {
    if (target_ == pos_) {
      if (dir_ != 0) {
        if (event_handler_) {
          event_handler_->OnEvent(this, EVENT_STOPPED);
        }  
      }
      return; 
    }
      
    unsigned long now = micros();
    if (now - time_last_step_ < MOTOR_STEP_INTERVAL)
      return;
    
    int target_dir = target_ < pos_ ? -1 : 1;
    if (target_dir != dir_) {
      dir_ = target_dir;
      digitalWrite(dir_pin_, dir_ < 0 ? LOW : HIGH);
    }
    
    digitalWrite(step_pin_, LOW);
    digitalWrite(step_pin_, HIGH);
    
    pos_ += dir_;
    time_last_step_ = now;
  }
  
 private:
  int dir_pin_;
  int step_pin_;
  long pos_;
  int dir_;
  long target_;
  unsigned long time_last_step_;
  EventHandler* event_handler_;
};

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

class Button {
 public:
  Button(int pin);
  
  void SetEventHandler(EventHandler* event_handler);
  void Step();

 private:
  DigitalInput input_;
  int state_;
  EventHandler* event_handler_;
};

Button::Button(int pin) 
    : input_(pin, LOW), 
      state_(LOW),
      event_handler_(NULL) {
}

void Button::SetEventHandler(EventHandler* event_handler) {
  event_handler_ = event_handler;  
}

void Button::Step() {
  int new_state = input_.Read();
  if (new_state == state_) {
    return;
  }
  state_ = new_state;
  if (event_handler_) {
    Event event = (new_state == HIGH) ?
        EVENT_DOWN :
        EVENT_UP;  
    event_handler_->OnEvent(this, event);
  }
}

struct ElevatorInfo {
  int floor_count;
  unsigned long floor_positions[];
};

class Elevator : public EventHandler {
 public:
  Elevator(int dir_pin, int );
  
  void Step() {
  }
  
 private:
  Stepper stepper_;
};

class TestHandler : public EventHandler {
 public:
  virtual void OnEvent(void* target, Event event) {
    Serial.println(event);  
  }
};

Stepper* g_elevator_motor;
Button* g_button;
TestHandler* g_handler;

void setup() {
  Serial.begin(9600);
  
  Serial.println("init");
  
  g_button = new Button(7);
  g_handler = new TestHandler();
  g_button->SetEventHandler(g_handler);
  
  
  
}

void loop() {
  g_button->Step();
  
  
}



