

/*
JVCVolUp();
JVCVolDn();
JVCSource();
JVCSound();
JVCMute();
JVCSkipFwd();
JVCSkipBack();
JVCSkipFwdHold();
JVCSkipBackHold();
*/
const int PIN = 3;          // Digital IO pin connected to base of transistor
const int Length = 537;     // Length in Microseconds
const int Reps = 3;         // Number of times to repeat each transmission


int JVCVolUpVal;
int JVCVolDnVal;
int JVCSourceVal;
int JVCSoundVal;
int JVCMuteVal;
int JVCSkipFwdVal;
int JVCSkipBackVal;
int JVCSkipFwdHoldVal;
int JVCSkipBackHoldVal;


int traning_mode = false;
int analog=0;// для чтения аналогового сигнала
unsigned long time;// переменная для хранения времени


void setup() 
{
  pinMode(PIN, OUTPUT); // Set pin to output
  pinMode(13, OUTPUT);
  pinMode(A3, INPUT_PULLUP); // Set pin to output
  digitalWrite(PIN, LOW); // Make PIN low to shut off transistor
  Serial.begin(9600); 
}

void loop(){
    boolean button1 = !digitalRead(A3);
    analog = analogRead(A0);

    


    if (button1 == 1 && !traning_mode){
        traning_mode = true;
      }

    if(traning_mode){

        signal_diod(4);
        traning_mode = false;
      
      
      }

    Serial.println("TEst");
    Serial.println(button1);
    Serial.println(analog);

    delay(500);

}
/*


  JVC
  Communicate with JVC car radio over 3.5mm Remote connector
  
  Connect Arduino PIN to Base of NPN transistor
  Emitter is tied to ground
  Collector is tied to 3.5mm TIP
  Ground 3.5mm Ring/Sleeve
 






  By Dan Guerra (deguerra98@yahoo.com)
  */

void signal_diod(int count){
    for (int i = 0; i < count; i++){
        digitalWrite(13, 1);
        delay(500);
        digitalWrite(13, 0);
        delay(500);
      }
  
  }
void JVCVolUp() {      // Send 0x04
  for (int i = 1; i <= Reps; i++); {
    Preamble();
    
    bZERO();
    bZERO();
    bONE();    // 4 
    bZERO();
    
    bZERO();
    bZERO();   // 0
    bZERO();
  
    Postamble();
  }
}

void JVCVolDn() {      // Send 0x05
  for (int i = 1; i <= Reps; i++); {
    Preamble();
    
    bONE();
    bZERO();
    bONE();    // 5 
    bZERO();
    
    bZERO();
    bZERO();   // 0
    bZERO();
  
    Postamble();
  }
}

void JVCSource() {      // Send 0x08
  for (int i = 1; i <= Reps; i++); {
    Preamble();
    
    bZERO();
    bZERO();
    bZERO();    // 8 
    bONE();
    
    bZERO();
    bZERO();   // 0
    bZERO();
  
    Postamble();
  }
}

void JVCSound() {      // Send 0x0D
  for (int i = 1; i <= Reps; i++); {
    Preamble();
    
    bONE();
    bZERO();
    bONE();    // D (13) 
    bONE();
    
    bZERO();
    bZERO();   // 0
    bZERO();
  
    Postamble();
  }
}

void JVCMute() {      // Send 0x0E
  for (int i = 1; i <= Reps; i++); {
    Preamble();
    
    bZERO();
    bONE();
    bONE();    // E (14) 
    bONE();
    
    bZERO();
    bZERO();   // 0
    bZERO();
  
    Postamble();
  }
}

void JVCSkipFwd() {      // Send 0x12
  for (int i = 1; i <= Reps; i++); {
    Preamble();
    
    bZERO();
    bONE();
    bZERO();    // 2 
    bZERO();
    
    bONE();
    bZERO();   // 1
    bZERO();
  
    Postamble();
  }
}

void JVCSkipBack() {      // Send 0x13
  for (int i = 1; i <= Reps; i++); {
    Preamble();
    
    bONE();
    bONE();
    bZERO();    // 3 
    bZERO();
    
    bONE();
    bZERO();   // 1
    bZERO();
  
    Postamble();
  }
}

void JVCSkipFwdHold() {      // Send 0x14
  for (int i = 1; i <= Reps; i++); {
    Preamble();
    
    bZERO();
    bZERO();
    bONE();    // 4 
    bZERO();
    
    bONE();
    bZERO();   // 1
    bZERO();
  
    Postamble();
  }
}

void JVCSkipBackHold() {      // Send 0x15
  for (int i = 1; i <= Reps; i++); {
    Preamble();
    
    bONE();
    bZERO();
    bONE();    // 5 
    bZERO();
    
    bONE();
    bZERO();   // 1
    bZERO();
  
    Postamble();
  }
}


void bONE() {                     // Send a binary ONE over the line
  digitalWrite(PIN, HIGH);        // Pull 3.5mm TIP low
  delayMicroseconds(Length);      // for 537us
  digitalWrite(PIN, LOW);         // Allow 3.5mm TIP to go high
  delayMicroseconds(Length * 3);  // for 537 * 3 = 1611us
}


void bZERO() {                    // Send a binary ZERO over the line
  digitalWrite(PIN, HIGH);        // Pull 3.5mm TIP low
  delayMicroseconds(Length);      // for 537us
  digitalWrite(PIN, LOW);         // Allow 3.5mm TIP to go high
  delayMicroseconds(Length);      // for 537us
}

void Preamble() {
  digitalWrite(PIN, LOW);         // Not sure what this does
  delayMicroseconds(Length * 1);
  
  digitalWrite(PIN, HIGH);        // AGC
  delayMicroseconds(Length * 16);
  
  digitalWrite(PIN, LOW);         // AGC
  delayMicroseconds(Length * 8);
  
  bONE();    // 1 Start Bit
  
  bONE();    //       (7 bit device code)
  bONE();    
  bONE();    // 7
  bZERO();
 
  bZERO();
  bZERO();    //4
  bONE();
}
    
    
void Postamble() {
  bONE();
  bONE();    // 2 stop bits
}

