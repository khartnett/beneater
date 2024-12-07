const char ADDR[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
const char DATA[] = {39, 41, 43, 45, 47, 49, 51, 53};
//const char DATA[] = {3, 4, 5, 6, 7, 8, 9, 10};
#define CLOCK 27
#define READ_WRITE 29


int clockSpeed = 200;
int incomingByte = 0; // for incoming serial data
int runMode = 0;
int count = 0;

void setup() {
  for (int n = 0; n < 16; n += 1) {
    pinMode(ADDR[n], INPUT);
  }
  for (int n = 0; n < 8; n += 1) {
    pinMode(DATA[n], INPUT);
  }
  pinMode(READ_WRITE, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(CLOCK, OUTPUT);

  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(CLOCK, LOW);
  
  //Serial.begin(57600);
  Serial.begin(9600);
  // instructions
  Serial.println("Type 's' to start/stop. Use Enter when stopped to step.");
  Serial.println("Type 'f' to run faster. Type 'd' to run slower. Type 'r' to reset counter.");
}

void pulse(int clockspeed) {  
  digitalWrite(LED_BUILTIN, HIGH);
  digitalWrite(CLOCK, HIGH);
  delay(clockspeed);                       // wait for a second
  scan();
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(CLOCK, LOW);
  count++;
  delay(clockspeed);                       // wait for a second
}

void scan() {
  char output[15];
  Serial.print(count);
  Serial.print("- ");

  unsigned int address = 0;
  for (int n = 0; n < 16; n += 1) {
    int bit = digitalRead(ADDR[n]) ? 1 : 0;
    Serial.print(bit);
    address = (address << 1) + bit;
  }
  
  Serial.print("   ");
  
  unsigned int data = 0;
  for (int n = 0; n < 8; n += 1) {
    int bit = digitalRead(DATA[n]) ? 1 : 0;
    Serial.print(bit);
    data = (data << 1) + bit;
  }

  sprintf(output, "   %04x  %c %02x", address, digitalRead(READ_WRITE) ? 'r' : 'W', data);
  Serial.println(output);  
}

void loop() {
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.read();

    // Use this to find key codes:
    //Serial.print("Input received: ");
    //Serial.println(incomingByte, DEC);
    if (incomingByte == 115) { // 's'
      runMode = !runMode;
      while (!Serial.available()) {
        ;// wait for 'return'
      }
      incomingByte = Serial.read(); // eat 'Enter' key
      incomingByte = 0;
      if (runMode) {
        Serial.println("Run Mode: Start");
      } else {
        Serial.println("Run Mode: Stop");
      }
    }
    
    if (incomingByte == 102) { // 'f'
      while (!Serial.available()) {
        ;// wait for 'return'
      }
      incomingByte = Serial.read(); // eat 'Enter' key
      incomingByte = 0;
      clockSpeed = clockSpeed / 2;
      Serial.print("Run Speed up, delay: ");
      Serial.println(clockSpeed);
    }
    if (incomingByte == 100) { // 'd'
      while (!Serial.available()) {
        ;// wait for 'return'
      }
      incomingByte = Serial.read(); // eat 'Enter' key
      incomingByte = 0;
      clockSpeed = clockSpeed * 2;
      Serial.print("Run Speed down, delay: ");
      Serial.println(clockSpeed);
    }
    if (incomingByte == 114) { // 'r'
      while (!Serial.available()) {
        ;// wait for 'return'
      }
      incomingByte = Serial.read(); // eat 'Enter' key
      incomingByte = 0;
      count = 0;
      Serial.println("Counter Reset.");
    }
   
    if (incomingByte == 10 && !runMode) { // 'Enter'
      pulse(10);
    }
  }
  // continueous
  if (runMode) {
    pulse(clockSpeed);
  }
}

