#include <Arduino.h>
#include <Wire.h>
#include "Adafruit_LEDBackpack.h"
#include "Adafruit_GFX.h"
//#include <Tone.h>
//#include <OrangutanBuzzer.h>
#include "Mappings.h"

Adafruit_8x8matrix matrix = Adafruit_8x8matrix();

// Variables
enum state {STUDY_ST, SHORT_REST_ST, LONG_REST_ST};
state curr_state;
uint8_t blocks_completed = 0, progressNum = 0;

//Tone tone1;
//OrangutanBuzzer buzzer;
//unsigned char currentIdx = 0;
//#define MELODY_LENGTH 95

// Record runtime
unsigned long startTime;
unsigned long currentTime;

// Timers
int8_t STUDY = 25;
int8_t SHORT_REST = 5;
int8_t LONG_REST = 20;
int8_t current_timer = 0;

void setup() {
  matrix.begin(0x70);
  matrix.setRotation(3);
  matrix.setBrightness(1);
  matrix.setTextColor(LED_ON);
  matrix.setTextSize(1);
  matrix.setTextWrap(false);
  matrix.clear();
  matrix.writeDisplay();
//  tone1.begin(10);

  // Start in STUDY state
  // Initialize timer to 25 mins
  curr_state = STUDY_ST;
  startTime = millis();
  reset_timer(STUDY_ST);
}

void loop() {
  update();

  currentTime = millis();
  // Perform every minute
  if (((currentTime - startTime) >= 200) && current_timer >= 0) {
    startTime = currentTime;
    current_timer--;
  }

  // If time block has ended
  if (current_timer < 0)
    end_block();

//  if (currentIdx < MELODY_LENGTH && !buzzer.isPlaying())
//  {
//    // play note at max volume
//    buzzer.playNote(note[currentIdx], duration[currentIdx], 8);
//    currentIdx++;
//  }
//  if (currentIdx == MELODY_LENGTH)
//    currentIdx = 0;
  
}

// Reset current timer according to the given state
void reset_timer(enum state) {
  // Go to appropriate state
  switch (curr_state) {
    case STUDY_ST:
      current_timer = STUDY;
      break;
    case SHORT_REST_ST:
      current_timer = SHORT_REST;
      break;
    case LONG_REST_ST:
      current_timer = LONG_REST;
      break;
  }
}

// Once a block state has ended, switch to new state
void end_block() {
  
  switch (curr_state) {  
    case STUDY_ST:
      // Increment blocks completed
      blocks_completed++;
    
      // Every 4 blocks, take a long rest
      if (blocks_completed % 4 == 0) {
        curr_state = LONG_REST_ST;
        reset_timer(LONG_REST);
      }
      // Else, take a short rest
      else {
        curr_state = SHORT_REST_ST;
        reset_timer(SHORT_REST);
      }
      break;
    case SHORT_REST_ST:
      curr_state = STUDY_ST;
      reset_timer(STUDY);
      break;
    case LONG_REST_ST:
      curr_state = STUDY_ST;
      reset_timer(STUDY);
      break;
  }
}

// Update matrix display
void update() {

  // Find left digit
  uint8_t leftdigit = 0;
  int8_t n = current_timer;
  while (n/10 > 0) {
    leftdigit++;
    n -= 10;
  }
  // Find right digit
  uint8_t rightdigit = current_timer % 10;

  // Update matrix
  matrix.clear();
  const uint8_t *left_matrix = numbers[leftdigit];
  const uint8_t *right_matrix = numbers[rightdigit];
  matrix.drawBitmap(-5, 0, left_matrix, 8, 8, LED_ON);
  matrix.drawBitmap(-1, 0, right_matrix, 8, 8, LED_ON);

  // Display number of blocks completed
  for (uint8_t i = 0; i < blocks_completed && i < 8; i++)
    matrix.drawPixel(i, 6, LED_ON);

  // Display progress of current STUDY block
  progressNum = 8 - (current_timer / 3);
  for (uint8_t j = 0; j < progressNum; j++)
    matrix.drawPixel(j, 7, LED_ON);  

  // Indicator that in STUDY state
  if (curr_state == STUDY_ST)
    matrix.drawPixel(7, 5, LED_ON);
  matrix.writeDisplay();
}



