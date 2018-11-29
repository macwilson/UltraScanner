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

// Motor Libraries
#include <Wire.h>
#include <Adafruit_MotorShield.h>

// Create the motor shield object with the default I2C address
Adafruit_MotorShield motor1 = Adafruit_MotorShield(); 
Adafruit_MotorShield motor2 = Adafruit_MotorShield(); 

Adafruit_StepperMotor *horizontal_motor = motor1.getStepper(200, 1);
Adafruit_StepperMotor *vertical_motor = motor2.getStepper(200, 2);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps

  motor1.begin();  // create with the default frequency 1.6KHz
  motor2.begin();
  horizontal_motor->setSpeed(100); 
  vertical_motor->setSpeed(100);

  //Define inputs and outputs
  pinMode(trigPinBot, OUTPUT);
  pinMode(echoPinBot, INPUT);
  pinMode(trigPinTop, OUTPUT);
  pinMode(echoPinTop, INPUT);
  pinMode(buttonPin, INPUT);
}

void loop() {
  if(digitalRead(buttonPin)==HIGH){
    digitalWrite(buttonPin, LOW);
//    // Begin movement and imaging
    for(int i=0;i<400;i++){
      get_distances();
    }
    //Reset averaging values
    sumBot = 0; 
    sumTop = 0;
    //Move and capture data
    moveRight();
    moveLeft();
  }

}

void moveRight(){
  for(int i=0; i< 15; i++){
    
    delay(1000);

    get_distances();
    print_to_serial();
    horizontal_motor->step(25, FORWARD, SINGLE);
  }
  delay(1000);
  get_distances();
  print_to_serial();
}
void moveLeft(){
  for(int j=0; j<15; j++){
    horizontal_motor->step(25, BACKWARD, SINGLE);
  }
}

// Function to read distance from trasnducers ------------------------------
void get_distances() {  
  for(int k=0; k<num_to_avg; k++){
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
    
    // Sum distances
    sumBot += mmBot;
    sumTop += mmTop;
   
    
//    Serial.print("Top Dist: ");
//    Serial.println(mmTop);
//    Serial.print("Bottom Dist: ");
 //   Serial.println(mmBot);
  }
}

void print_to_serial(){
  // SEND PIXEL VALUE TO PYTHON
 Serial.println(sumTop/num_to_avg); // TOP PIXEL VALUE
 Serial.println(sumBot/num_to_avg); // BOTTOM PIXEL VALUE
  
  // Reset values
  sumBot = 0; 
  sumTop = 0;
}

