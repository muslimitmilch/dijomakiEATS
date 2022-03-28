//file for IR-functions, code heavily inspired/copied from Rick Osgood



void sendIRmitte(int IRsignal[], int NumIRsignals){ //rewrite; code by rick osgood
  delay(10);
  for (int i = 0; i < NumIRsignals; i+=2) {         //Loop through all of the IR timings
      pulseIRmitte(IRsignal[i]*10);              //Flash IR LED at 38khz for the right amount of time
      delayMicroseconds(IRsignal[i+1]*10);  //Then turn it off for the right amount of time
    }
}

void sendIRbett(int IRsignal[], int NumIRsignals){ //rewrite; code by rick osgood
  delay(10);
  for (int i = 0; i < NumIRsignals; i+=2) {         //Loop through all of the IR timings
      pulseIRbett(IRsignal[i]*10);              //Flash IR LED at 38khz for the right amount of time
      delayMicroseconds(IRsignal[i+1]*10);  //Then turn it off for the right amount of time
    }
}


//from here on old code

void pulseIRmitte(long microsecs) { //code by rick osgood
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


void pulseIRbett(long microsecs) { //code by rick osgood
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
