/* 
This is a test sketch for the Adafruit assembled Motor Shield for Arduino v2
It won't work with v1.x motor shields! Only for the v2's with built in PWM
control

For use with the Adafruit Motor Shield v2 
---->  http://www.adafruit.com/products/1438
*/


#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield motor1 = Adafruit_MotorShield(); 

// Or, create it with a different I2C address (say for stacking)
//Adafruit_MotorShield motor2 = Adafruit_MotorShield(); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *motor_1 = motor1.getStepper(200, 1);


void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
//  Serial.print(motor1);
  motor1.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  motor_1->setSpeed(100);  // 100 rpm   
}

void loop() {
// step(num_of_steps @ 1.8degrees per step, direction (FORWARD/BACKWARD), (SINGLE/DOUBLE/INTERLEAVE/MICROSTEP))
  
  moveRight(); // move towards motor
  delay(3000);
  moveLeft(); // move away from motor 
  delay(3000);
}

void moveRight(){
  for(int i=0; i< 25; i++){
    motor_1->step(10, FORWARD, DOUBLE);
    delay(100);
  }
}
void moveLeft(){
  for(int j=0; j<25; j++){
    motor_1->step(10, BACKWARD, DOUBLE); 
    delay(100);
  }
}
