#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif
#include <SoftwareSerial.h>

#define AUX1 2
#define AUX2 3
#define AUX3 4
#define AUX4 5
#define MAINBODY 6

Adafruit_NeoPixel mainBody = Adafruit_NeoPixel(18, MAINBODY, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel aux1 = Adafruit_NeoPixel(9, AUX1, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel aux2 = Adafruit_NeoPixel(9, AUX2, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel aux3 = Adafruit_NeoPixel(9, AUX3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel aux4 = Adafruit_NeoPixel(9, AUX4, NEO_GRB + NEO_KHZ800);

Adafruit_NeoPixel strips[] = { mainBody, aux1, aux2, aux3, aux4 };
unsigned char numStrips = 5;
unsigned char serialReadCount = 0;
int incomingByte = 0;

SoftwareSerial mySerial(10, 11);

void setup() {
  for (unsigned char i = 0; i < numStrips; i++) {
    strips[i].begin();
  }
  Serial.begin(9600);
  mySerial.begin(9600);
  randomSeed(analogRead(0));
}

int red = 255;
int green = 255;
int blue = 255;
int wait = 50;
int zeroCount = 0;

void loop() {
  
  if (mySerial.available() > 0) {
    zeroCount = 0;
    switch (serialReadCount) {
      case 0:
        red = mySerial.read();
        serialReadCount++;
        Serial.print("RED: ");
        Serial.println(red, DEC);  
        break;
      case 1:
        green = mySerial.read();
        serialReadCount++;
        Serial.print("GREEN: ");
        Serial.println(red, DEC); 
        break;
      case 2:
        blue = mySerial.read();
        serialReadCount++;
        Serial.print("BLUE: ");
        Serial.println(red, DEC); 
        break;
    }   
  }

  red = red - (random(300) % 256);
  blue = blue - (random(300) % 256);
  green = green - (random(300) % 256);

  if (serialReadCount == 3) {
    colorWipe(mainBody.Color(red, green, blue), 1, 50);
    serialReadCount = 0;
  }
  
}

void colorWipe(uint32_t c, uint8_t wait, uint8_t brightness) {
  for(uint16_t i = 0; i < mainBody.numPixels(); i++) {
    for (unsigned char j = 0; j < numStrips; j++) {
      strips[j].setPixelColor(i, c);
      strips[j].show();
      strips[j].setBrightness(brightness);
    }
    delay(wait);
  }
}
