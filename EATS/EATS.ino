#include <SoftwareSerial.h>
#include <Servo.h>

//list of pins
const int pedalPin = 9;
const int barPin = 12;
const int catapultPin = 10;
const int IRledPinBett = 7;
const int IRledPinMitte = 8;
const int ultrasonicSensorPinTrigger = 3;
const int ultrasonicSensorPinEcho = 2;

//initialising variables
int distance = 0;
int i;
int measuredDistance = 0;
int setupDistance = 0;

//initialising objects
Servo catapult;
Servo bar;

//list of signals (currently only IRsignal_ON)
int IRsignal_ON[] = {
// ON, OFF (in 10's of microseconds)
  936, 456,
  56, 62,
  56, 176,
  54, 62,
  56, 60,
  56, 62,
  56, 62,
  56, 60,
  56, 60,
  56, 62,
  56, 176,
  54, 176,
  56, 176,
  56, 176,
  56, 176,
  54, 176,
  56, 176,
  56, 174,
  58, 60,
  56, 62,
  54, 62,
  56, 62,
  54, 62,
  56, 62,
  56, 60,
  56, 62,
  56, 176,
  56, 174,
  56, 176,
  56, 176,
  56, 174,
  56, 176,
  56, 176,
  56, 4136,
  936, 228,
  58, 3346,
  936, 228,
  58, 3346,
  938, 226,
  60, 0};
const int NumIRsignals_ON = 80;




void setup() {
  pinMode(pedalPin, INPUT_PULLUP);
  pinMode(IRledPinBett, OUTPUT);
  pinMode(IRledPinMitte, OUTPUT);
  pinMode(ultrasonicSensorPinTrigger, OUTPUT);
  pinMode(ultrasonicSensorPinEcho, INPUT);
  Serial.begin(9600);
  catapult.attach(catapultPin);
  catapult.write(80);
  bar.attach(barPin);
  bar.write(65);
  setupDistance = getDistance();
  playWindows();
  delay(2500);
  playDijomaki();
  Serial.println("fertig initialisiert");
}


void loop() {
  measuredDistance = getDistance();
  Serial.println(measuredDistance);
  if(measuredDistance < (setupDistance * 0.7)){
    Serial.println("aktiviert");
    catapult.write(10);
    bar.write(115);
    sendIRmitte(IRsignal_ON, NumIRsignals_ON);
    sendIRbett(IRsignal_ON, NumIRsignals_ON);
    delay(50);
    catapult.write(80);
    //bar.write(105);
    while(digitalRead(pedalPin)){}
    bar.write(64);
    Serial.println("reset");
  }
  else{
    Serial.println("keine bewegung");
  }
  delay(200);
}



int getDistance(){ 
  long distance=0;
  long time=0;
  digitalWrite(ultrasonicSensorPinTrigger, LOW); 
  delayMicroseconds(3);
  digitalWrite(ultrasonicSensorPinTrigger, HIGH); //Trigger Impuls 10 us
  delayMicroseconds(10);
  digitalWrite(ultrasonicSensorPinTrigger, LOW); 
  time = pulseIn(ultrasonicSensorPinEcho, HIGH); // Echo-Zeit messen 
  
  time = (time/2); // Zeit halbieren
  distance = time / 29.1; // Zeit in Zentimeter umrechnen
  return(distance);
}
