// Transducer #1
int trigPinBot = 12;  
int echoPinBot = 13;  

// Transducer #2
int trigPinTop = 10;
int echoPinTop = 11;

// Button pin
int buttonPin=7;

// Math parameters
long mmBot, mmTop, sumBot, sumTop;
int num_to_avg = 5;

// Hardware parameters
int num_horizontal_steps = 15; //
int size_horizontal_step = 25; // motor step size in horizontal direction
int num_vertical_steps = 12; // includes both big and little steps
int size_small_vertical_step = 39; // motor step size in horizontal direction
int size_big_vertical_step = 118; // motor step size in horizontal direction
int vertical_step_index = -1; //start with small step
int horizontal_direction = 1; //start forwards


// Step motor libraries
#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield objects with the default I2C address
Adafruit_MotorShield motor1 = Adafruit_MotorShield(); 
Adafruit_MotorShield motor2 = Adafruit_MotorShield(); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)to motor port #1(M1 and M2) #2 (M3 and M4)
Adafruit_StepperMotor *horizontal_motor = motor1.getStepper(200, 1);
Adafruit_StepperMotor *vertical_motor = motor2.getStepper(200, 2);

void setup() {
  //Serial Port begin
  Serial.begin (9600);

  // Create motors with default frequency (1.6KHz) and set rpm
  motor1.begin();
  motor2.begin();
  horizontal_motor->setSpeed(100);  // 100 rpm 
  vertical_motor->setSpeed(100);    // 100 rpm 
  
  //Define inputs and outputs
  pinMode(trigPinBot, OUTPUT);
  pinMode(echoPinBot, INPUT);
  pinMode(trigPinTop, OUTPUT);
  pinMode(echoPinTop, INPUT);

  // Define Button input
  pinMode(buttonPin, INPUT);
}

void loop() {
  
  // once button has been pressed, begin one full image
  if(digitalRead(buttonPin)==HIGH){
    Serial.println(99999);
    digitalWrite(buttonPin, LOW);
    
    // Initial distance calibration
    for(int i=0; i<300; i++){
      read_distances();
    }
    // Reset values
    sumBot = 0; 
    sumTop = 0;

    // Begin imaging
    for(int i=0; i<num_vertical_steps; i++) { 
      for(int j=0; j<num_horizontal_steps; j++) { // ROW LOOP
        delay(500);
        for(int k=0; k<num_to_avg; k++){ // AVERAGING LOOP
          // At each single location, take a pixel value (averaged)
          read_distances();
          sumBot += mmBot;
          sumTop += mmTop;
        }   
        // SEND PIXEL VALUE TO PYTHON
        Serial.println(sumBot/num_to_avg); // BOTTOM PIXEL VALUE
        Serial.println(sumTop/num_to_avg); // TOP PIXEL VALUE

        // Reset values
        sumBot = 0; 
        sumTop = 0;
        
        // Move motor horizontally one step
        moveHorizontally();
      }
      
      // HORIZONTAL LINE IS DONE.

      // Move vertically
      if(i < num_vertical_steps - 1) {
        moveVertically(); 
        delay(300);
      }

      // Change horiz. direction and vert. step size for next time
      horizontal_direction = -1*horizontal_direction;
      vertical_step_index = -1 * vertical_step_index; 
    }

    // 2D IMAGE IS DONE.
    // Serial.println("Imaging is done."); // debug
    // Reset vertical position (drop height to bottom?)
    resetPosition();
  }

}



void read_distances() {  
  long durationBot, durationTop;
  // GATHER BOTTOM DATA
  // Low pulse to clean signal
  digitalWrite(trigPinBot, LOW);
  delayMicroseconds(5);

  // Trigger signal
  digitalWrite(trigPinBot, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinBot, LOW); 
 
  // Read duration til signal, in microseconds
  durationBot = pulseIn(echoPinBot, HIGH);

  // GATHER TOP DATA
  // Low pulse to clean signal
  digitalWrite(trigPinTop, LOW);
  delayMicroseconds(5);

  // Trigger signal 
  digitalWrite(trigPinTop, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPinTop, LOW);

  // Read duration til signal, in microseconds
  durationTop = pulseIn(echoPinTop, HIGH);
 
  // Convert the time into a distance
  mmBot = (durationBot/2) / 2.91; 
  mmTop = (durationTop/2) / 2.91;
}


void moveHorizontally() {
  // size_horizontal_step
  if(horizontal_direction <0) {
    // Move transducer platform one small step right
    horizontal_motor->step(size_horizontal_step, BACKWARD, DOUBLE); 
  }
  else {
    // Move transducer platform one small step right
    horizontal_motor->step(size_horizontal_step, FORWARD, DOUBLE);
  }
}

void moveVertically() {
  // Move vertical platform up, either 1 step or 3, 
  // depending on the order required. 
  if(vertical_step_index < 0) {
    // Do a small step
    vertical_motor->step(size_small_vertical_step, BACKWARD, DOUBLE);
  }
  else {
    // Do a big step!!
    vertical_motor->step(size_big_vertical_step, BACKWARD, DOUBLE); 
  }
  
}

void resetPosition() {
  // Drop the vertical platform back to the bottom
  // Move the horizontal cart back to original position (if not already)
  int total_vertical_size = (num_vertical_steps/2 -1) * size_big_vertical_step + (num_vertical_steps/2) * size_small_vertical_step;
  vertical_motor->step(total_vertical_size, FORWARD, DOUBLE); 
}
