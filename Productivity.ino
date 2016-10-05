#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
#include <Tone.h>
#include "Mappings.h"

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

uint8_t numIndex[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
//uint8_t leftdigit = 8, rightdigit = 9;

Tone tone1;
unsigned long runTime;

void setup() {
  matrix.begin(0x70);
  matrix.setRotation(3);
  matrix.setBrightness(1);
  matrix.setTextColor(LED_ON);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.clear();
  matrix.writeDisplay();
}

void loop() {

  // Max number 25
  uint8_t currentnum = 25;
  
  // Find left digit
  uint8_t leftdigit = 0, n = maxnum;
  while (n/10 > 0) {
    leftdigit++;
    n -= 10;
  }
  // Find right digit
  uint8_t rightdigit = maxnum % 10;
  
  matrix.clear();

  const uint8_t *left_matrix = numbers[leftdigit];
  const uint8_t *right_matrix = numbers[rightdigit];
  
  matrix.drawBitmap(-5, 0, left_matrix, 8, 8, LED_ON);
  matrix.drawBitmap(-1, 0, right_matrix, 8, 8, LED_ON);

  matrix.writeDisplay();
  delay(500);
}
