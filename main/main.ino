#include <EEPROM.h>

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

const int PIN = 3; // Digital IO pin connected to base of transistor
const int Length = 527; // Length in Microseconds
const int Reps = 3; // Number of times to repeat each transmission
const int fault = 20;

int JVCVolUpVal;
int JVCVolDnVal;
int JVCSourceVal;
int JVCSoundVal;
int JVCMuteVal;
int JVCSkipFwdVal;
int JVCSkipBackVal;
int JVCSkipFwdHoldVal;
int JVCSkipBackHoldVal;

int count_button = (2 * 9) - 2;
int traning_mode = false;
int analog = 0; // для чтения аналогового сигнала
int step_memory = 0;
bool read_EEPROM = true;

void setup() {
  pinMode(PIN, OUTPUT); // Set pin to output
  pinMode(13, OUTPUT);
  pinMode(A3, INPUT_PULLUP); // Set pin to output
  digitalWrite(PIN, LOW); // Make PIN low to shut off transistor
  Serial.begin(9600);
}

void loop() {
//Load bauuton from EEPROM
  if (read_EEPROM) {
    read_from_EEPROM();
    printAll_EEPROM();
    read_EEPROM = false;
  }

  boolean button1 = !digitalRead(A3);
  analog = analogRead(A0);

  if (button1 == 1 && !traning_mode) {
    traning_mode = true;
  }

  while (traning_mode) {
    Serial.println("Go to traning_mode");
    int start_point = analog;
    int button = 0;

    signal_diod(4);
    Serial.println("preLoop");
    while (!(analogRead(A0) > start_point + fault || analogRead(A0) < start_point - fault || (!digitalRead(A3) == true))) {
      Serial.print("Start_poin:");
      Serial.println(start_point);
      Serial.print("Now");
      Serial.println(analogRead(A0));
    }
    if (!digitalRead(A3) == true) {
      //Any Impossible Value
      button = 4000;
    } else {
      button = analogRead(A0);

    }

    EEPROM_int_write(step_memory, button);
    signal_diod(2);
    step_memory += 2;
    Serial.println("Write done");

    if (step_memory > count_button) {
      read_from_EEPROM();
      printAll_EEPROM();
      traning_mode = false;
      step_memory = 0;
    }

  }

  if (between(JVCVolUpVal, analogRead(A0))) {
    JVCVolUp();
    Serial.println("JVCVolUpVal");
  }
  if (between(JVCVolDnVal, analogRead(A0))) {
    JVCVolDn();
    Serial.println("JVCVolDnVal");
  }
  if (between(JVCSourceVal, analogRead(A0))) {
    JVCSource();
    Serial.println("JVCSourceVal");
  }
  if (between(JVCSoundVal, analogRead(A0))) {
    JVCSound();
    Serial.println("JVCSoundVal");
  }
  if (between(JVCMuteVal, analogRead(A0))) {
    JVCMute();
    Serial.println("JVCMuteVal");
  }
  if (between(JVCSkipFwdVal, analogRead(A0))) {
    JVCSkipFwd();
    Serial.println("JVCSkipFwdVal");
  }
  if (between(JVCSkipBack, analogRead(A0))) {
    JVCSkipBack();
    Serial.println("JVCSkipBack");
  }
  if (between(JVCSkipFwdHoldVal, analogRead(A0))) {
    JVCSkipFwdHold();
    Serial.println("JVCSkipFwdHoldVal");
  }
  if (between(JVCSkipBackHoldVal, analogRead(A0))) {
    JVCSkipBackHold();
    Serial.println("JVCSkipBackHoldVal");
  }

}

boolean between(int button, int analog) {
  return (analog < button + fault && analog > button - fault);

}

void printAll_EEPROM() {
  for (int i = 0; i <= count_button; i += 2) {
    Serial.print("Memory_");
    Serial.print(i);
    Serial.print("_val:");
    Serial.println(EEPROM_int_read(i));

  }
}

//I'm ashamed of this piece...

void read_from_EEPROM() {
  JVCVolUpVal = EEPROM_int_read(0);
  JVCVolDnVal = EEPROM_int_read(2);
  JVCSourceVal = EEPROM_int_read(4);
  JVCSoundVal = EEPROM_int_read(6);
  JVCMuteVal = EEPROM_int_read(8);
  JVCSkipFwdVal = EEPROM_int_read(10);
  JVCSkipBackVal = EEPROM_int_read(12);
  JVCSkipFwdHoldVal = EEPROM_int_read(14);
  JVCSkipBackHoldVal = EEPROM_int_read(16);
}

