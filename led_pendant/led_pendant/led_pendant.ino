#include <FastLED.h>

#define DATA_PIN 0

#define BASE_HUE 0
#define BASE_VALUE 255
#define BRIGHTNESS 64
#define NUM_COLORS 8
#define NUM_RINGS 3
#define RANDOM_ANALOG_PIN 1

// 12 LED Ring
#define LED_RING_SIZE 12
#define NUM_LEDS 19
#define INNER_RING 12
#define MIDDLE_RING 13
#define OUTER_RING 0

// 16 LED Ring
// #define LED_RING_SIZE 16
// #define NUM_LEDS 23
// #define INNER_RING 16
// #define MIDDLE_RING 17
// #define OUTER_RING 0

// Amount of time between each hue in rainbow
#define RAINBOW_MILLIS 20

const uint8_t ringIndexes[] = {OUTER_RING, MIDDLE_RING, INNER_RING};
const uint8_t ringSizes[] = {LED_RING_SIZE, NUM_LEDS - MIDDLE_RING, 1};
uint8_t ringOrder[NUM_LEDS];

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
#define PATTERN_SECS 24

// Amount of time to show each palette
#define PALETTE_SECS ((PATTERN_SECS) / 4)

// ================
// Global Variables
// ================

CRGB leds[NUM_LEDS];
uint8_t currentPattern;    // Index of currently selected pattern
uint8_t currentHSVPalette; // Index of currently selected HSV Palette
uint8_t currentRGBPalette; // Index of currently selected RGB Palette
uint8_t rainbowHue = 0;    // Global value for cycling through hues

// ============
// HSV Palettes
// ============

/*
 * Original Pendant Colors, when only one palette would fit in memory
 * circa 2015.
 */

