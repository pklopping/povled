/// @file    DemoReel100.ino
/// @brief   FastLED "100 lines of code" demo reel, showing off some effects
/// @example DemoReel100.ino

#include <FastLED.h>

FASTLED_USING_NAMESPACE

// FastLED "100-lines-of-code" demo reel, showing just a few 
// of the kinds of animation patterns you can quickly and easily 
// compose using FastLED.  
//
// This example also shows one easy way to define multiple 
// animations patterns and have them automatically rotate.
//
// -Mark Kriegsman, December 2014


#define DATA_PIN    4
#define CLK_PIN     3
#define LED_TYPE    APA102
#define COLOR_ORDER GRB
//#define NUM_LEDS  144
#define NUM_LEDS    8

CRGB leds[NUM_LEDS];

uint8_t cur_image = 0;
uint8_t cur_row = 0;

int images[1][NUM_LEDS][NUM_LEDS] = {
  {
    {255, 0, 0, 0, 0, 0, 0, 255},
    {0, 255, 0, 0, 0, 0, 255, 0},
    {0, 0, 255, 0, 0, 255, 0, 0},
    {0, 0, 0, 255, 255, 0, 0, 0},
    {0, 0, 0, 255, 255, 0, 0, 0},
    {0, 0, 255, 0, 0, 255, 0, 0},
    {0, 255, 0, 0, 0, 0, 255, 0},
    {255, 0, 0, 0, 0, 0, 0, 255},
  }
};

#define BRIGHTNESS          96
#define FRAMES_PER_SECOND  120

void setup() {
  delay(3000); // 3 second delay for recovery
  FastLED.addLeds<LED_TYPE,DATA_PIN,CLK_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}


void loop()
{
  // Show the current frame

  // send the 'leds' array out to the actual LED strip
  FastLED.show();  
  // insert a delay to keep the framerate modest
//  FastLED.delay(1000/FRAMES_PER_SECOND); 

//  // do some periodic updates
//  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow
//  EVERY_N_SECONDS( 10 ) { nextPattern(); } // change patterns periodically
    EVERY_N_MILLISECONDS (1) {nextRow(); }
}

void nextRow()
{
  cur_row++;

  if (cur_row >= NUM_LEDS)
  {
    cur_row = 0;
    FastLED.delay(5);
  }

  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = images[cur_image][cur_row][i];
  }
}
