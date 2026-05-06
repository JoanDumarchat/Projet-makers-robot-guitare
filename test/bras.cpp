#include <Arduino.h>

const int r_en_pin = 19; 
const int l_en_pin = 21; 
const int r_pwm_pin = 22; 
const int l_pwm_pin = 23; 

void setup() {
  Serial.begin(115200);

  pinMode(r_en_pin, OUTPUT);
  pinMode(l_en_pin, OUTPUT);
  pinMode(r_pwm_pin, OUTPUT);
  pinMode(l_pwm_pin, OUTPUT);

  digitalWrite(r_en_pin, HIGH);
  digitalWrite(l_en_pin, HIGH);

  digitalWrite(r_pwm_pin, LOW); 
  digitalWrite(l_pwm_pin, LOW); 

  delay(2000);
}

void loop() {
  digitalWrite(l_pwm_pin, LOW); 
  digitalWrite(r_pwm_pin, HIGH); 
  delay(150);                     
  digitalWrite(r_pwm_pin, LOW);  
  delay(1000);                   

  digitalWrite(r_pwm_pin, LOW);  
  digitalWrite(l_pwm_pin, HIGH); 
  delay(150);                     
  digitalWrite(l_pwm_pin, LOW);  
  delay(1000);                   
}