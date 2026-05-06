#include <Arduino.h>
#include <AccelStepper.h>

const int dir_pin = 27; 
const int pul_pin = 25; 

AccelStepper slider(AccelStepper::DRIVER, pul_pin, dir_pin);

const float STEPS_PER_MM = 40.0; 
float fret_positions_mm[] = {0.0, 35.156, 68.12, 99.318, 128.723, 156.528, 182.783, 207.537, 230.791, 252.645, 273.199, 292.553, 311.056, 328.559, 345.162, 360.765, 375.267, 388.920, 401.772, 414.074, 428.577};

int sequence[] = {0,1, 2, 3,4,5,6,7,8,9,10,11, 12}; 
int current_move = 0;
int total_moves = 13;
bool test_finished = false;

void setup() {
    Serial.begin(115200);
    slider.setCurrentPosition(0);
    slider.setMaxSpeed(4000.0); 
    slider.setAcceleration(2500.0); 

}

void loop() {
    slider.run(); 

    if (test_finished) {
        return; 
    }

    static unsigned long wait_timer = 0;
    static bool is_waiting = false;

    if (slider.distanceToGo() == 0) {
        
        if (!is_waiting) {
            wait_timer = millis();
            is_waiting = true;
        }

        if (is_waiting && millis() - wait_timer >= 1000) {
            is_waiting = false;
            
            if (current_move >= total_moves) {
                test_finished = true;
            } else {
                int target_fret = sequence[current_move];
                long target_position = fret_positions_mm[target_fret] * STEPS_PER_MM;                
                slider.moveTo(target_position); 
                current_move++;
            }
        }
    }
}