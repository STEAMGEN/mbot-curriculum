// mbot program
// flash LEDs alternating red/blue
// siren sound
// drive while
//	a) line following (stay on black)
//	b) obstical avoidance
//	c) don't fall off table (stay on white)
//	determine drive mode by state at start
// eye look around as appropriate
//

// http://forum.makeblock.cc/t/mbot-arduino-ide-examples/1537
// I started to play with my mBot using Arduino IDE (since mBlock is not an option on Linux at this time)
// I just wanted to share a few examples I wrote. Maybe they will be useful to someone.
// They are packaged as a fork of the Makeblock library so install should be quite straightforward.
//
// The release is there: https://github.com/Oaz/Makeblock-Library/releases/tag/A

// mCore based on Arduino Uno
// http://www.makeblock.cc/mcore-main-control-board-for-mbot/

// makeblock library for Arduino IDE
// http://learn.makeblock.com/Makeblock-library-for-Arduino/

// mcore schematic
// http://learn.makeblock.com/mcore/

// how to generate a siren sound
// http://www.ehow.com/how_8222908_generate-siren-sound.html
// play C, then drop to G; or play D and drop to A  (perfect forth below original note)

#include "pt.h"

#include "MeMCore.h"

#include "array.h"

#include <Adafruit_NeoPixel.h>
// Parameter 1 = number of pixels in strip
// Parameter 2 = pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(2, 13, NEO_GRB + NEO_KHZ800);

MeLineFollower lineFinder(PORT_2);

MeLEDMatrix ledMx(PORT_4);

struct pt	lightState,
			sirenState,
			driveState,
			eyesState;

void setup()
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  ledMx.setBrightness(4); // set matrix brightness
  ledMx.setColorIndex(1);

  randomSeed(analogRead(5));  // light sensor

	PT_INIT(&lightState);
	PT_INIT(&sirenState);
	PT_INIT(&driveState);
	PT_INIT(&eyesState);
}

volatile int lineSensorState;

void loop()
{
	// wait for button press?
	
	while (1)
	{
    // both eyes and drive need this, read it only once
    lineSensorState = lineFinder.readSensors();

		lightThread();
		sirenThread();
		driveThread();
		eyesThread();
	}
}

#define LIGHT_DELAY 150
#define LIGHT_BRIGHT 128

PT_THREAD(lightThread(void))
{
	static int		state;
	static unsigned long	delayStart;
  static int  delay;
  	
	PT_BEGIN(&lightState);
	state = 0;
	while (1)
	{
		switch(state)
		{
			case 0:
      case 2:
        // left LED red
        // right LED blue
        strip.setPixelColor(0, LIGHT_BRIGHT,0,0);
        strip.setPixelColor(1, 0,0,LIGHT_BRIGHT);
        delay = LIGHT_DELAY;
				break;
				
			case 4:
      case 6:
				// left LED blue
				// right LED red
        strip.setPixelColor(0, 0,0,LIGHT_BRIGHT);
        strip.setPixelColor(1, LIGHT_BRIGHT,0,0);
        delay = LIGHT_DELAY;
				break;

      case 1:
      case 5:
        strip.setPixelColor(0, 0,0,0);
        strip.setPixelColor(1, 0,0,0);
        delay = 25;
        break;

      case 3:
      case 7:
        strip.setPixelColor(0, 0,0,0);
        strip.setPixelColor(1, 0,0,0);
        delay = 100;
        break;
		}

    strip.show();
		delayStart = millis();
		PT_WAIT_WHILE(&lightState, millis() - delayStart < delay);

    state = (++state % 8);
	}
	PT_END(&lightState);
}

#define SIREN_DELAY (60000/55)
#define SIREN_PIN 8
PT_THREAD(sirenThread(void))
{
	static int		state;
	static unsigned long	delayStart;
	int freq;
  static int period = 1000000L / freq;
  static int pulse = period / 2;
  static unsigned long i;
  
	PT_BEGIN(&sirenState);
	state = 0;
  pinMode(SIREN_PIN, OUTPUT);

	while (1)
	{
    // http://upcommons.upc.edu/bitstream/handle/2117/18181/Neuwsirentones.pdf
    // french police 435-580 @ 55 cycles/min
		switch(state)
		{
			case 0:
        freq = 580;  // D = 587
				break;
				
			case 1:
        freq = 435;  // A = 440
				break;
		}
		state = (++state % 2);

    period = 1000000L / freq;
    pulse = period / 2;
    for (i = 0; i < (SIREN_DELAY/2) * 1000L; i += period) 
    {
      digitalWrite(SIREN_PIN, HIGH);
      delayStart = micros();
      PT_WAIT_WHILE(&sirenState, micros() - delayStart < pulse);
      digitalWrite(SIREN_PIN, LOW);
      delayStart = micros();
      PT_WAIT_WHILE(&sirenState, micros() - delayStart < pulse);
    }
  }
	PT_END(&sirenState);
}

MeDCMotor motorL(M1);
MeDCMotor motorR(M2);
#define MOTOR_SPEED 175
PT_THREAD(driveThread(void))
{
  static int  lastTurn = 1;
  static int lastLineSensorState;
  
  PT_BEGIN(&driveState);
  while (1)
  {
    switch(lineSensorState)
    {
      case S1_IN_S2_IN: // both on the black line
        motorL.run(-MOTOR_SPEED);
        motorR.run(MOTOR_SPEED);
        break;
      case S1_IN_S2_OUT:  // left in, right out
        motorL.run(-MOTOR_SPEED/2);
        motorR.run(MOTOR_SPEED*2);
        lastTurn = 1;
        break;
      case S1_OUT_S2_IN:  // left out, right in
        motorL.run(-MOTOR_SPEED*2);
        motorR.run(MOTOR_SPEED/2);
        lastTurn = -1;
        break;
      case S1_OUT_S2_OUT: // off the line entirely
        motorL.run(lastTurn * MOTOR_SPEED/2);
        motorR.run(lastTurn * MOTOR_SPEED/2);
        break;
    }

    // wait until our input changes, then react
    lastLineSensorState = lineSensorState;
    PT_WAIT_WHILE(&driveState, lastLineSensorState == lineSensorState);
  }
  PT_END(&driveState);
}

