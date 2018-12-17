//Vít Petřík@2018
//SCL = pin 7
//SDA = pin 5
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))

#include <QTouchADCTiny.h>
#include <TinyWireS.h>

#define I2C_SLAVE_ADDRESS 0x04  //I2C adresa

#define refPin PB5              //piny
#define sensePin PB4
#define ledPin PB1

int ref = 0;
int value;

void setup() {
  pinMode(ledPin, OUTPUT);
  asm (" ldi PORTB1, 1 ");
  delay(50);
  QTouchADCTiny.init();
  
  TinyWireS.begin(I2C_SLAVE_ADDRESS);
  TinyWireS.onReceive(receiveEvent);
  TinyWireS.onRequest(requestEvent);
}

void loop() {
 
}

//když master pošle data, tak je hodíme na LEDku jako PWM
void receiveEvent(uint8_t byty)
{
  byte pwmValue = TinyWireS.receive();
  if ( pwmValue == 0 ){
    asm (" ldi PORTB1, 1 ");
  }
  else if ( pwmValue == 255 ){
    asm (" ldi PORTB1, 1 ");
  }
  else{
    sbi(TCCR0A, COM0B1);
    OCR0B = pwmValue;
  }
}

//když si master vyžádá data, tak mu pošleme číslo value jako dva byty
void requestEvent()
{ 
  value = QTouchADCTiny.sense(sensePin, refPin, 1);
  TinyWireS.send(highByte(value));
  TinyWireS.send(lowByte(value));
}
