/*
 * created by Rui Santos, https://randomnerdtutorials.com
 * 
 * Complete Guide for Ultrasonic Sensor HC-SR04
 *
    Ultrasonic sensor Pins:
        VCC: +5VDC
        Trig : Trigger (INPUT) - Pin11
        Echo: Echo (OUTPUT) - Pin 12
        GND: GND
 */
 // Transducer #1
int trigPin1 = 12;  
int echoPin1 = 13;  

// Transducer #2
int trigPin2 = 10;
int echoPin2 = 11;

long duration1, duration2, mm1, mm2, sum1, sum2;
int counter = 0;
int num_to_avg = 5;
 
void setup() {
  //Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);
}
 
void loop() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration1 = pulseIn(echoPin1, HIGH);

  digitalWrite(trigPin2, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);
  duration2 = pulseIn(echoPin2, HIGH);
 
  // Convert the time into a distance
  mm1 = (duration1/2) / 2.91;     // Divide by 29.1 or multiply by 0.0343
  mm2 = (duration2/2) / 2.91;
  sum1 += mm1;
  sum2 += mm2;
  if(counter == num_to_avg -1){
    counter = 0;
    Serial.println(sum1/num_to_avg); // THIS IS THE PIXEL VALUE
    Serial.println(sum2/num_to_avg); // THIS IS THE PIXEL VALUE
    sum1 = 0;
    sum2 = 0;
    delay(200);
    // Motor would be moved here
  }
  else {
    counter ++;
  }
  
  //delay(10);
}
