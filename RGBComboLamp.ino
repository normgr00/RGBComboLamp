#include <FastLED.h>
#include <Bounce.h>

#define DATA_PIN 11
#define CLOCK_PIN 13
#define NUM_LEDS    32
#define LED_TYPE    LPD8806
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
#define FREQUENCY     200  // only for lightning - controls the interval between strikes
#define FLASHES       8                 // only for lightning - the upper limit of flashes per strike


// COOLING: How much does the air cool as it rises?
// Less cooling = taller flames.  More cooling = shorter flames.
// Default 50, suggested range 20-100
#define COOLING  50   // for fire

// SPARKING: What chance (out of 255) is there that a new spark will be lit?
// Higher chance = more roaring fire.  Lower chance = more flickery fire.
// Default 120, suggested range 50-200.
#define SPARKING 100    // for fire

const int speedPotPin = 0;
int potVal = 0;
int UPDATES_PER_SECOND = (potVal);

const int brightnessPotPin = 1;
int brightnessReading = 0;
int BRIGHTNESS = (brightnessReading);

const int veriableColorPotPin0 = 2;
const int veriableColorPotPin1 = 3;
const int veriableColorPotPin2 = 4;
const int veriableColorPotPin3 = 5;

const int paternSelectionSwitchPin = 3;
Bounce pushbutton = Bounce(paternSelectionSwitchPin, 10);  // 10 ms debounce
int paternSelection = 0;

CRGBPalette16 currentPalette;
TBlendType    currentBlending;
//extern CRGBPalette16 myRedWhiteBluePalette;
//extern const TProgmemPalette16 myRedWhiteBluePalette_p PROGMEM;

byte previousState = HIGH;         // what state was the button last time
unsigned int counter = 0;            // how many times has it changed to low
unsigned long countAt = 0;         // when count changed
unsigned int countPrinted = 0;     // last count printed


void setup() {
  delay( 2000 ); // power-up safety delay
  //Serial.begin(9600);
  pinMode(paternSelectionSwitchPin, INPUT);
  digitalWrite (paternSelectionSwitchPin, HIGH);

  FastLED.addLeds<LED_TYPE, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  SetupVeriablePalette(); //currentPalette = RainbowColors_p;
}



void loop()
{
  potVal = map(analogRead(speedPotPin), 0, 1023, 250, 1);
  UPDATES_PER_SECOND = (potVal);

  brightnessReading = map(analogRead(brightnessPotPin), 0, 1023, 1, 255);  //works
  BRIGHTNESS = (brightnessReading);

  if (pushbutton.update()) {
    if (pushbutton.fallingEdge()) {
      counter = counter + 1;
      countAt = millis();
    }
  }
  else {
    if (counter != countPrinted) {
      unsigned long nowMillis = millis();
      if (nowMillis - countAt > 100) {
        countPrinted = counter;
      }
      if (counter > 12) {
        counter = 0;

      }
    }

  }

  paternSelection = counter;

  switch (paternSelection)  {
    case 0:
      SetupVeriablePalette(); //SetupVeriableAndGreenPalette();
      currentBlending = BLEND;
      palletteStuff();  //test
      break;
    case 1:
      SetupVeriablePalette2(); //SetupVeriableAndGreenPalette();
      currentBlending = BLEND;
      palletteStuff();  //test
      break;
    case 2:
      currentPalette = RainbowColors_p;
      currentBlending = BLEND;
      palletteStuff();  //test
      break;
    case 3:
      SetupVeriablePalette();
      currentBlending = BLEND;
      palletteStuff();  //test
      break;
    case 4:
      currentPalette = RainbowStripeColors_p;
      currentBlending = BLEND;
      palletteStuff();  //test
      break;
    case 5:
      SetupPurpleAndGreenPalette();
      currentBlending = BLEND;
      palletteStuff();  //test
      break;
    case 6:
      currentPalette = CloudColors_p;
      currentBlending = BLEND;
      palletteStuff();  //test
      break;
    case 7:
      currentPalette = PartyColors_p;
      currentBlending = BLEND;
      palletteStuff();  //test
      break;
    case 8:
      currentPalette = ForestColors_p;
      currentBlending = BLEND;
      palletteStuff();  //test
      break;
    case 9:
      currentPalette = OceanColors_p;
      currentBlending = BLEND;
      palletteStuff();  //test
      break;
    case 10:
      currentPalette = ForestColors_p;
      currentBlending = BLEND;
      palletteStuff();  //test
      break;
    case 11:
      currentPalette = LavaColors_p;
      currentBlending = BLEND;
      palletteStuff();  //test
      break;
    case 12:
      rainbow2();
      break;
    case 13:
      fire();
      break;
    default:
      SetupVeriablePalette();
      currentBlending = BLEND;
      palletteStuff();  //test
      break;
  }

  {
  }
}

void palletteStuff()
{
  static uint8_t startIndex = 0;
  startIndex = startIndex + 1; /* motion speed */
  FillLEDsFromPaletteColors( startIndex);
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}