// Read from EEPROM
int EEPROM_int_read(int addr) {
  byte raw[2];
  for (byte i = 0; i < 2; i++) raw[i] = EEPROM.read(addr + i);
  int & num = (int & ) raw;
  return num;
}

// Write to EEPROM
void EEPROM_int_write(int addr, int num) {
  byte raw[2];
  (int & ) raw = num;
  for (byte i = 0; i < 2; i++) EEPROM.write(addr + i, raw[i]);
}

void signal_diod(int count) {
  for (int i = 0; i < count; i++) {
    digitalWrite(13, 1);
    delay(200);
    digitalWrite(13, 0);
    delay(200);
  }

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

void JVCVolUp() { // Send 0x04
  for (int i = 1; i <= Reps; i++); {
    Preamble();

    bZERO();
    bZERO();
    bONE(); // 4 
    bZERO();

    bZERO();
    bZERO(); // 0
    bZERO();

    Postamble();
  }
}

void JVCVolDn() { // Send 0x05
  for (int i = 1; i <= Reps; i++); {
    Preamble();

    bONE();
    bZERO();
    bONE(); // 5 
    bZERO();

    bZERO();
    bZERO(); // 0
    bZERO();

    Postamble();
  }
}

void JVCSource() { // Send 0x08
  for (int i = 1; i <= Reps; i++); {
    Preamble();

    bZERO();
    bZERO();
    bZERO(); // 8 
    bONE();

    bZERO();
    bZERO(); // 0
    bZERO();

    Postamble();
  }
}

void JVCSound() { // Send 0x0D
  for (int i = 1; i <= Reps; i++); {
    Preamble();

    bONE();
    bZERO();
    bONE(); // D (13) 
    bONE();

    bZERO();
    bZERO(); // 0
    bZERO();

    Postamble();
  }
}

void JVCMute() { // Send 0x0E
  for (int i = 1; i <= Reps; i++); {
    Preamble();

    bZERO();
    bONE();
    bONE(); // E (14) 
    bONE();

    bZERO();
    bZERO(); // 0
    bZERO();

    Postamble();
  }
}

void JVCSkipFwd() { // Send 0x12
  for (int i = 1; i <= Reps; i++); {
    Preamble();

    bZERO();
    bONE();
    bZERO(); // 2 
    bZERO();

    bONE();
    bZERO(); // 1
    bZERO();

    Postamble();
  }
}

void JVCSkipBack() { // Send 0x13
  for (int i = 1; i <= Reps; i++); {
    Preamble();

    bONE();
    bONE();
    bZERO(); // 3 
    bZERO();

    bONE();
    bZERO(); // 1
    bZERO();

    Postamble();
  }
}

void JVCSkipFwdHold() { // Send 0x14
  for (int i = 1; i <= Reps; i++); {
    Preamble();

    bZERO();
    bZERO();
    bONE(); // 4 
    bZERO();

    bONE();
    bZERO(); // 1
    bZERO();

    Postamble();
  }
}

void JVCSkipBackHold() { // Send 0x15
  for (int i = 1; i <= Reps; i++); {
    Preamble();

    bONE();
    bZERO();
    bONE(); // 5 
    bZERO();

    bONE();
    bZERO(); // 1
    bZERO();

    Postamble();
  }
}

void bONE() { // Send a binary ONE over the line
  digitalWrite(PIN, HIGH); // Pull 3.5mm TIP low
  delayMicroseconds(Length); // for 537us
  digitalWrite(PIN, LOW); // Allow 3.5mm TIP to go high
  delayMicroseconds(Length * 2); // for 537 * 3 = 1611us
}

void bZERO() { // Send a binary ZERO over the line
  digitalWrite(PIN, HIGH); // Pull 3.5mm TIP low
  delayMicroseconds(Length); // for 537us
  digitalWrite(PIN, LOW); // Allow 3.5mm TIP to go high
  delayMicroseconds(Length); // for 537us
}

void Preamble() {
  //  digitalWrite(PIN, LOW);         // Not sure what this does
  //  delayMicroseconds(Length * 1);
  //  
  digitalWrite(PIN, HIGH); // AGC
  delayMicroseconds(8440);

  digitalWrite(PIN, LOW); // AGC
  delayMicroseconds(4220);

  bONE(); // 1 Start Bit

  bONE(); //       (7 bit device code)
  bONE();
  bONE(); // 7
  bZERO();

  bZERO();
  bZERO(); //4
  bONE();
}

void Postamble() {
  bONE();
  bONE(); // 2 stop bits
}
