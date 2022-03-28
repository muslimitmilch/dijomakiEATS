#include <SoftwareSerial.h>
#include <Servo.h>


int IRsignal[] = {
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
const int NumIRsignals = 80;

const int pedalPin = 9;
const int barPin = 12;
const int catapultPin = 10;
const int IRledPinBett = 7;
const int IRledPinMitte = 8;
const int sensorPinOut = 3;
const int sensorPinIn = 2;
int distance = 0;
int i;
int measuredDistance = 0;
int setupDistance = 0;



Servo catapult;
Servo bar;
void setup() {
  pinMode(pedalPin, INPUT_PULLUP);
  pinMode(IRledPinBett, OUTPUT);
  pinMode(IRledPinMitte, OUTPUT);
  pinMode(sensorPinOut, OUTPUT);
  pinMode(sensorPinIn, INPUT);
  Serial.begin(9600);
  catapult.attach(catapultPin);
  catapult.write(80);
  bar.attach(barPin);
  bar.write(65);
  setupDistance = getDistance();
  delay(200);
  Serial.println("fertig initialisiert");
}


void loop() {
  measuredDistance = getDistance();
  Serial.println(measuredDistance);
  if(measuredDistance < (setupDistance * 0.7)){
    Serial.println("aktiviert");
    catapult.write(10);
    bar.write(115);
    signal1mitte();
    signal1bett();
    catapult.write(80);
    //bar.write(105);
    delay(10);
    
    while(digitalRead(pedalPin)){}
    bar.write(64);
    Serial.println("reset");
    delay(5000);
  }
  else{
    Serial.println("keine bewegung");
  }
  delay(200);
}



void signal1mitte(){ //code von rick osgood
  delay(10);
  for (int i = 0; i < NumIRsignals; i+=2) {         //Loop through all of the IR timings
      pulseIRmitte(IRsignal[i]*10);              //Flash IR LED at 38khz for the right amount of time
      delayMicroseconds(IRsignal[i+1]*10);  //Then turn it off for the right amount of time
    }
  }

  
void signal1bett(){ //code von rick osgood
  delay(10);
  for (int i = 0; i < NumIRsignals; i+=2) {         //Loop through all of the IR timings
      pulseIRbett(IRsignal[i]*10);              //Flash IR LED at 38khz for the right amount of time
      delayMicroseconds(IRsignal[i+1]*10);  //Then turn it off for the right amount of time
    }
  }


void pulseIRmitte(long microsecs) { //code von rick osgood
  while (microsecs > 0) {
    // 38 kHz is about 13 microseconds high and 13 microseconds low
   digitalWrite(IRledPinMitte, HIGH);  // this takes about 3 microseconds to happen
   delayMicroseconds(10);         // hang out for 10 microseconds, you can also change this to 9 if its not working
   digitalWrite(IRledPinMitte, LOW);   // this also takes about 3 microseconds
   delayMicroseconds(10);         // hang out for 10 microseconds, you can also change this to 9 if its not working
 
   // so 26 microseconds altogether
   microsecs -= 26;
  }
}


void pulseIRbett(long microsecs) { //code von rick osgood
  while (microsecs > 0) {
    // 38 kHz is about 13 microseconds high and 13 microseconds low
   digitalWrite(IRledPinBett, HIGH);  // this takes about 3 microseconds to happen
   delayMicroseconds(10);         // hang out for 10 microseconds, you can also change this to 9 if its not working
   digitalWrite(IRledPinBett, LOW);   // this also takes about 3 microseconds
   delayMicroseconds(10);         // hang out for 10 microseconds, you can also change this to 9 if its not working
 
   // so 26 microseconds altogether
   microsecs -= 26;
  }
}


int getDistance(){ 
  long distance=0;
  long time=0;
  digitalWrite(sensorPinOut, LOW); 
  delayMicroseconds(3);
  digitalWrite(sensorPinOut, HIGH); //Trigger Impuls 10 us
  delayMicroseconds(10);
  digitalWrite(sensorPinOut, LOW); 
  time = pulseIn(sensorPinIn, HIGH); // Echo-Zeit messen 
  
  time = (time/2); // Zeit halbieren
  distance = time / 29.1; // Zeit in Zentimeter umrechnen
  return(distance);
}