void FillLEDsFromPaletteColors( uint8_t colorIndex)
{
  for ( int i = 0; i < NUM_LEDS; i++) {
    leds[i] = ColorFromPalette( currentPalette, colorIndex, BRIGHTNESS, currentBlending);
    colorIndex += 3; // Defines the width of the collor bands - Dynamic changes in this are problomatic - flickering
  }
}


// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.  All are shown here.

// This function sets up a palette of black and white stripes,
// using code.  Since the palette is effectively an array of
// sixteen CRGB colors, the various fill_* functions can be used
// to set them up.

// This function sets up a palette of purple and green stripes.
void SetupPurpleAndGreenPalette()
{
  CRGB purple = CHSV( HUE_PURPLE, 255, 255);
  CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     green,  green,  black,  black,
                     purple, purple, black,  black,
                     green,  green,  black,  black,
                     purple, purple, black,  black );
}


void SetupVeriablePalette()
{
  CRGB veriable0 = CHSV( map(analogRead(veriableColorPotPin0), 0, 1023, 0, 255), 255, 255);  //240 turns to red again
  CRGB veriable1 = CHSV( map(analogRead(veriableColorPotPin1), 0, 1023, 0, 255), 255, 255);
  CRGB veriable2 = CHSV( map(analogRead(veriableColorPotPin2), 0, 1023, 0, 255), 255, 255);
  CRGB veriable3 = CHSV( map(analogRead(veriableColorPotPin3), 0, 1023, 0, 255), 255, 255);
  //CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     veriable0,  veriable0,  black,  black,
                     veriable1, veriable1, black,  black,
                     veriable2,  veriable2,  black,  black,
                     veriable3, veriable3, black,  black );
}

void SetupVeriablePalette2()
{
  CRGB veriable0 = CHSV( map(analogRead(veriableColorPotPin0), 0, 1023, 0, 240), 255, 255);  //240 turns to red again
  CRGB veriable1 = CHSV( map(analogRead(veriableColorPotPin1), 0, 1023, 0, 240), 255, 255);
  CRGB veriable2 = CHSV( map(analogRead(veriableColorPotPin2), 0, 1023, 0, 240), 255, 255);
  CRGB veriable3 = CHSV( map(analogRead(veriableColorPotPin3), 0, 1023, 0, 240), 255, 255);
  //CRGB green  = CHSV( HUE_GREEN, 255, 255);
  CRGB black  = CRGB::Black;

  currentPalette = CRGBPalette16(
                     veriable0,  black,  veriable1,  black,
                     veriable2, black, veriable3,  black,
                     veriable0,  black,  veriable1,  black,
                     veriable2, black, veriable3,  black );
}


void lightning()
{
#define color White;
  unsigned int dimmer = 1;
  for (int flashCounter = 0; flashCounter < random8(3, FLASHES); flashCounter++)
  {
    if (flashCounter == 0) dimmer = 5;    // the brightness of the leader is scaled down by a factor of 5
    else dimmer = random8(1, 3);          // return strokes are brighter than the leader

    FastLED.showColor(CHSV(255, 0, 255 / dimmer));
    FastLED.delay(random8(4, 10));                // each flash only lasts 4-10 milliseconds
    FastLED.showColor(CHSV(255, 0, 0));

    if (flashCounter == 0) delay (150);   // longer delay until next flash after the leader
    FastLED.delay(50 + random8(100));             // shorter delay between strokes
  }
  FastLED.delay(random8(FREQUENCY) * 100);        // delay between strikes
}


void fire()
{

  // Add entropy to random number generator; we use a lot of it.
  random16_add_entropy( random());

  Fire2012(); // run simulation frame

  FastLED.show(); // display this frame
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}




void Fire2012()
{
  // Array of temperature readings at each simulation cell
  static byte heat[NUM_LEDS];

  // Step 1.  Cool down every cell a little
  for ( int i = 0; i < NUM_LEDS; i++) {
    heat[i] = qsub8( heat[i],  random8(0, ((COOLING * 10) / NUM_LEDS) + 2));
  }

  // Step 2.  Heat from each cell drifts 'up' and diffuses a little
  for ( int k = NUM_LEDS - 3; k > 0; k--) {
    heat[k] = (heat[k - 1] + heat[k - 2] + heat[k - 2] ) / 3;
  }

  // Step 3.  Randomly ignite new 'sparks' of heat near the bottom
  if ( random8() < SPARKING ) {
    int y = random8(7);
    heat[y] = qadd8( heat[y], random8(160, 255) );
  }

  // Step 4.  Map from heat cells to LED colors
  for ( int j = 0; j < NUM_LEDS; j++) {
    leds[j] = HeatColor( heat[j]);
  }

}



void rainbow2() {
  static uint8_t starthue = 0;
  fill_rainbow( leds, NUM_LEDS, --starthue, 20);
  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
  //delay(100);
}


