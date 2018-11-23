// Transducer #1
int trigPin1 = 12;  
int echoPin1 = 13;  

// Transducer #2
int trigPin2 = 10;
int echoPin2 = 11;

// Math parameters
long mm1, mm2, sum1, sum2;
int counter = 0;
int num_to_avg = 5;

// Hardware parameters
int num_horizontal_steps = 17; //?
int size_horizontal_step = 50; //for motor turning ??
int num_vertical_steps = 17; //?
int size_vertical_step = 50; //for motor turning
int vertical_step_index = -1; //start with small step
int horizontal_direction = 1; //start forwards


// Step motor libraries

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
  
  // once button has been pressed, begin one full image
  int input = int(Serial.read()); //use serial input to impersonate button for now
  if(input != -1) {   
    for(int i=0; i<num_vertical_steps; i++) { 
      for(int j=0; j<num_horizontal_steps; j++) {
        // At each single location, take a pixel value (averaged)
        read_distances();
        sum1 += mm1;
        sum2 += mm2;

        // Check if pixel value is complete
        if(counter == num_to_avg -1){
          // SEND PIXEL VALUE TO PYTHON
          Serial.println(sum1/num_to_avg); // THIS IS THE PIXEL VALUE
  
          // Reset values
          sum1 = 0; 
          sum2 = 0;
          counter = 0;
          
          // Move motor horizontally one step
          //moveHorizontally();
          horizontal_direction = -1*horizontal_direction;
        }
        else {
          counter ++;
        }
      
        //delay(10);
      }
      
      // Horizontal line is done.
      // NOTE: Either swap horizontal movement direction now, 
      // or reset horizontal position by moving it back.
      //moveVertically();
      vertical_step_index = -1 * vertical_step_index; // next step is other size
    }

    // All imaging is done.
    Serial.println("Imaging is done.");
    // Reset vertical position (drop height to bottom?)
  }

}



void read_distances() {  
  long duration1, duration2;
  // Low pulse to clean signal
  digitalWrite(trigPin1, LOW);
  delayMicroseconds(5);

  // Trigger signal
  digitalWrite(trigPin1, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin1, LOW);
 
  // Read duration til signal, in microseconds
  duration1 = pulseIn(echoPin1, HIGH);

  // Low pulse to clean signal
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(5);

  // Trigger signal 
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin2, LOW);

  // Read duration til signal, in microseconds
  duration2 = pulseIn(echoPin2, HIGH);
 
  // Convert the time into a distance
  mm1 = (duration1/2) / 2.91; 
  mm2 = (duration2/2) / 2.91;
}


void moveHorizontally() {
  // size_horizontal_step
  int d = horizontal_direction;
  
}

void moveVertically() {
  if(vertical_step_index < 0) {
    // Do a small step
    int s = size_vertical_step;
  }
  else {
    // Do a big step!!
    int s = size_vertical_step * 3;
  }
  
}

