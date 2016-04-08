#include <Arduino.h>
#include <Wire.h>
#include <Servo.h>
#include <SoftwareSerial.h>

#include <MeMCore.h>

double angle_rad = PI/180.0;
double angle_deg = 180.0/PI;
MeBuzzer buzzer;



void setup(){
    buzzer.tone(440, 500);
    buzzer.tone(440, 500);
    buzzer.tone(440, 500);
    buzzer.tone(349, 376);
    buzzer.tone(523, 126);
    
    buzzer.tone(440, 500);
    buzzer.tone(349, 376);
    buzzer.tone(523, 126);
    buzzer.tone(440, 1000);
    
    buzzer.tone(659, 500);
    buzzer.tone(659, 500);
    buzzer.tone(659, 500);
    buzzer.tone(698, 376);
    buzzer.tone(523, 126);
    
    buzzer.tone(415, 500);
    buzzer.tone(349, 376);
    buzzer.tone(523, 126);
    buzzer.tone(440, 1000);
    
    buzzer.tone(880, 500);
    buzzer.tone(440, 376);
    buzzer.tone(440, 126);
    buzzer.tone(880, 500);
    buzzer.tone(831, 376);
    buzzer.tone(784, 126);
    
    buzzer.tone(740, 166);
    buzzer.tone(698, 166);
    buzzer.tone(740, 166);
    delay(250);
    buzzer.tone(466, 250);
    buzzer.tone(622, 500);
    buzzer.tone(587, 376);
    buzzer.tone(554, 126);
    
    buzzer.tone(523, 166);
    buzzer.tone(494, 166);
    buzzer.tone(523, 166);
    delay(250);
    buzzer.tone(349, 250);
    buzzer.tone(415, 500);
    buzzer.tone(349, 376);
    buzzer.tone(415, 126);
    
    buzzer.tone(523, 500);
    buzzer.tone(440, 376);
    buzzer.tone(523, 126);
    buzzer.tone(659, 1000);
    
    buzzer.tone(880, 500);
    buzzer.tone(440, 376);
    buzzer.tone(440, 126);
    buzzer.tone(880, 500);
    buzzer.tone(831, 376);
    buzzer.tone(784, 126);
    
    buzzer.tone(740, 166);
    buzzer.tone(698, 166);
    buzzer.tone(740, 166);
    delay(250);
    buzzer.tone(466, 250);
    buzzer.tone(622, 500);
    buzzer.tone(587, 376);
    buzzer.tone(554, 126);

    buzzer.tone(523, 166);
    buzzer.tone(494, 166);
    buzzer.tone(523, 166);
    delay(250);
    buzzer.tone(349, 250);
    buzzer.tone(415, 500);
    buzzer.tone(349, 376);
    buzzer.tone(523, 126);
    
    buzzer.tone(440, 500);
    buzzer.tone(349, 376);
    buzzer.tone(523, 126);
    buzzer.tone(440, 1000);
    
}

void loop(){
    
    
}

