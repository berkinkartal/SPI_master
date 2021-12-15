#define CS 10
#define MOSI 11
#define MISO 12
#define CLK 13
#define LED 14

#include <SPI.h>
#include <inttypes.h>

/* SPCR byte setup (as per SPI specifications): SPIE|SPE|DORD|MSTR|CPOL|CPHA|SPR1|SPR2
    bit8 SPIE= 1 enable SPI interrupt
    bit7 SPE= 1 enable SPI
    bit6 DORD= 1 LSB first - 0 MSB first
    bit5 MSTR= 1 set as Master - 0 set as SLAVE
    bit 4 CPOL= 1 clock idle HIGH - idle LOW
    bit3 CHPA= 1 sample data on FALLING - 0 on RISING
    bit2+1 SPR1+SPR2= 00 speed fastest - 11 slowest

   Now different way to setup and start SPI . I choosed the direct register write within the main loop()
   SPCR = (1 << SPE); // Enable SPI as slave.
   SPI.beginTransaction(SPISettings(16000000,MSBFIRST,SPI_MODE0));
*/
const char * hwrld = "Hello, world!\n";
uint32_t t_data = 0x12345678;
static uint32_t buf[6] = {0x00580005, 0x00580006, 0x00580007, 0x00580008, 0x00580009, 0x0058000A};

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT);
  pinMode(MOSI, OUTPUT);
  pinMode(MISO, INPUT);
  pinMode(CLK, OUTPUT);
  pinMode(CS, OUTPUT);
  digitalWrite(CS, HIGH);
  //  SPI.begin();
  SPCR = 0b01010011;
//  SPSR |= 0b00000001;
  SPSR |= bit(SPI2X);
  Serial.begin(9600);
  
  //Set everything high, especially the CS to let the ADF4350 reset its comms before starting talking.
  //  digitalWrite(CS, HIGH);
  //  digitalWrite(CLK, LOW);
  //  digitalWrite(MOSI, LOW);
/*
  for (int i(0); i < 10; ++i) {
    delay(100);
    digitalWrite(LED, !digitalRead(LED));
  }
*/
  /*
    if (true) {
      shiftWord_byte(0x11);
    //    shiftWord(0x00580005);  // register 5
    //    shiftWord(0x008506FC);    // new register 4
    //    shiftWord(0x000084B3);    // new register 3
    //    shiftWord(0x04004E42);    // register 2
    //    shiftWord(0x08008191);    // new register 1
    //    shiftWord(0x00C40038);    // new register 0
    }
  */
}


void shiftWord(int32_t w) {
  shiftCMD((w >> 24) & 0xFF, (w >> 16) & 0xFF, (w >> 8) & 0xFF, w & 0xFF);
}

void shiftCMD(byte A, byte B, byte C, byte D) {
//  SPCR = 0b01010011;
  digitalWrite(CS, LOW);
  SPI.transfer(A);
//  SPI.transfer(0xA);
  digitalWrite(CS, HIGH);
  digitalWrite(CS, LOW);
  SPI.transfer(B);
//  SPI.transfer(0xA);
  digitalWrite(CS, HIGH);
  digitalWrite(CS, LOW);
  SPI.transfer(C);
//  SPI.transfer(0xA);
  digitalWrite(CS, HIGH);
  digitalWrite(CS, LOW);
  SPI.transfer(D);
//  SPI.transfer(0xA);
  digitalWrite(CS, HIGH);
  digitalWrite(CS, LOW);
  SPI.transfer(0xA);
  digitalWrite(CS, HIGH);
  delay(10);
//  SPCR = 0b00000011;
  //  shiftOut(MOSI, CLK, MSBFIRST, A);
  //  shiftOut(MOSI, CLK, MSBFIRST, B);
  //  shiftOut(MOSI, CLK, MSBFIRST, C);
  //  shiftOut(MOSI, CLK, MSBFIRST, D);
//  Serial.print(F("data="));
//  Serial.print(A);
//  Serial.print(B);
//  Serial.print(C);
//  Serial.println(D);
}

void send_char_spi(const char *p){
  char c;
  digitalWrite(CS, LOW);    // SS is pin 10

  for (p; c = *p; p++)
    SPI.transfer (c);

  digitalWrite(CS, HIGH);
}


void loop() {
//  char i;
//  char buf1[10];
  digitalWrite(LED, !digitalRead(LED));
  //  shiftWord_byte(0x44);
  //  shiftWord_byte(0b00001001);
  //for (i = 0; i < 6; i++){
 //   shiftWord(buf[i]);
//    sprintf(buf1, "%0x", t_data);
    delay(1000);
    shiftWord(t_data);

//    t_data[0] = t_data[0] & 0xFFFFFFFF;
//    Serial.print(buf1);
 // }
//    delay (1000);  // 1 seconds delay
//  shiftWord(0x41424344);
//  send_char_spi(hwrld);
  //  digitalWrite(CS, LOW);
  //  SPI.transfer(0b11100111);
  //  digitalWrite(CS, HIGH);

}

void shiftWord_byte(byte w) {
  shiftCMD_byte(w & 0xFF);
}

void shiftCMD_byte(byte A) {
  digitalWrite(CS, LOW);
  //  shiftOut(MOSI, CLK, MSBFIRST, A);
  SPI.transfer(A);
//  Serial.print(F("data="));
//  Serial.println(A);
  digitalWrite(CS, HIGH);
}
