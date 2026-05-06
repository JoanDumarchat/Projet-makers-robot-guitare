#include <Arduino.h>
#include <AccelStepper.h>


const int dir_pin = 27; 
const int pul_pin = 25; 
AccelStepper slider(AccelStepper::DRIVER, pul_pin, dir_pin);

const float STEPS_PER_MM = 40.0; 
float fret_positions_mm[] = {0.0, 35.156, 68.12, 99.318, 128.723, 156.528, 182.783, 207.537, 230.791, 252.645, 273.199, 292.553, 311.056, 328.559, 345.162, 360.765, 375.267, 388.920, 401.772, 414.074, 428.577};

const int r_en_pin = 19; 
const int l_en_pin = 21; 
const int r_pwm_pin = 22; 
const int l_pwm_pin = 23; 

const int ALPHA_PLUCK = 200;   
const int PLUCK_DURATION = 80; 
bool strum_direction = true;



struct Note {
  int fret;
  unsigned long duration;
};

Note melody[] = {
  {0, 1500}, 
  {3, 1000}, 
  {5, 1500}, 
  
  {0, 1500}, 
  {3, 1000}, 
  {6, 600},  
  {5, 2000}, 
  
  {0, 1500}, 
  {3, 1000}, 
  {5, 1500}, 
  {3, 1000}, 
  {0, 3000}  
};

int note_index = 0;
int melody_size = 12; 
unsigned long note_timer = 0;


enum RobotState { PLUCKING, MOVING_AND_WAITING, FINISHED };
RobotState currentState = PLUCKING;

bool move_started = false;

void setup() {
  Serial.begin(115200);

  slider.setCurrentPosition(0);
  slider.setMaxSpeed(38000.0); 
  slider.setAcceleration(36000.0);

  pinMode(r_en_pin, OUTPUT);
  pinMode(l_en_pin, OUTPUT);
  pinMode(r_pwm_pin, OUTPUT);
  pinMode(l_pwm_pin, OUTPUT);

  digitalWrite(r_en_pin, HIGH);
  digitalWrite(l_en_pin, HIGH);
  analogWrite(r_pwm_pin, 0); 
  analogWrite(l_pwm_pin, 0); 

  delay(3000); 
}

void loop() {
  slider.run(); 

  uint32_t now_millis = millis();

  switch (currentState) {
    case PLUCKING: {
      Serial.print("Gratte Frette "); 
      Serial.println(melody[note_index].fret);

      if (strum_direction) {
        analogWrite(l_pwm_pin, 0);
        analogWrite(r_pwm_pin, ALPHA_PLUCK);
      } else {
        analogWrite(r_pwm_pin, 0);
        analogWrite(l_pwm_pin, ALPHA_PLUCK);
      }
      
      strum_direction = !strum_direction;
      note_timer = now_millis;
      
      move_started = false;
      
      currentState = MOVING_AND_WAITING;
      break;
    } 
    case MOVING_AND_WAITING: { 
      
      if (now_millis - note_timer > PLUCK_DURATION) {
        analogWrite(r_pwm_pin, 0);
        analogWrite(l_pwm_pin, 0);
      }

      int next_index = note_index + 1;

      if (next_index >= melody_size) {
        if (now_millis - note_timer >= melody[note_index].duration) {
          currentState = FINISHED; 
        }
        break; 
      }

      if (!move_started) {
        int target_fret = melody[next_index].fret;
        long target_position = fret_positions_mm[target_fret] * STEPS_PER_MM;
        slider.moveTo(target_position); 
        move_started = true;
      }

      if (move_started && (now_millis - note_timer >= melody[note_index].duration) && slider.distanceToGo() == 0) {
        note_index++; 
        currentState = PLUCKING; 
      }
      break;
    }
    case FINISHED: {
      break;
    }
  }
}