const CHSVPalette16 OriginalPendantColors_p(
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

/*
 * Chez Cargot Colors
 *
 * A variety of dynamic colors that move across the color hue wheel in 1/6ths
 * with a few colors wrapping around, having a mixed sorting order that
 * encourages similar colors to be spread out.
 */
const CHSVPalette16 ChezCargotColorsPalette_p(
    CHSV(0, 223, 255),
    CHSV(127, 223, 255),
    CHSV(42, 223, 255),
    CHSV(170, 223, 255),

    CHSV(234, 223, 255),
    CHSV(191, 223, 255),
    CHSV(64, 223, 255),
    CHSV(191, 223, 255),

    CHSV(106, 223, 255),
    CHSV(234, 223, 255),
    CHSV(42, 223, 255),
    CHSV(255, 223, 255),

    CHSV(128, 223, 255),
    CHSV(255, 223, 255),
    CHSV(170, 223, 255),
    CHSV(42, 223, 255));

/*
 * Saturated Sunset Palette
 *
 * Features the same colors as the sunset palette, but with higher saturation.
 */
const CHSVPalette16 SaturatedSunsetPalette_p(
    CHSV(6, 223, 223),
    CHSV(217, 223, 141),
    CHSV(8, 223, 247),
    CHSV(163, 223, 71),
    CHSV(251, 223, 231),
    CHSV(163, 223, 100),
    CHSV(233, 223, 197),
    CHSV(169, 223, 121),
    CHSV(220, 223, 154),
    CHSV(207, 223, 112),
    CHSV(203, 223, 96),
    CHSV(253, 223, 237),
    CHSV(163, 223, 89),
    CHSV(242, 223, 216),
    CHSV(169, 223, 121),
    CHSV(233, 223, 218));

/*
 * Blue, Purple, and Red Palette
 *
 * Features a variety of blue and purple colors, with notes of de-saturated
 * reds. Was initially inspired by a photo of a flower, and was worked
 * extensively from there.
 */
const CHSVPalette16 BPRPalette_p(
    CHSV(142, 236, 254),
    CHSV(146, 236, 254),
    CHSV(151, 236, 254),
    CHSV(154, 236, 254),
    CHSV(159, 236, 254),
    CHSV(166, 236, 254),
    CHSV(173, 236, 254),
    CHSV(184, 236, 254),
    CHSV(193, 236, 254),
    CHSV(204, 170, 242),
    CHSV(210, 170, 242),
    CHSV(210, 156, 242),
    CHSV(234, 156, 242),
    CHSV(239, 163, 241),
    CHSV(246, 188, 248),
    CHSV(251, 188, 248));

/*
 * Grass Palette
 *
 * Features a variety of colors found on grass, including many pigments of
 * green with hints of yellow.
 */
const CHSVPalette16 GrassPalette_p(
    CHSV(112, 236, 109),
    CHSV(112, 221, 109),
    CHSV(112, 178, 70),
    CHSV(62, 151, 62),
    CHSV(50, 255, 70),
    CHSV(80, 222, 93),
    CHSV(90, 252, 132),
    CHSV(70, 252, 161),
    CHSV(65, 210, 180),
    CHSV(80, 190, 208),
    CHSV(66, 190, 208),
    CHSV(66, 211, 232),
    CHSV(66, 172, 249),
    CHSV(56, 172, 249),
    CHSV(46, 187, 255),
    CHSV(46, 187, 255));

/*
 * Candy Land Palette
 *
 * A variety of bright and saturated colors that were very bright and candy
 * like - specifically targeting high saturation to look good with the LED's.
 */
const CHSVPalette16 FiveHSVPalette_p(
    CHSV(43, 255, 255),
    CHSV(85, 255, 255),
    CHSV(124, 255, 255),
    CHSV(158, 255, 255),
    CHSV(117, 204, 255),
    CHSV(190, 204, 255),
    CHSV(43, 255, 255),
    CHSV(85, 255, 255),
    CHSV(124, 255, 255),
    CHSV(158, 255, 255),
    CHSV(117, 204, 255),
    CHSV(190, 204, 255),
    CHSV(43, 255, 255),
    CHSV(85, 255, 255),
    CHSV(124, 255, 255),
    CHSV(158, 255, 255));

// ============
// RGB Palettes
// ============

/*
 * Lava Colors (No Blacks) Palette
 *
 * A modification of FastLED's LavaColors palette, except replacing blacks with
 * other shades of red and orange.
 */
const CRGBPalette16 LavaNoBlackColorsPalette_p(
    CRGB::Crimson,
    CRGB::Maroon,
    CRGB::Red,
    CRGB::Maroon,

    CRGB::DarkRed,
    CRGB::Maroon,
    CRGB::DarkRed,
    CRGB::Orange,

    CRGB::DarkRed,
    CRGB::DarkRed,
    CRGB::Red,
    CRGB::Orange,

    CRGB::White,
    CRGB::Orange,
    CRGB::Red,
    CRGB::DarkRed);

// The collection of available HSV Palettes
CHSVPalette16 hsvPalettes[] = {
    OriginalPendantColors_p, ChezCargotColorsPalette_p, SaturatedSunsetPalette_p,
    BPRPalette_p, GrassPalette_p, FiveHSVPalette_p};

// The collection of available RGB Palettes
CRGBPalette16 rgbPalettes[] = {
    OriginalPendantColors_p, ChezCargotColorsPalette_p, LavaNoBlackColorsPalette_p, PartyColors_p,
    OceanColors_p, SaturatedSunsetPalette_p,
    BPRPalette_p, GrassPalette_p, FiveHSVPalette_p};

// Length of hsvPalettes[]
const uint16_t hsvPalettesLength = (sizeof(hsvPalettes) / sizeof(hsvPalettes[0]));

// Length of rgbPalettes[]
const uint16_t rgbPalettesLength = (sizeof(rgbPalettes) / sizeof(rgbPalettes[0]));

/*
 * Returns a random amount of entropy using an unused analogPin's input.
 *
 * Takes the read value of an unused analog pin, and scales it to a 16 byte
 * range. This allows the order of animations and palettes to be more random
 * upon first turning on the lights.
 */
uint16_t generate_entropy()
{
    return (analogRead(RANDOM_ANALOG_PIN) * 65535) / 1023;
}

/*
 * Returns a hue index that corresponds with the given LED index.
 *
 * This function smoothly interpolates over the range of [0, 255].
 *
 * Pre: 0 <= led < NUM_LEDS
 * Returns: 0 <= gradientHue <= 255
 */
uint8_t getGradientHue(uint16_t led)
{
    return (led * 255) / (NUM_LEDS - 1);
}

/*
 * Returns a hue index that corresponds to the group index of the given LED.
 *
 * There are NUM_GROUPS groups, and each group number corresponds to a value
 * over [0, 255]; this function returns the discrete value that corresponds
 * with a given LED's group. For example, if LED_GROUP_SIZE == 15, led
 * indexes [0, 14] would all return the same value, and the next hue starts at
 * led index 15.
 *
 * Pre: 0 <= led < NUM_LEDS
 * Returns: 0 <= groupHue <= 255
 */
uint8_t getGroupHue(uint16_t led)
{
    uint16_t modLedIndex = led;
    if (led >= MIDDLE_RING) // Assumes MIDDLE_RING > INNER_RING > OUTER_RING
    {
        modLedIndex = led - 1;
    }
    else if (led == INNER_RING)
    {
        modLedIndex = NUM_LEDS - 1;
    }
    return ((modLedIndex / 2) * 255) / (NUM_LEDS / 2);
}

void colorFlowPattern()
{
    static uint64_t lastTime = 0;
    static const uint64_t animTimeMillis = 50;

    uint64_t currentTime = millis();
    uint16_t dot, colorIndex;

    if (lastTime + animTimeMillis <= currentTime)
    {
        colorIndex = rainbowHue;
        for (dot = 0; dot < NUM_LEDS; ++dot)
        {
            leds[dot] = ColorFromPalette(
                rgbPalettes[currentRGBPalette], colorIndex, BASE_VALUE, LINEARBLEND);
            colorIndex += 5;
        }

        lastTime = currentTime;
    }
}

void lightGroupByOffset(uint8_t start, uint8_t end, uint8_t offset, uint8_t groupSize, uint8_t color)
{
    uint8_t dot;
    uint16_t colorIndex;
    for (dot = start; dot < end; ++dot)
    {
        if ((((dot - start) + offset) % groupSize) < (groupSize / 2))
        {
            colorIndex = ((dot & 1) == 0) ? color : color + 16;
            leds[dot] = ColorFromPalette(
                rgbPalettes[currentRGBPalette], colorIndex, BASE_VALUE, LINEARBLEND);
        }
        else
        {
            leds[dot] = CRGB::Black;
        }
    }
}

void forwardsAndBackwardsPattern()
{
    static uint8_t offset = 0;
    static uint64_t lastTime = 0;
    static const uint64_t animTimeMillis = 75;
    static uint8_t animCount = 0;
    static bool forwards = true;

    uint64_t currentTime = millis();

    if (lastTime + animTimeMillis <= currentTime)
    {
        uint8_t start = ringIndexes[0];
        uint8_t end = start + ringSizes[0];

        lightGroupByOffset(start, end, offset, (ringSizes[0] * 2) / 3, rainbowHue);
        ringPulse(1, NUM_RINGS, false);

        offset = forwards ? offset + 1 : offset - 1;

        if (offset % ringSizes[0] == 0)
        {
            animCount++;
        }
        if (animCount == 8)
        {
            forwards = !forwards;
            animCount = 0;
        }
        lastTime = currentTime;
    }
}

void twelveToSixPattern()
{
    static uint64_t lastTime = 0;
    static const uint64_t animTimeMillis = 75;
    static uint8_t offset = 0;

    uint16_t color;
    uint64_t currentTime = millis();

    uint8_t curHue = rainbowHue;

    if (lastTime + animTimeMillis <= currentTime)
    {
        // Outer Ring
        twelveToSixRing(ringIndexes[0], ringIndexes[0] + ringSizes[0], offset, (ringSizes[0] * 2) / 3, curHue);

        // Inner Rings
        ringPulse(1, NUM_RINGS, false);

        offset++;
        lastTime = currentTime;
    }
}

void twelveToSixRing(uint8_t start, uint8_t end, uint8_t offset, uint8_t groupSize, uint8_t color)
{
    for (uint8_t dot = start; dot < ((start + end) / 2); ++dot)
    {
        uint8_t mirrorDot = end - (dot - start) - 1;
        if ((((dot - start) + offset) % groupSize) < (groupSize / 2))
        {
            leds[dot] = ColorFromPalette(
                rgbPalettes[currentRGBPalette], color, BASE_VALUE, LINEARBLEND);

            leds[mirrorDot] = ColorFromPalette(
                rgbPalettes[currentRGBPalette], color, BASE_VALUE, LINEARBLEND);
        }
        else
        {
            leds[dot] = CRGB::Black;
            leds[mirrorDot] = CRGB::Black;
        }
    }
}

void ringPulse(uint8_t startRing, uint8_t endRing, bool groupHues)
{
    uint8_t currentHue = rainbowHue;
    uint8_t phaseOffset;
    for (uint8_t ringNum = startRing; ringNum < endRing; ++ringNum)
    {
        uint8_t startLed = ringIndexes[ringNum];
        uint8_t endLed = startLed + ringSizes[ringNum];
        for (uint8_t i = startLed; i < endLed; ++i)
        {
            uint8_t hue;
            phaseOffset = ringNum * 64;
            if (groupHues)
            {
                // hue = getGroupHue(i) + (ringNum * 16);
                hue = getGroupHue(i);
            }
            else
            {
                hue = currentHue + (ringNum * 16);
            }
            leds[i] = ColorFromPalette(
                rgbPalettes[currentRGBPalette],
                hue,
                beatsin8(60, 0, 255, 0, phaseOffset));
        }
    }
}

void ringsPattern()
{
    ringPulse(0, NUM_RINGS, false);
}

void ringsGroupPattern()
{
    ringPulse(0, NUM_RINGS, true);
}

void flowerPattern()
{
    static uint64_t lastTime = 0;
    static const uint64_t animTimeMillis = 180;
    static uint8_t curRing = 0;

    uint64_t currentTime = millis();

    fadeToBlackBy(leds, NUM_LEDS, 15);
    if (lastTime + animTimeMillis <= currentTime)
    {
        uint8_t dot;
        uint8_t oppRing = NUM_RINGS - curRing - 1;

        uint8_t start = ringIndexes[oppRing];
        uint8_t end = start + ringSizes[oppRing];
        for (dot = 0; dot < NUM_LEDS; ++dot)
        {
            if (dot >= start && dot < end)
            {
                leds[dot] = ColorFromPalette(
                    rgbPalettes[currentRGBPalette],
                    rainbowHue + (curRing * 32),
                    BASE_VALUE);
            }
        }
        curRing = (curRing + 1) % NUM_RINGS;
        lastTime = currentTime;
    }
}

/*
 * Pattern that goes the entire strip in 16 off beat sin waves.
 */
void beatSyncMultiplesPattern()
{
    fadeToBlackBy(leds, NUM_LEDS, 20);
    CRGBPalette16 palette = rgbPalettes[currentRGBPalette];
    for (uint16_t i = 0; i < 16; ++i)
    {
        uint16_t index = beatsin16(i * 2, 0, NUM_LEDS);
        leds[index] |= ColorFromPalette(palette, i * 16, BASE_VALUE);
    }
}

/*
 * Turns on LED's at random, creating a sparkling pattern where LED's slowly
 * fade to black.
 */
void randomSparklesPattern()
{
    static uint64_t lastTime = 0;
    static uint8_t lastPos = NUM_LEDS; // Always higher than what is possible with random8(NUM_LEDS)
    static const uint64_t animTimeMillis = 20;

    uint64_t currentTime = millis();
    uint8_t pos = random8(NUM_LEDS);
    while (pos == lastPos)
    {
        pos = random8(NUM_LEDS);
    }
    uint8_t hue;

    fadeToBlackBy(leds, NUM_LEDS, 10);
    if (lastTime + animTimeMillis <= currentTime)
    {
        hue = rainbowHue;
        leds[pos] += CHSV(hue + random8(64), 223, BASE_VALUE);
        lastTime = currentTime;
        lastPos = pos;
    }
}

void spiralPattern()
{
    static uint64_t lastTime = 0;
    static const uint64_t animTimeMillis = 50;
    static uint8_t first = 0;
    static uint8_t second = ringSizes[0];

    uint64_t currentTime = millis();
    fadeToBlackBy(leds, NUM_LEDS, 30);

    if (lastTime + animTimeMillis <= currentTime)
    {
        leds[ringOrder[first]] = ColorFromPalette(
            rgbPalettes[currentRGBPalette],
            rainbowHue,
            BASE_VALUE);

        leds[ringOrder[second]] = ColorFromPalette(
            rgbPalettes[currentRGBPalette],
            rainbowHue,
            BASE_VALUE);

        first = (first + 1) % NUM_LEDS;
        second = (second + 1) % NUM_LEDS;
        lastTime = currentTime;
    }
}

void firstLightsReference()
{
    for (uint8_t i = 0; i < NUM_LEDS; ++i)
    {
        leds[i] = CRGB::Black;
    }
    for (uint8_t i = 0; i < NUM_RINGS; ++i)
    {
        leds[ringIndexes[i]] = CRGB::Red;
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
    // firstLightsReference
    colorFlowPattern,
    flowerPattern,
    forwardsAndBackwardsPattern,
    ringsPattern,
    ringsGroupPattern,
    twelveToSixPattern,
    beatSyncMultiplesPattern,
    randomSparklesPattern,
    spiralPattern,
};

// Length of patterns[]
const uint16_t patternsLength = sizeof(patterns) / sizeof(patterns[0]);

// ========================
// State Changing Functions
// ========================

/*
 * Randomly changes the index on both the HSV and RGB palette arrays. This
 * should be called in a EVERY_N_SECONDS macro provided by FastLED in order
 * to dynamically change color palettes in patterns.
 */
void nextPalette()
{
    if (rgbPalettesLength > 1)
    {
        uint8_t newRGBPalette = random8(rgbPalettesLength);
        while (newRGBPalette == currentRGBPalette)
        {
            newRGBPalette = random8(rgbPalettesLength);
        }
        currentRGBPalette = newRGBPalette;
    }

    if (hsvPalettesLength > 1)
    {
        uint8_t newHSVPalette = random8(hsvPalettesLength);
        while (newHSVPalette == currentHSVPalette)
        {
            newHSVPalette = random8(hsvPalettesLength);
        }
        currentHSVPalette = newHSVPalette;
    }
}
/*
 * Randomly changes the index of the patterns array in order to switch to the
 * next pattern. This should be called in an EVERY_N_SECONDS macro provided by
 * FastLED in order to dynamically change patterns.
 */
void nextPattern()
{
    if (patternsLength > 1)
    {
        uint8_t newPattern = random8(patternsLength);
        while (newPattern == currentPattern)
        {
            newPattern = random8(patternsLength);
        }
        currentPattern = newPattern;
    }
}

void setupRingOrder()
{
    uint8_t orderIndex = 0;
    for (uint8_t ringNum = 0; ringNum < NUM_RINGS; ++ringNum)
    {
        uint8_t start = ringIndexes[ringNum];
        uint8_t end = start + ringSizes[ringNum];
        for (uint8_t index = start; index < end; ++index)
        {
            ringOrder[orderIndex++] = index;
        }
    }
}

void setup()
{
    Serial.begin(9600);

    // Add some entropy to initial random FastLED seed
    random16_add_entropy(generate_entropy());

    // Initialize starting patterns and palettes
    currentPattern = random8(patternsLength);
    currentHSVPalette = random8(hsvPalettesLength);
    currentRGBPalette = random8(rgbPalettesLength);

    setupRingOrder();

    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop()
{
    patterns[currentPattern](); // Setup one frame of a pattern
    FastLED.show();             // Show the LED's
    FastLED.delay(1000 / FPS);  // Add a global delay at the frame rate.

    // Switch patterns every PATTERN_SECS
    EVERY_N_SECONDS(PATTERN_SECS) { nextPattern(); }

    // Switch palettes every PALETTE_SECS
    EVERY_N_SECONDS(PALETTE_SECS) { nextPalette(); }

    // Increment the "rainbow hue" every RAINBOW_MILLIS milli's.
    EVERY_N_MILLISECONDS(RAINBOW_MILLIS) { ++rainbowHue; }
}
