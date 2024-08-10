#include <FastLED.h>

#define DATA_PIN 0

#define BASE_HUE 0
#define BASE_VALUE 255
#define BRIGHTNESS 64
#define NUM_COLORS 8
#define NUM_LEDS 19
#define NUM_RINGS 3

// ========
// Typedefs
// ========

// Typedef to make function pointer array easier to type.
typedef void (*PatternArray[])();

// ========================
// Time Syncing Definitions
// ========================

// Frames Per Second
#define FPS 120

// Amount of time to show each pattern
#define PATTERN_SECS 12

// ================
// Global Variables
// ================

CRGB leds[NUM_LEDS];
uint8_t currentPattern; // Index of currently selected pattern

const CHSVPalette16 PENDANT_COLORS(
    // Red
    CHSV(0, 255, 255),
    CHSV(0, 156, 255),

    // Orange/Yellow
    CHSV(24, 255, 255),
    CHSV(24, 156, 255),

    // Yellow
    CHSV(35, 255, 255),
    CHSV(35, 156, 255),

    // Yellow-Green
    CHSV(50, 255, 246),
    CHSV(50, 155, 248),

    // Cyan
    CHSV(128, 255, 153),
    CHSV(128, 155, 178),

    // Light Blue
    CHSV(146, 238, 164),
    CHSV(146, 146, 187),

    // Blue/Purple
    CHSV(180, 226, 175),
    CHSV(180, 137, 195),

    // Violet
    CHSV(231, 255, 205),
    CHSV(231, 155, 217));

void colorFlow()
{
    // TODO: Adjust this pattern for the new format.
    uint16_t dot, index, colorIndex;
    for (index = 0; index < 256; ++index)
    {
        colorIndex = index;
        for (dot = 0; dot < NUM_LEDS; ++dot)
        {
            leds[dot] = ColorFromPalette(
                PENDANT_COLORS, colorIndex, BASE_VALUE, LINEARBLEND);
            colorIndex += 2;
        }
        FastLED.show();
        delay(50);
    }
}

void lightFirstFourLEDS(uint16_t color, uint8_t offset)
{
    // TODO: Adjust this pattern for the new format.
    uint8_t dot;
    uint16_t colorIndex;
    for (dot = 0; dot < NUM_LEDS; ++dot)
    {
        if (((dot + offset) & 7) < 4)
        {
            colorIndex = ((dot & 1) == 0) ? color : color + 16;
            leds[dot] = ColorFromPalette(
                PENDANT_COLORS, colorIndex, BASE_VALUE, LINEARBLEND);
        }
        else
        {
            leds[dot] = CRGB::Black;
        }
    }
    FastLED.show();
    delay(75);
}

void forwardAndBackwards()
{
    // TODO: Adjust this pattern for the new format.
    uint8_t dot;
    int8_t offset;
    uint16_t color;
    for (color = 0; color < 256; color += 32)
    {
        for (offset = 0; offset < NUM_LEDS; ++offset)
        {
            lightFirstFourLEDS(color, offset);
        }
    }
    for (color = 0; color < 256; color += 32)
    {
        for (offset = NUM_LEDS - 1; offset >= 0; --offset)
        {
            lightFirstFourLEDS(color, offset);
        }
    }
}

void twelveToSix()
{
    // TODO: Adjust this pattern for the new format.
    uint8_t dot;
    uint16_t color;
    int8_t offset;
    for (color = 0; color < 256; color += 4)
    {
        for (offset = 0; offset < 8; ++offset)
        {
            for (dot = 0; dot < (NUM_LEDS / 2); ++dot)
            {
                if (((dot + offset) & 7) < 4)
                {
                    leds[dot] = ColorFromPalette(
                        PENDANT_COLORS, color, BASE_VALUE, LINEARBLEND);
                    leds[NUM_LEDS - dot - 1] = ColorFromPalette(
                        PENDANT_COLORS, color, BASE_VALUE, LINEARBLEND);
                }
                else
                {
                    leds[dot] = CRGB::Black;
                    leds[NUM_LEDS - dot - 1] = CRGB::Black;
                }
            }
            FastLED.show();
            delay(75);
        }
    }
}

// TODO: Add color to the loop
// TODO: Code in per-frame basis
void outward()
{
    static uint64_t lastTime = 0;
    static const uint64_t animTimeMillis = 500;

    uint64_t currentTime = millis();

    if (lastTime + animTimeMillis <= currentTime)
    {
        lastTime = currentTime;
    }
    uint8_t ringIndex;
    uint8_t dot;
    uint8_t boundaries[][2] = {
        {12, 12},
        {13, 18},
        {0, 11},
    };

    for (ringIndex = 0; ringIndex < NUM_RINGS; ++ringIndex)
    {
        uint8_t left = boundaries[ringIndex][0];
        uint8_t right = boundaries[ringIndex][1];
        for (dot = 0; dot < NUM_LEDS; ++dot)
        {
            if (dot >= left && dot <= right)
            {
                leds[dot] = CRGB::Green;
            }
            else
            {
                leds[dot] = CRGB::Black;
            }
        }
        delay(200);
        FastLED.show();
    }
}

// =======================
// Patterns Initialization
// =======================

/*
 * Global list of pattern functions.
 *
 * Each should be the name of a function that has return type void and takes no
 * parameters. Some patterns are repeated in order to increase their chances of
 * showing up when the next pattern is randomly chosen.
 */
PatternArray patterns = {
    // colorFlow,
    outward,
    // forwardAndBackwards,
    // twelveToSix,
};

// Length of patterns[]
const uint16_t patternsLength = sizeof(patterns) / sizeof(patterns[0]);

/*
 * Randomly changes the index of the patterns array in order to switch to the
 * next pattern. This should be called in an EVERY_N_SECONDS macro provided by
 * FastLED in order to dynamically change patterns.
 */
void nextPattern()
{
    uint8_t newPattern = random8(patternsLength);
    if (patternsLength <= 1)
    {
        // If there's only one pattern, just leave the current one on.
        return;
    }
    while (newPattern == currentPattern)
    {
        newPattern = random8(patternsLength);
    }
    currentPattern = newPattern;
}

void setup()
{
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
}

// TODO: Do a loop where the biggest ring moves clockwise, middle ring counter
// clockwise, center just changes color.

// TODO: Add a pattern of spiraling inside and out.

void loop()
{
    patterns[currentPattern](); // Setup one frame of a pattern
    FastLED.show();             // Show the LED's
    FastLED.delay(1000 / FPS);  // Add a global delay at the frame rate.

    // Switch patterns every PATTERN_SECS
    EVERY_N_SECONDS(PATTERN_SECS) { nextPattern(); }
}
