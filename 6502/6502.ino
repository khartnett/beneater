const char ADDR[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50, 52};
const char DATA[] = {39, 41, 43, 45, 47, 49, 51, 53};
//const char DATA[] = {3, 4, 5, 6, 7, 8, 9, 10};
#define CLOCK 27
#define READ_WRITE 29


int clockSpeed = 200;
int incomingByte = 0; // for incoming serial data
int runMode = 0;
int count = 0;
int cyclesLeft = 0;
const char *opCodes[] = {
  "BRK 7",
  "ORA izx 6",
  "*KIL",
  "*SLO izx 8",
  "*NOP zp 3",
  "ORA zp 3",
  "ASL zp 5",
  "*SLO zp 5",
  "PHP 3",
  "ORA imm 2",
  "ASL 2",
  "*ANC imm 2",
  "*NOP abs 4",
  "ORA abs 4",
  "ASL abs 6",
  "*SLO abs 6",
  "BPL rel 2*",
  "ORA izy 5*",
  "*KIL",
  "*SLO izy 8",
  "*NOP zpx 4",
  "ORA zpx 4",
  "ASL zpx 6",
  "*SLO zpx 6",
  "CLC 2",
  "ORA aby 4*",
  "*NOP 2",
  "*SLO aby 7",
  "*NOP abx 4*",
  "ORA abx 4*",
  "ASL abx 7",
  "*SLO abx 7",
  "JSR abs 6",
  "AND izx 6",
  "*KIL",
  "*RLA izx 8",
  "BIT zp 3",
  "AND zp 3",
  "ROL zp 5",
  "*RLA zp 5",
  "PLP 4",
  "AND imm 2",
  "ROL 2",
  "*ANC imm 2",
  "BIT abs 4",
  "AND abs 4",
  "ROL abs 6",
  "*RLA abs 6",
  "BMI rel 2*",
  "AND izy 5*",
  "*KIL",
  "*RLA izy 8",
  "*NOP zpx 4",
  "AND zpx 4",
  "ROL zpx 6",
  "*RLA zpx 6",
  "SEC 2",
  "AND aby 4*",
  "*NOP 2",
  "*RLA aby 7",
  "*NOP abx 4*",
  "AND abx 4*",
  "ROL abx 7",
  "*RLA abx 7",
  "RTI 6",
  "EOR izx 6",
  "*KIL",
  "*SRE izx 8",
  "*NOP zp 3",
  "EOR zp 3",
  "LSR zp 5",
  "*SRE zp 5",
  "PHA 3",
  "EOR imm 2",
  "LSR 2",
  "*ALR imm 2",
  "JMP abs 3",
  "EOR abs 4",
  "LSR abs 6",
  "*SRE abs 6",
  "BVC rel 2*",
  "EOR izy 5*",
  "*KIL",
  "*SRE izy 8",
  "*NOP zpx 4",
  "EOR zpx 4",
  "LSR zpx 6",
  "*SRE zpx 6",
  "CLI 2",
  "EOR aby 4*",
  "*NOP 2",
  "*SRE aby 7",
  "*NOP abx 4*",
  "EOR abx 4*",
  "LSR abx 7",
  "*SRE abx 7",
  "RTS 6",
  "ADC izx 6",
  "*KIL",
  "*RRA izx 8",
  "*NOP zp 3",
  "ADC zp 3",
  "ROR zp 5",
  "*RRA zp 5",
  "PLA 4",
  "ADC imm 2",
  "ROR 2",
  "*ARR imm 2",
  "JMP ind 5",
  "ADC abs 4",
  "ROR abs 6",
  "*RRA abs 6",
  "BVS rel 2*",
  "ADC izy 5*",
  "*KIL",
  "*RRA izy 8",
  "*NOP zpx 4",
  "ADC zpx 4",
  "ROR zpx 6",
  "*RRA zpx 6",
  "SEI 2",
  "ADC aby 4*",
  "*NOP 2",
  "*RRA aby 7",
  "*NOP abx 4*",
  "ADC abx 4*",
  "ROR abx 7",
  "*RRA abx 7",
  "*NOP imm 2",
  "STA izx 6",
  "*NOP imm 2",
  "*SAX izx 6",
  "STY zp 3",
  "STA zp 3",
  "STX zp 3",
  "*SAX zp 3",
  "DEY 2",
  "*NOP imm 2",
  "TXA 2",
  "*XAA imm 2",
  "STY abs 4",
  "STA abs 4",
  "STX abs 4",
  "*SAX abs 4",
  "BCC rel 2*",
  "STA izy 6",
  "*KIL",
  "*AHX izy 6",
  "STY zpx 4",
  "STA zpx 4",
  "STX zpy 4",
  "*SAX zpy 4",
  "TYA 2",
  "STA aby 5",
  "TXS 2",
  "*TAS aby 5",
  "*SHY abx 5",
  "STA abx 5",
  "*SHX aby 5",
  "*AHX aby 5",
  "LDY imm 2",
  "LDA izx 6",
  "LDX imm 2",
  "*LAX izx 6",
  "LDY zp 3",
  "LDA zp 3",
  "LDX zp 3",
  "*LAX zp 3",
  "TAY 2",
  "LDA imm 2",
  "TAX 2",
  "*LAX imm 2",
  "LDY abs 4",
  "LDA abs 4",
  "LDX abs 4",
  "*LAX abs 4",
  "BCS rel 2*",
  "LDA izy 5*",
  "*KIL",
  "*LAX izy 5*",
  "LDY zpx 4",
  "LDA zpx 4",
  "LDX zpy 4",
  "*LAX zpy 4",
  "CLV 2",
  "LDA aby 4*",
  "TSX 2",
  "*LAS aby 4*",
  "LDY abx 4*",
  "LDA abx 4*",
  "LDX aby 4*",
  "*LAX aby 4*",
  "CPY imm 2",
  "CMP izx 6",
  "*NOP imm 2",
  "*DCP izx 8",
  "CPY zp 3",
  "CMP zp 3",
  "DEC zp 5",
  "*DCP zp 5",
  "INY 2",
  "CMP imm 2",
  "DEX 2",
  "*AXS imm 2",
  "CPY abs 4",
  "CMP abs 4",
  "DEC abs 6",
  "*DCP abs 6",
  "BNE rel 2*",
  "CMP izy 5*",
  "*KIL",
  "*DCP izy 8",
  "*NOP zpx 4",
  "CMP zpx 4",
  "DEC zpx 6",
  "*DCP zpx 6",
  "CLD 2",
  "CMP aby 4*",
  "*NOP 2",
  "*DCP aby 7",
  "*NOP abx 4*",
  "CMP abx 4*",
  "DEC abx 7",
  "*DCP abx 7",
  "CPX imm 2",
  "SBC izx 6",
  "*NOP imm 2",
  "*ISC izx 8",
  "CPX zp 3",
  "SBC zp 3",
  "INC zp 5",
  "*ISC zp 5",
  "INX 2",
  "SBC imm 2",
  "NOP 2",
  "*SBC imm 2",
  "CPX abs 4",
  "SBC abs 4",
  "INC abs 6",
  "*ISC abs 6",
  "BEQ rel 2*",
  "SBC izy 5*",
  "*KIL",
  "*ISC izy 8",
  "*NOP zpx 4",
  "SBC zpx 4",
  "INC zpx 6",
  "*ISC zpx 6",
  "SED 2",
  "SBC aby 4*",
  "*NOP 2",
  "*ISC aby 7",
  "*NOP abx 4*",
  "SBC abx 4*",
  "INC abx 7",
  "*ISC abx",
};

