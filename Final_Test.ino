#include <Servo.h>
#include "IRremote.h"
#include<Vector.h>
#include<string.h>
#include<stdlib.h>

int receiver = 2; // Signal Pin of IR receiver to Arduino Digital Pin 11
/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
//vairable uses to store the last decodedRawData
uint32_t last_decodedRawData = 0;

int count = 0;
const int Echo = 10;
const int Trig = 11;
const int motor = 12;
int Speed = 30;
int spoint = 90;
int BUZZER_PIN = 13;
Servo m1;
Servo m2;
char value;
int distance;
int Left;
int Right;
int L = 0;
int R = 0;
int L1 = 0;
int R1 = 0;
Servo servo;

Servo sg90;                   //initializing a variable for servo named sg90
int initial_position = 90;    //Declaring the initial position at 90
int LDR1 = A1;                //Pin at which LDR is connected
int LDR2 = A2;                //Pin at which LDR is connected
int error = 100;                //initializing variable for error
int servopin=7;

void setup() {
 Serial.begin(9600);
  m1.attach(8);
  m2.attach(9);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  servo.attach(motor);
  irrecv.enableIRIn(); // Start the receiver
  pinMode(BUZZER_PIN, OUTPUT); // Set the buzzer pin as an output
  sg90.attach(servopin);  // attaches the servo on pin 9
  pinMode(LDR1, INPUT);   //Making the LDR pin as input
  pinMode(LDR2, INPUT);
  sg90.write(initial_position);   //Move servo at 90 degree
  delay(2000);                   // giving a delay of 2 seconds

}

void loop() {
  //directional_move();
  solarPanel();
  //remote_control();  
  //Obstacle();
}

void remote_control(){
  if (irrecv.decode()) // have we received an IR signal?
  {
    translateIR();
    irrecv.resume(); // receive the next value
  }
}

void directional_move(){
  char *directions[] = {"Turn left and move for 23", "Turn left and move for 10","Continue forward for 30", "Turn right and move for 20"};
  char* direction[99];
  for(int i = 0; i < 4; i++)
  {
    int counter = 0;
    String mine = String(directions[i]);
    String word = "";
    bool funcChecker = false;
    bool distanceChecker = false;
    String funcValue = "";
    String distanceValue = "";

    for(int j = 0; j < mine.length(); j++)
    {
      if(char(mine[j]) != ' ')
      {
        word.concat(mine[j]);
      }
      else
      {
        if(word == "right" || word == "left" || word == "forward")
        {
          funcValue = word;
          Serial.println(funcValue);
        }
        word = "";
      }
      if(j == mine.length()-1)
      {
        distanceValue = word;
        Serial.println(distanceValue);
      }
    }
    
    if(funcValue == "right"){
      right();
      delay(500);
      forward();
      delay(2*distanceValue.toInt()/0.034);
      Stop();
    }
    if(funcValue == "left"){
    left();
    delay(500);
    forward();
    delay(2*distanceValue.toInt()/0.034);
    Stop();
    }
    if(funcValue == "forward"){
      forward();
      delay(2*distanceValue.toInt()/0.034);
      Stop();
    }
    delay(1000);
  }
}


void Obstacle() {
  distance = ultrasonic();
  if (distance <= 5) {
    Stop();
    backward();
    delay(100);
    Stop();
    L = leftsee();
    servo.write(spoint);
    delay(800);
    R = rightsee();
    servo.write(spoint);
    if (L < R) {
      right();
      delay(500);
      Stop();
      delay(200);
    } else if (L > R) {
      left();
      delay(500);
      Stop();
      delay(200);
    }
  } else {
    forward();
  }
}

void playTone(int frequency, int duration) {
  tone(BUZZER_PIN, frequency, duration); // Generate tone at the specified frequency for the given duration
  delay(duration + 50); // Add a small delay for a better separation between tones
  noTone(BUZZER_PIN); // Turn off the buzzer
}
void tone1(){
  playTone(500, 500);
  delay(500);
}

void tone2(){
  playTone(1000, 500);
  delay(500);
  playTone(1000, 500);
  delay(500);
}

void tone3(){
  playTone(1500, 500);
  delay(500);
  playTone(1500, 500);
  delay(500);
  playTone(1500, 500);
  delay(500);
}

// Ultrasonic sensor distance reading function
int ultrasonic() {
  digitalWrite(Trig, LOW);
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);
  long t = pulseIn(Echo, HIGH);
  long cm = t * 0.034/2; //time convert distance
  return cm;
}

void forward() {
  m1.write(80);
  m2.write(80);
  }
void backward() {
  m1.write(100);
  m2.write(100);
  }
void left() {
  m1.write(100);
  m2.write(80);
  }
void right() {
  m1.write(80);
  m2.write(100);
  }
void Stop() {
  m1.write(90);
  m2.write(90);
  }
int rightsee() {
  servo.write(20);
  delay(800);
  Left = ultrasonic();
  return Left;
}
int leftsee() {
  servo.write(180);
  delay(800);
  Right = ultrasonic();
  return Right;
}

void translateIR() // takes action based on IR code received
{
  // Check if it is a repeat IR code 
  if (irrecv.decodedIRData.flags)
  {
    //set the current decodedRawData to the last decodedRawData 
    irrecv.decodedIRData.decodedRawData = last_decodedRawData;
    Serial.println("REPEAT!");
  } else
  {
    //output the IR code on the serial monitor
    Serial.print("IR code:0x");
    Serial.println(irrecv.decodedIRData.decodedRawData, HEX);
  }
  //map the IR code to the remote key
  switch (irrecv.decodedIRData.decodedRawData)
  {
    case 0xB847FF00: Stop(); break;
    case 0xBB44FF00: left();    break;
    case 0xBC43FF00: right();   break;
    case 0xEA15FF00: backward();    break;
    case 0xB946FF00: forward();    break;

    default:
      break;;
  }// End Case
  //store the last decodedRawData
  last_decodedRawData = irrecv.decodedIRData.decodedRawData;
  delay(500); // Do not get immediate repeat
} //END translateIR


void solarPanel(){
  int Right1 = analogRead(LDR1); // reading value from LDR 1
  int Right2 = analogRead(LDR2); // reading value from LDR 2
  int diff1= abs(Right1 - Right2);   // Calculating the difference between the LDR's
  int diff2= abs(Right2 - Right1);
  if((diff1 <= error) || (diff2 <= error)) {
    //if the difference is under the error then do nothing
  } else {    
    if(Right1 < Right2)
    {
      initial_position = --initial_position;  //Move the servo towards 0 degree
      Serial.println("right");
      sg90.write(initial_position);
    }
    if(Right1 > Right2) 
    {
      initial_position = ++initial_position; //Move the servo towards 180 degree
      Serial.println("left");
      sg90.write(initial_position);
    }
  }
  // write the position to servo
  Serial.println(Right1);
  Serial.println(Right2);
  delay(100);
}