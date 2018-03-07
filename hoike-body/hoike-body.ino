#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

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

void setup() {
  for (unsigned char i = 0; i < numStrips; i++) {
    strips[i].begin();
  }
  
}

void loop() {

  unsigned char red = 255;
  unsigned char green = 255;
  unsigned char blue = 255;
  unsigned char wait = 50;
  
  colorWipe(mainBody.Color(red, green, blue), 50, 255);
  colorWipe(mainBody.Color(255, 0, 0), 50, 100);
  colorWipe(mainBody.Color(255, 0, 255), 50, 50);
  colorWipe(mainBody.Color(255, 255, 0), 50, 5);
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
