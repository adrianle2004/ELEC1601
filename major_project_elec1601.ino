// C++ code
//
#include <Servo.h>                      // Include servo library

int value;
Servo servoLeft; 
Servo servoRight;										// Declare left servo

int motor_value;

const int irLedPin_left=10, irReceiverPin_left=11;   // Select these to match the IR LED/receiver pair that you are using
const int redLedPin_left = A2;                    // Select this to match the red LED next to the IR receiver you are using

const int irLedPin_right=2, irReceiverPin_right=3;   // Select these to match the IR LED/receiver pair that you are using
const int redLedPin_right = A0;                    // Select this to match the red LED next to the IR receiver you are using

const int irLedPin_mid=6, irReceiverPin_mid=7;   // Select these to match the IR LED/receiver pair that you are using
const int redLedPin_mid = A1;                    // Select this to match the red LED next to the IR receiver you are using

int sense_1, sense_2, sense_3;

int mode = 0; // 0: record time travel, 1: check time travel and stop

unsigned long time_record_turn;

unsigned long time_record_finish;

int count_turn = 0;

int finish_flag = 0;

void setup()                            // Built in initialization block
{               
  // Attach left signal to pin 13
  // 1.3ms = full speed clockwise
  pinMode(redLedPin_left, OUTPUT);
  pinMode(redLedPin_right, OUTPUT);
  pinMode(redLedPin_mid, OUTPUT);

  servoLeft.attach(13); 
  servoRight.attach(12);
  stand(1000);
  Serial.begin(9600);
}

void loop()                             // Main loop auto-repeats
{    
  sense_1 = irDistance(irLedPin_left, irReceiverPin_left);
  sense_2 = irDistance(irLedPin_right, irReceiverPin_right);
  sense_3 = irDistance(irLedPin_mid, irReceiverPin_mid);

  //time_record = millis();
  /*
  if (mode == 1) { // finish the maze and go back
    if (time_record = spin_delay + time_record_finish) {
      // time come back = time finish + time finish + spin time
      stop();      
    }
  }
  */
  
  Serial.print(sense_1);
  Serial.print(",");
  Serial.print(sense_2);
  Serial.print(",");
  Serial.print(sense_3);
  Serial.println(" ");
  
  // Going through the maze logic 
  if (sense_3 <= 3 && (sense_1 <= 4 && sense_2 <= 4)){ // dead end
    finish();
    count_turn = 0;
    finish_flag = 1;
    // At the end of the maze
  } else if (sense_1 <= 4 && sense_3 <= 3) {
    turn_r(150);
    time_record_turn = millis();
    count_turn++;
    if (count_turn > 15){
      digitalWrite(redLedPin_mid, HIGH);
    }
  } else if (sense_2 <= 4 && sense_3 <= 3){
    turn_l(150);
    time_record_turn = millis();
    count_turn++;
    if (count_turn > 15){
      digitalWrite(redLedPin_mid, HIGH);
    }
  } else if (sense_3 <= 1) {
    backward(150);
    if (sense_2 <= 4) {
      turn_l(100);
    } else if (sense_1 <= 4) {
      turn_r(100);
    } else {
      modify_l(150);
      backward(100);
      //modify_r(150);
      if (sense_3 == 5) {
        turn_r(200);
      } else {
        turn_l(50);
      }
    }
    time_record_turn = millis();
  } else {
    forward(0);
  }
  check_finish(finish_flag);
  /*
  Serial.print(time_record_turn);
  Serial.print(",");
  Serial.println(time_record_finish);
  */
  // Modify the car's direction - without worrrying about latency in function
  if (sense_1 <= 0) {
    turn_r(5);
  }
  if (sense_2 <= 0) {
    turn_l(5);
  } 
}

void check_finish(int flag){
  if (flag == 1) { // Have finished the maze, on the way to go back
    if (count_turn > 15){
      time_record_finish = millis();
      if (abs(time_record_finish - time_record_turn) > 3500){
        //Serial.print("finished,");
        finish();
      } 
    }
  }
}

//if (sense_3 <= 2 && (sense_1 <= 2 || sense_2 <= 2)){ // dead end

void modify_l(int time){
  servoLeft.writeMicroseconds(1350);
  servoRight.writeMicroseconds(1495);
  delay(time);
}
void modify_r(int time){
  servoLeft.writeMicroseconds(1495);
  servoRight.writeMicroseconds(1650);
  delay(time);
}

void finish(){
  if (mode == 1) { // Back to the start
    stop();
  } else {
    backward(1350);
    //time_record_finish = millis(); // record time to finish the maze
    //backward(500);
    spin();
    //spin_delay = millis(); // finish time + spin time
    mode = 1; // change into stop when we comeback
    digitalWrite(redLedPin_mid, LOW);
    digitalWrite(redLedPin_right, HIGH);
    digitalWrite(redLedPin_left, HIGH);    
  }
}

void stop(){
  //backward(100);
  stand(0);
  servoLeft.detach();
  servoRight.detach();
}

void spin(){
  turn_r(4570);
  stand(250);
  turn_r(300);
}

void forward(int time){
  servoLeft.writeMicroseconds(1650);
  servoRight.writeMicroseconds(1350);
  delay(time);
}

void backward(int time){
  servoLeft.writeMicroseconds(1350);
  servoRight.writeMicroseconds(1650);
  delay(time);
}

void turn_r(int time){
  servoLeft.writeMicroseconds(1650);
  servoRight.writeMicroseconds(1650);
  delay(time);
}

void turn_l(int time){
  servoLeft.writeMicroseconds(1350);
  servoRight.writeMicroseconds(1350);
  delay(time);
}

void stand(int time){
  servoLeft.writeMicroseconds(1495);
  servoRight.writeMicroseconds(1495);
  delay(time);
}


int irDistance(int irLedPin, int irReceivePin)
{
   int distance = 0;
   for(long f = 38000; f <= 42000; f += 1000)
   {
      distance += irDetect(irLedPin, irReceivePin, f);
   }
   return distance;
}

int irDetect(int irLedPin, int irReceiverPin, long frequency)
{
  tone(irLedPin, frequency);                 // Turn on the IR LED square wave
  delay(1);                                  // Wait 1 ms
  noTone(irLedPin);                          // Turn off the IR LED
  int ir = digitalRead(irReceiverPin);       // IR receiver -> ir variable
  delay(1);                                  // Down time before recheck
  return ir;                                 // Return 0 detect, 1 no detect
}