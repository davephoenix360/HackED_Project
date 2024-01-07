String res;
String directions;
bool sentData = false;
bool dataGotten = false;
int count = 0;
String HUB_MALL = "9005 112 ST NW";
String ETLC = "9120 116 St NW T6G 2V4";
String HOME = "11540 78 Ave NW T6G 0N5";
String CCIS = "11335 Saskatchewan Drive NW T6G 2M9";
String LISTER_RESIDENCE = "11613 87 Avenue NW T6G 2H6";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  /*for(int i = 0; i < 10; i++)
  {
    Serial.println(i);
    delay(1000);
    }
    */  
  while(Serial.available() == 0)
  {
    delay(100);
  }
  res = Serial.readStringUntil('\n');
  //Serial.println(res); // receive r
  if(res == "r")
  {
    Serial.println(HOME);
  }
  while(Serial.available() == 0)
  {
    delay(100);
  }
  res = Serial.readStringUntil('\n');
  if(res == "r")
  {
    Serial.println(ETLC);
  }
    //send origin
    //Serial.println(count);
    //count += 1; 
  while(Serial.available() == 0)
  {
    delay(100);
  }
  directions = Serial.readStringUntil('\n'); // receive directions
  Serial.println(directions);
   
}