const char bytesAndCycles[256][2] = {
  {0,0},
  {2,6},
  {0,0},
  {2,8},
  {2,3},
  {2,3},
  {2,5},
  {2,5},
  {1,3},
  {2,2},
  {1,2},
  {2,2},
  {3,4},
  {3,4},
  {3,6},
  {3,6},
  {2,3},
  {2,5},
  {0,0},
  {2,8},
  {2,4},
  {2,4},
  {2,6},
  {2,6},
  {1,2},
  {3,4},
  {1,2},
  {3,7},
  {3,4},
  {3,4},
  {3,7},
  {3,7},
  {0,6},
  {2,6},
  {0,0},
  {2,8},
  {2,3},
  {2,3},
  {2,5},
  {2,5},
  {1,4},
  {2,2},
  {1,2},
  {2,2},
  {3,4},
  {3,4},
  {3,6},
  {3,6},
  {2,2},
  {2,5},
  {0,0},
  {2,8},
  {2,4},
  {2,4},
  {2,6},
  {2,6},
  {1,2},
  {3,4},
  {1,2},
  {3,7},
  {3,4},
  {3,4},
  {3,7},
  {3,7},
  {0,6},
  {2,6},
  {0,0},
  {2,8},
  {2,3},
  {2,3},
  {2,5},
  {2,5},
  {1,3},
  {2,2},
  {1,2},
  {2,2},
  {0,3},
  {3,4},
  {3,6},
  {3,6},
  {2,3},
  {2,5},
  {0,0},
  {2,8},
  {2,4},
  {2,4},
  {2,6},
  {2,6},
  {1,2},
  {3,4},
  {1,2},
  {3,7},
  {3,4},
  {3,4},
  {3,7},
  {3,7},
  {0,6},
  {2,6},
  {0,0},
  {2,8},
  {2,3},
  {2,3},
  {2,5},
  {2,5},
  {1,4},
  {2,2},
  {1,2},
  {2,2},
  {0,5},
  {3,4},
  {3,6},
  {3,6},
  {2,2},
  {2,5},
  {0,0},
  {2,8},
  {2,4},
  {2,4},
  {2,6},
  {2,6},
  {1,2},
  {3,4},
  {1,2},
  {3,7},
  {3,4},
  {3,4},
  {3,7},
  {3,7},
  {2,2},
  {2,6},
  {2,2},
  {2,6},
  {2,3},
  {2,3},
  {2,3},
  {2,3},
  {1,2},
  {2,2},
  {1,2},
  {2,2},
  {3,4},
  {3,4},
  {3,4},
  {3,4},
  {2,3},
  {2,6},
  {0,0},
  {2,6},
  {2,4},
  {2,4},
  {2,4},
  {2,4},
  {1,2},
  {3,5},
  {0,2},
  {0,5},
  {3,5},
  {3,5},
  {3,5},
  {3,5},
  {2,2},
  {2,6},
  {2,2},
  {2,6},
  {2,3},
  {2,3},
  {2,3},
  {2,3},
  {1,2},
  {2,2},
  {1,2},
  {2,2},
  {3,4},
  {3,4},
  {3,4},
  {3,4},
  {2,2},
  {2,5},
  {0,0},
  {2,5},
  {2,4},
  {2,4},
  {2,4},
  {2,4},
  {1,2},
  {3,4},
  {1,2},
  {3,4},
  {3,4},
  {3,4},
  {3,4},
  {3,4},
  {2,2},
  {2,6},
  {2,2},
  {2,8},
  {2,3},
  {2,3},
  {2,5},
  {2,5},
  {1,2},
  {2,2},
  {1,2},
  {2,2},
  {3,4},
  {3,4},
  {3,6},
  {3,6},
  {2,3},
  {2,5},
  {0,0},
  {2,8},
  {2,4},
  {2,4},
  {2,6},
  {2,6},
  {1,2},
  {3,4},
  {1,2},
  {3,7},
  {3,4},
  {3,4},
  {3,7},
  {3,7},
  {2,2},
  {2,6},
  {2,2},
  {2,8},
  {2,3},
  {2,3},
  {2,5},
  {2,5},
  {1,2},
  {2,2},
  {1,2},
  {2,2},
  {3,4},
  {3,4},
  {3,6},
  {3,6},
  {2,2},
  {2,5},
  {0,0},
  {2,8},
  {2,4},
  {2,4},
  {2,6},
  {2,6},
  {1,2},
  {3,4},
  {1,2},
  {3,7},
  {3,4},
  {3,4},
  {3,7},
  {3,7},
};

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
  Serial.print(output);
  if (cyclesLeft <= 1) {
    Serial.print(" ");
    Serial.print(opCodes[data]);
    Serial.print(" bytes: ");
    Serial.print(bytesAndCycles[data][0], DEC);
    Serial.print(" cycles: ");
    Serial.println(bytesAndCycles[data][1], DEC);
    cyclesLeft = bytesAndCycles[data][1]; // comment this out to ignore cycles
  } else {
    Serial.print(" --");
    cyclesLeft--;
    Serial.println(cyclesLeft, DEC);
  }

  if (address == 0xfffd) {
      count = 0;
      cyclesLeft = 0;
      Serial.println("Counter Reset.");
  }
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
      cyclesLeft = 0;
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