Array<uint8_t> eyeQueue(10);

uint8_t eyeOpenSequence[] = {0x00, 0x11, 0x22};
uint8_t eyeCloseSequence[] = {0x22, 0x11, 0x00};
uint8_t eyeBlinkSequence[] = {0x22, 0x11, 0x00, 0x00, 0x11, 0x22};
uint8_t eyeRightWinkSequence[] = {0xF2, 0xF1, 0xF0, 0xF0, 0xF1, 0xF2};
uint8_t eyeLeftWinkSequence[] = {0x2F, 0x1F, 0x0F, 0x0F, 0x1F, 0x2F};

PT_THREAD(eyesThread(void))
{
  static uint8_t  eyeState, 
        prevState;
  static unsigned long  delayStart, 
        lastBlinkTime;
  
  PT_BEGIN(&eyesState);
  eyeSet(0x00); // start with eyes closed
  for (int i = 0; i < sizeof(eyeOpenSequence); i++)
  {
    eyeQueue.push(eyeOpenSequence[i]);
    eyeQueue.push(eyeOpenSequence[i]);  // push 2 copies to make opening slower
  }
  
  while (1)
  {
    // IN and OUT refer to being inside or outside the black line
    switch(lineSensorState)
    {
      case S1_IN_S2_IN: // both on the black line
        eyeState = 0x33;  // eyes forward
        break;
      case S1_IN_S2_OUT:  // left in, right out
        eyeState = 0x66;  // eyes LR
        break;
      case S1_OUT_S2_IN:  // left out, right in
        eyeState = 0x88;  // eyes LL
        break;
      case S1_OUT_S2_OUT: // off the line entirely
        eyeState = 0xCD;  // angry eyes
        break;
    }

    if (!eyeQueue.isEmpty())
    {
      eyeSet(eyeQueue.shift());
      delayStart = millis();
      PT_WAIT_WHILE(&eyesState, millis() - delayStart < 50);
    }
    else
    {
      if (eyeState != prevState)
      {
        eyeSet(eyeState);
        prevState = eyeState;
      }
  
      // humans blink every 3-4 seconds (15-20 times per minute)
      if (millis() - lastBlinkTime > random(2500, 4500))
      {
        for (int i = 0; i < sizeof(eyeBlinkSequence); i++)
          eyeQueue.push(eyeBlinkSequence[i]);
  
        eyeQueue.push(prevState); // and restore our current state
        
        lastBlinkTime = millis();
      }
      
      // update eyes 10 times per second
      delayStart = millis();
      PT_WAIT_WHILE(&eyesState, millis() - delayStart < 100);
    }
  }

  PT_END(&eyesState);
}

/*
 * Graphics for the eyes on the matrix display
 * Each graphic represents one eyeball so left and right can be independent
 * 
 * NOTE - there are only 16 possibilities so we can store each index in a nibble
 * that keeps our storage size low
 * 
 * The 0x0F at the end is a special case that we use to indicate a NOP in some patterns
 * it basically means don't do anything to this eye
 */
uint8_t eyeGraphics[][8] =
{
  {0x00,0x18,0x18,0x18,0x18,0x18,0x18,0x00},  // 0 closed
  {0x00,0x18,0x18,0x24,0x24,0x18,0x18,0x00},  // 1 quarter open
  {0x00,0x18,0x24,0x42,0x42,0x24,0x18,0x00},  // 2 three-quarter open
  {0x00,0x3C,0x42,0x5A,0x5A,0x42,0x3C,0x00},  // 3 pupil center
  {0x00,0x38,0x44,0x42,0x62,0x72,0x3C,0x00},  // 4 pupil UR
  {0x00,0x18,0x24,0x42,0x5A,0x5A,0x3C,0x00},  // 5 pupil R
  {0x00,0x1C,0x22,0x42,0x46,0x4E,0x3C,0x00},  // 6 pupil LR
  {0x00,0x1C,0x22,0x4E,0x4E,0x22,0x1C,0x00},  // 7 pupil down
  {0x00,0x3C,0x4E,0x46,0x42,0x22,0x1C,0x00},  // 8 pupil LL
  {0x00,0x3C,0x5A,0x5A,0x42,0x24,0x18,0x00},  // 9 pupil L
  {0x00,0x3C,0x72,0x62,0x42,0x44,0x38,0x00},  // A pupil UL
  {0x00,0x38,0x44,0x72,0x72,0x44,0x38,0x00},  // B pupil up
  {0x00,0x3C,0x22,0x52,0x2E,0x16,0x08,0x00},  // C angry left eye
  {0x00,0x08,0x16,0x2E,0x52,0x22,0x3C,0x00},  // D angry right eye
  {0x00,0x3C,0x42,0x42,0x42,0x42,0x3C,0x00}   // E full open no pupil
  // F = nop, leave as is
};

void eyeSet(uint8_t state)
{
    if ((state >> 4) != 0x0F)
      ledMx.drawBitmap(0, 0, 8, eyeGraphics[state >> 4]);  // left eye
    if ((state & 0x0F) != 0x0F)
      ledMx.drawBitmap(8, 0, 8, eyeGraphics[state & 0x0F]);  // right eye
}

