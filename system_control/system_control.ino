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
int counter = 0;
int num_to_avg = 5;

// Hardware parameters
int num_horizontal_steps = 10; //
int size_horizontal_step = 38; // motor step size in horizontal direction
int num_vertical_steps = 3; // includes a big step and little step
int size_small_vertical_step = 59; // motor step size in horizontal direction
int size_big_vertical_step = 177; // motor step size in horizontal direction
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
    Serial.println(-100)
    digitalWrite(buttonPin, LOW);
    for(int i=0; i<num_vertical_steps; i++) { 
      for(int j=0; j<num_horizontal_steps; j++) {
        
        // At each single location, take a pixel value (averaged)
        read_distances();
        sumBot += mmBot;
        sumTop += mmTop;

        counter++;
        
        // Check if pixel value is complete
        if(counter == num_to_avg -1){
          
          // SEND PIXEL VALUE TO PYTHON
          Serial.println(sumBot/num_to_avg); // BOTTOM PIXEL VALUE
          Serial.println(sumTop/num_to_avg); // TOP PIXEL VALUE
  
          // Reset values
          sumBot = 0; 
          sumTop = 0;
          counter = 0;
          
          // Move motor horizontally one step
          moveHorizontally();

         
        }
      }
      
      // HORIZONTAL LINE IS DONE.

      // Move vertically
      moveVertically();

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
    int s = size_vertical_step;
    vertical_motor->step(size_small_vertical_step, FORWARD, DOUBLE);
  }
  else {
    // Do a big step!!
    int s = size_vertical_step * 3;
    vertical_motor->step(size_big_vertical_step, FORWARD, DOUBLE); 
  }
  
}

void resetPosition() {
  // Drop the vertical platform back to the bottom
  // Move the horizontal cart back to original position (if not already)
  
}
