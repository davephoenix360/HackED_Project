#include <Servo.h>
#include "IRremote.h"
#include<Vector.h>
#include<string.h>
#include<stdlib.h>

int receiver = 2; // Signal Pin of IR receiver to Arduino Digital Pin 11
/*-----( Declare objects )-----*/
//IRrecv irrecv(receiver);     // create instance of 'irrecv'
//vairable uses to store the last decodedRawData
uint32_t last_decodedRawData = 0;

int count = 0;
const int Echo = 10;
const int Trig = 11;
const int motor = 12;
int Speed = 30;
int spoint = 103;
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

void setup() {
  Serial.begin(9600);
  m1.attach(8);
  m2.attach(9);
  pinMode(Trig, OUTPUT);
  pinMode(Echo, INPUT);
  servo.attach(motor);
    //m1.write(80);
  //m2.write(80);
  irrecv.enableIRIn(); // Start the receiver

}

void loop() {
  directional_move();
  //remote_control();  
  //Obstacle();
  //Serial.println("yeah b");
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