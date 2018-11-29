// Transducer 
int trigPin = 10;  
int echoPin = 11;  

long duration, mm, sum;
int counter = 0;
int num_to_avg = 1;
 
void setup() {
  //Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}
 
void loop() {
  // The sensor is triggered by a HIGH pulse of 10 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  duration = pulseIn(echoPin, HIGH);
 
  // Convert the time into a distance
  mm = (duration/2) / 2.91;     
  sum += mm;
  if(counter == num_to_avg -1){
    counter = 0;
    Serial.println(sum/num_to_avg); // THIS IS THE PIXEL VALUE
    sum = 0;
    delay(200);
  }
  else {
    counter ++;
  }
  
}
