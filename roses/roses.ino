#include "FastLED.h"

// =======================
// Arduino Pin Definitions
// =======================


// Digital Clock Pin For APA102 (Dotstar) Timing
/* #define CLOCK_PIN 23  // SPI Clock Pin (Arduino Zero) */
#define CLOCK_PIN 13  // SPI Clock Pin (Teensy 3.2)

// Digital Data Pin For APA102 (Dotstar) Data
/* #define DATA_PIN 24  //SPI Data Pin (Arduino Zero) */
#define DATA_PIN 11  //SPI Data Pin (Teensy 3.2)

// Analog Pin used to add entropy to random seed.
// NOTE: Nothing should be plugged into this pin.
#define RANDOM_ANALOG_PIN 0

// ================
// APA102/Dotstar Definitions
// ================

// RGB Order for LED strips
#define RGB_ORDER BGR

// ===============
// HSV Definitions
// ===============

// Brightness/Value
#define DEFAULT_BRIGHTNESS 191
#define MIN_BRIGHTNESS 63
#define MAX_BRIGHTNESS 255

// Saturation
#define DEFAULT_SAT 223
#define MIN_SAT 191
#define MAX_SAT 255

// ==========================
// LED Strip Size Definitions
// ==========================

// This number represents the offset between the
// stem of the rose vs the petals. There are three heights
// to correspond to each of the three roses we originally made.
// Only one value should be set for the roses.

// Burrow Rose Short
// #define PETAL_OFFSET 44

// Burrow Rose Medium
// #define PETAL_OFFSET 45

// Burrow Rose Large / Tall
#define PETAL_OFFSET 59

// Number of LED's On Strip
#define NUM_LEDS (PETAL_OFFSET + 240)

// Number of rings within a single flower
#define NUM_RINGS ((NUM_LEDS - PETAL_OFFSET) / 60)

// ========================
// Time Syncing Definitions
// ========================

// Frames Per Second
#define FPS 120

// Amount of time to show each pattern
#define PATTERN_SECS 120
// #define PATTERN_SECS 600

// Amount of time to show each palette
#define PALETTE_SECS ((PATTERN_SECS) / 5)

// Amount of time between each hue in rainbow
#define RAINBOW_MILLIS 200

// ========
// Typedefs
// ========

// Typedef to make function pointer array easier to type.
typedef void (*PatternArray[])();

// Typedef for defining the animation type for a given pattern function.
// Generally used in a switch statement to add variety to existing animations.
typedef enum {
    RAINBOW_ANIM, GROUP_ANIM, HSV_PALETTE_ANIM
} AnimationType;


// ================
// Global Variables
// ================
CRGB leds[NUM_LEDS];

uint8_t currentPattern;     // Index of currently selected pattern
uint8_t currentHSVPalette;  // Index of currently selected HSV Palette
uint8_t rainbowHue = 0;     // Global value for cycling through hues


// ============
// HSV Palettes
// ============

/*
 * Chez Cargot Colors
 *
 * A variety of dynamic colors that move across the color hue wheel in 1/6ths
 * with a few colors wrapping around, having a mixed sorting order that
 * encourages similar colors to be spread out.
 */
const CHSVPalette16 ChezCargotColorsPalette_p(
    CHSV(0, DEFAULT_SAT, MAX_BRIGHTNESS),
    CHSV(127, DEFAULT_SAT, MAX_BRIGHTNESS),
    CHSV(42, DEFAULT_SAT, MAX_BRIGHTNESS),
    CHSV(170, DEFAULT_SAT, MAX_BRIGHTNESS),

    CHSV(234, DEFAULT_SAT, MAX_BRIGHTNESS),
    CHSV(191, DEFAULT_SAT, MAX_BRIGHTNESS),
    CHSV(64, DEFAULT_SAT, MAX_BRIGHTNESS),
    CHSV(191, DEFAULT_SAT, MAX_BRIGHTNESS),

    CHSV(106, DEFAULT_SAT, MAX_BRIGHTNESS),
    CHSV(234, DEFAULT_SAT, MAX_BRIGHTNESS),
    CHSV(42, DEFAULT_SAT, MAX_BRIGHTNESS),
    CHSV(255, DEFAULT_SAT, MAX_BRIGHTNESS),

    CHSV(128, DEFAULT_SAT, MAX_BRIGHTNESS),
    CHSV(255, DEFAULT_SAT, MAX_BRIGHTNESS),
    CHSV(170, DEFAULT_SAT, MAX_BRIGHTNESS),
    CHSV(42, DEFAULT_SAT, MAX_BRIGHTNESS)
);

// NOT USED
/*
 * Colors from a picture of a rose.
 * 
 * Reds, greens, and a variety of colors that make up the
 * petals, the stem, and the leaves of a rose.
 */
const CHSVPalette16 RoseColorsPalette_p(
    // Petals
    CHSV(251, 203, 190),
    CHSV(0, 152, 238),
    CHSV(252, 203, 180),
    CHSV(253, 182, 158),

    // Leaves
    CHSV(120, 33, 92),
    CHSV(113, 20, 113),
    CHSV(143, 71, 125),
    CHSV(119, 51, 80),

    // Stem
    CHSV(7, 55, 106),
    CHSV(7, 44, 103),
    CHSV(5, 63, 116),
    CHSV(8, 27, 46),

    // More Petals
    CHSV(251, 209, 197),
    CHSV(251, 198, 213),
    CHSV(253, 183, 171),
    CHSV(1, 110, 246)
);

const CHSVPalette16 BriWoodlandMicroscapePalette_p(
    CHSV(90, 116, 118),
    CHSV(90, 52, 194),
    CHSV(89, 89, 151),
    CHSV(90, 150, 88),

    CHSV(131, 115, 93),
    CHSV(130, 52, 152),
    CHSV(130, 88, 118),
    CHSV(130, 147, 69),

    CHSV(19, 115, 152),
    CHSV(20, 53, 251),
    CHSV(20, 89, 194),
    CHSV(19, 148, 113),

    CHSV(1, 115, 152),
    CHSV(2, 53, 251),
    CHSV(1, 89, 194),
    CHSV(1, 148, 113)
);

const CHSVPalette16 BriTheBlueberriesTasteLikeRaspberriesPalette_p(
    CHSV(207, 231, 207),
    CHSV(207, 89, 97),
    CHSV(207, 171, 140),
    CHSV(207, 249, 251),
    CHSV(207, 248, 255),

    CHSV(245, 235, 244),
    CHSV(244, 91, 136),
    CHSV(245, 174, 197),
    CHSV(245, 191, 197),
    CHSV(245, 255, 254),
    CHSV(245, 253, 255),

    CHSV(181, 210, 211),
    CHSV(181, 82, 102),
    CHSV(181, 156, 147),
    CHSV(181, 227, 251),
    CHSV(181, 226, 255)
);

const CHSVPalette16 BriAcidRosePalette_p(
    CHSV(230, 255, 210),
    CHSV(230, 95, 221),
    CHSV(230, 115, 210),
    CHSV(230, 141, 210),
    CHSV(231, 179, 207),
    CHSV(231, 191, 181),
    CHSV(231, 255, 162),
    CHSV(231, 255, 121),

    CHSV(56, 255, 240),
    CHSV(56, 96, 244),
    CHSV(56, 110, 244),
    CHSV(56, 130, 244),
    CHSV(56, 161, 244),
    CHSV(56, 180, 239),
    CHSV(56, 255, 188),
    CHSV(56, 255, 141)
);

const CHSVPalette16 BriSummerRosePalette_p(
    CHSV(17, 242, 255),
    CHSV(17, 150, 255),
    CHSV(17, 195, 255),
    CHSV(17, 255, 241),
    CHSV(17, 255, 186),

    CHSV(28, 242, 255),
    CHSV(28, 150, 255),
    CHSV(28, 173, 255),
    CHSV(28, 195, 255),
    CHSV(28, 255, 241),
    CHSV(28, 255, 186),

    CHSV(245, 242, 240),
    CHSV(245, 149, 245),
    CHSV(245, 194, 242),
    CHSV(245, 255, 221),
    CHSV(245, 255, 171)
);

const CHSVPalette16 BriSherbertPalette_p(
    CHSV(238, 182, 221),
    CHSV(238, 251, 139),
    CHSV(238, 169, 182),
    CHSV(238, 73, 245),

    CHSV(222, 182, 186),
    CHSV(222, 252, 117),
    CHSV(221, 170, 153),
    CHSV(221, 74, 236),

    CHSV(248, 181, 241),
    CHSV(248, 251, 152),
    CHSV(248, 169, 199),
    CHSV(248, 74, 251),

    CHSV(5, 182, 255),
    CHSV(5, 251, 161),
    CHSV(5, 169, 211),
    CHSV(5, 73, 255)
);

const CHSVPalette16 Bri1983Palette_p(
    CHSV(51, 182, 245),
    CHSV(51, 251, 154),
    CHSV(51, 169, 202),
    CHSV(51, 73, 252),

    CHSV(74, 181, 216),
    CHSV(74, 251, 136),
    CHSV(75, 169, 179),
    CHSV(74, 74, 244),

    CHSV(249, 181, 243),
    CHSV(249, 251, 153),
    CHSV(248, 169, 201),
    CHSV(248, 74, 252),

    CHSV(217, 181, 177),
    CHSV(217, 252, 111),
    CHSV(217, 169, 146),
    CHSV(217, 74, 233)
);

// NOT USED
const CHSVPalette16 OceanColorsHSV_p(
    CHSV(170, 198, 112),
    CHSV(170, 255, 139),
    CHSV(170, 198, 112),
    CHSV(170, 255, 128),

    CHSV(170, 255, 139),
    CHSV(170, 255, 205),
    CHSV(103, 170, 139),
    CHSV(127, 255, 128),

    CHSV(128, 103, 160),
    CHSV(170, 255, 255),
    CHSV(127, 255, 139),
    CHSV(154, 147, 237),

    CHSV(113, 128, 255),
    CHSV(103, 170, 139),
    CHSV(127, 255, 255),
    CHSV(143, 117, 250)
);

/*
 * High Saturation Palette
 *
 * A variety of bright and saturated colors that were very bright and candy
 * like - specifically targeting high saturation to look good with the LED's.
 */
const CHSVPalette16 HighSatPalette_p(
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
    CHSV(158, 255, 255)
);

const CHSVPalette16 LavaColorsNoBlackHSVPalette_p(
    CHSV(246, 231, 220),
    CHSV(0, 255, 128),
    CHSV(0, 255, 255),
    CHSV(0, 255, 128),

    CHSV(0, 255, 139),
    CHSV(0, 255, 128),
    CHSV(0, 255, 139),
    CHSV(27, 255, 255),

    CHSV(0, 255, 139),
    CHSV(0, 255, 139),
    CHSV(0, 255, 255),
    CHSV(27, 255, 255),

    CHSV(0, 0, 255),
    CHSV(27, 255, 255),
    CHSV(0, 255, 255),
    CHSV(0, 255, 139)
);

// The collection of available HSV Palettes
CHSVPalette16 hsvPalettes[] = {
    ChezCargotColorsPalette_p,
    BriTheBlueberriesTasteLikeRaspberriesPalette_p,
    BriSummerRosePalette_p,
    HighSatPalette_p,
    LavaColorsNoBlackHSVPalette_p
};

// Length of hsvPalettes[]
const uint16_t hsvPalettesLength = (
    sizeof(hsvPalettes) / sizeof(hsvPalettes[0]));

// =================
// Utility Functions
// =================

/*
 * Returns a random amount of entropy using an unused analogPin's input.
 *
 * Takes the read value of an unused analog pin, and scales it to a 16 byte
 * range. This allows the order of animations and palettes to be more random
 * upon first turning on the lights.
 */
uint16_t generate_entropy() {
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
uint8_t getGradientHue(uint16_t led, uint16_t numLeds) {
    return (led * 255) / (numLeds - 1);
}

/*
 * Pattern that pulses out from the center in off-beat sine waves - similar to
 * flowerOrbPattern, but more subtle.
 */
void ringsPattern(AnimationType animType) {
    uint8_t phaseOffset;
    uint16_t ringSize = ((NUM_LEDS - PETAL_OFFSET) / NUM_RINGS);

    // Petal
    for (uint16_t ringNum = 0; ringNum < NUM_RINGS; ++ringNum) {
        uint16_t ringStart = (ringNum * ringSize) + PETAL_OFFSET;
        uint16_t ringEnd = ringStart + ringSize;
        phaseOffset = ringNum * (256 / NUM_RINGS);
        for (uint16_t i = ringStart; i < ringEnd; ++i) {
            uint16_t ringHue = ((i / ringSize) * 255) / (NUM_RINGS - 1);
            switch(animType) {
                case GROUP_ANIM:
                    leds[i] = ColorFromPalette(
                        (CRGBPalette16) hsvPalettes[currentHSVPalette],
                        ringHue + (ringNum * 16),
                        beatsin8(15, 0, MAX_BRIGHTNESS, 0, phaseOffset));
                    break;
                case HSV_PALETTE_ANIM:
                    leds[i] = ColorFromPalette(
                        (CRGBPalette16) hsvPalettes[currentHSVPalette],
                        rainbowHue + (ringNum * 16),
                        beatsin8(15, 0, MAX_BRIGHTNESS, 0, phaseOffset));
                    break;
                case RAINBOW_ANIM:
                default:
                    leds[i] = ColorFromPalette(
                        (CRGBPalette16) hsvPalettes[currentHSVPalette],
                        getGradientHue(i, NUM_LEDS - PETAL_OFFSET),
                        beatsin8(15, 0, MAX_BRIGHTNESS, 0, phaseOffset));
                    break;
            }
        }
    }
    stemPattern();
}

void ringsRainbowPattern() {
    ringsPattern(RAINBOW_ANIM);
}

void ringsGroupPattern() {
    ringsPattern(GROUP_ANIM);
}

void ringsHSVPattern() {
    ringsPattern(HSV_PALETTE_ANIM);
}

void beatSyncMultiplesPattern() {
    // Petal
    for (uint16_t i = PETAL_OFFSET; i < NUM_LEDS; ++i) {
        leds[i].nscale8(255 - 20);
    }
    CRGBPalette16 palette = (CRGBPalette16) hsvPalettes[currentHSVPalette];
    for(uint16_t i = 0; i < 16; ++i) {
        uint16_t index = beatsin16(i * 2, PETAL_OFFSET, NUM_LEDS);
        leds[index] |= ColorFromPalette(palette, i * 16, MAX_BRIGHTNESS);
    }

    stemPattern();
}

void convergePattern(AnimationType animType) {
    static const uint64_t animationTimeMillis = 64;
    static const uint16_t maxDist = 4;

    static uint64_t lastTime = 0;
    static uint16_t dist = 0;
    static boolean goingOut = true;

    uint64_t currentTime = millis();
    uint16_t start = maxDist - dist;
    uint16_t groupSize = 7;

    if (lastTime + animationTimeMillis <= currentTime) {
        for (uint16_t i = PETAL_OFFSET; i < NUM_LEDS; i += groupSize) {
            for (uint16_t j = i + start;
                    j <= i + 4 + dist && j < NUM_LEDS; ++j) {
                if (goingOut) {
                    switch (animType) {
                        case RAINBOW_ANIM:
                            leds[j] = ColorFromPalette(
                                hsvPalettes[currentHSVPalette],
                                getGradientHue(j, NUM_LEDS - PETAL_OFFSET) + random8(), DEFAULT_BRIGHTNESS);
                            break;
                        case HSV_PALETTE_ANIM:
                        default:
                            {
                                CHSV hsv = ColorFromPalette(
                                    hsvPalettes[currentHSVPalette],
                                    getGradientHue(j, NUM_LEDS - PETAL_OFFSET), DEFAULT_BRIGHTNESS);
                                hsv.sat = beatsin8(30, MIN_SAT, MAX_SAT);
                                hsv.hue += 48 * dist;
                                leds[j] = hsv;
                            }
                            break;
                    }
                } else {
                    leds[j].nscale8(200);
                }
            }
        }

        dist = (dist + 1) % (maxDist + 1);
        if (dist == 0) {
            goingOut = !goingOut;
        }
        lastTime = currentTime;
    }
    stemPattern();
}

void convergeRainbow() {
    convergePattern(RAINBOW_ANIM);
}

void convergeHSV() {
    convergePattern(HSV_PALETTE_ANIM);
}

void ringPulsePattern(AnimationType animType) {
    static const uint64_t animationTimeMillis = 468; // Roughly 128 BPM
    static uint64_t lastTime = 0;
    static uint8_t ringNum = NUM_RINGS - 1;

    uint64_t currentTime = millis();
    uint16_t ringSize = ((NUM_LEDS - PETAL_OFFSET) / NUM_RINGS);

    for (uint16_t i = PETAL_OFFSET; i < NUM_LEDS; ++i) {
        leds[i].nscale8(255 - 3);
    }

    if (lastTime + animationTimeMillis <= currentTime) {
        uint16_t ringStart = (ringNum * ringSize) + PETAL_OFFSET;
        uint16_t ringEnd = ringStart + ringSize;
        for (uint16_t i = ringStart; i < ringEnd; ++i) {
            uint16_t ringHue = ((i / ringSize) * 255) / (NUM_RINGS - 1);
            switch(animType) {
                case GROUP_ANIM:
                    leds[i] = ColorFromPalette(
                        hsvPalettes[currentHSVPalette],
                        ringHue,
                        MAX_BRIGHTNESS);
                    break;
                case RAINBOW_ANIM:
                    leds[i] = ColorFromPalette(
                        hsvPalettes[currentHSVPalette], rainbowHue,
                        MAX_BRIGHTNESS);
                    break;
                case HSV_PALETTE_ANIM:
                default:
                    leds[i] = ColorFromPalette(
                        hsvPalettes[currentHSVPalette], getGradientHue(i, NUM_LEDS - PETAL_OFFSET),
                        MAX_BRIGHTNESS);
                break;
            }
        }

        if (ringNum == 0) {
            ringNum = NUM_RINGS - 1;
        } else {
            --ringNum;
        }
        lastTime = currentTime;
    }
    stemPattern();
}

void ringPulsePatternRainbow() {
    ringPulsePattern(RAINBOW_ANIM);
}

void ringPulsePatternHSV() {
    ringPulsePattern(HSV_PALETTE_ANIM);
}

void ringPulsePatternGroup() {
    ringPulsePattern(GROUP_ANIM);
}

void pulsePattern() {
    uint8_t bpm = 15;
    uint8_t beat = beatsin8(bpm, 127, MAX_BRIGHTNESS);
    for (uint16_t i = PETAL_OFFSET; i < NUM_LEDS; ++i) {
        leds[i] = ColorFromPalette(hsvPalettes[currentHSVPalette], rainbowHue + (i * 3), beat);
    }
    stemPattern();
}

void beatSyncingBackAndForthPattern() {
    uint8_t bpm = 10;
    uint8_t beat = beatsin8(bpm, 64, MAX_BRIGHTNESS);
    uint8_t slowBeat = beatsin8(bpm / 10, 64, 255);

    CRGBPalette16 palette = ChezCargotColorsPalette_p;

    for(uint16_t i = PETAL_OFFSET; i < NUM_LEDS; ++i) {
        uint8_t brightness = beat - (rainbowHue + (i * 2));
        leds[i] = ColorFromPalette(
            palette, slowBeat - (rainbowHue + (i * 2)), brightness);
    }
    stemPattern();
}

void twinklePattern() {
    uint8_t bpm = 10;
    uint8_t beat = beatsin8(bpm, 0, 255);
    CHSVPalette16 palette = hsvPalettes[currentHSVPalette];
    for (uint16_t i = PETAL_OFFSET; i < NUM_LEDS; ++i) {
        CHSV hsv = ColorFromPalette(
            palette, getGradientHue(i, NUM_LEDS - PETAL_OFFSET), DEFAULT_BRIGHTNESS);
        hsv.sat = lerp8by8(MIN_SAT, MAX_SAT, beat);
        hsv.val = lerp8by8(127, MAX_BRIGHTNESS, beat);
        if (i == PETAL_OFFSET) {
            Serial.println(hsv.val);
        }
        leds[i] = hsv;
    }
    stemPattern();
}

void stemPattern() {
    // Stem
    uint8_t bpm = 15;
    uint8_t beat = beatsin8(bpm, 0, 255);
    CHSVPalette16 palette = hsvPalettes[currentHSVPalette];
    for (uint16_t i = 0; i < PETAL_OFFSET; ++i) {
        CHSV hsv = ColorFromPalette(
            palette, rainbowHue, 127);
        hsv.sat = lerp8by8(MIN_SAT, MAX_SAT, beat);
        if (i % 2 == 0) {
            hsv.val = lerp8by8(96, 16, beat);
        } else {
            hsv.val = lerp8by8(16, 96, beat);
        }
        leds[i] = hsv;
    }
}

void blackout() {
    for (uint16_t i = 0; i < NUM_LEDS; ++i) {
        leds[i] = CRGB::Black;
    }
}

PatternArray patterns = {
    // Chill Night
    // pulsePattern,
    // ringsHSVPattern,
    // ringsRainbowPattern,
    // ringsGroupPattern,
    // twinklePattern,

    // Beats Night
    ringPulsePatternGroup,
    ringPulsePatternRainbow,
    ringPulsePatternHSV,
    convergeRainbow,
    convergeHSV,
    beatSyncMultiplesPattern,
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
 *
 * NOTE: It always chooses a unique palette (no repetitions), so as a result
 * you have to have at least 2 palettes to not encounter an infinite loop.
 */
void nextPalette() {
    uint8_t newHSVPalette = random8(hsvPalettesLength);
    while (newHSVPalette == currentHSVPalette) {
        newHSVPalette = random8(hsvPalettesLength);
    }
    currentHSVPalette = newHSVPalette;
}

/*
 * Randomly changes the index of the patterns array in order to switch to the
 * next pattern. This should be called in an EVERY_N_SECONDS macro provided by
 * FastLED in order to dynamically change patterns.
 *
 * NOTE: It always chooses a unique pattern (no repetitions), so as a result
 * you have to have at least 2 patterns to not encounter an infinite loop.
 */
void nextPattern() {
    uint8_t newPattern = random8(patternsLength);
    while (newPattern == currentPattern) {
        newPattern = random8(patternsLength);
    }
    currentPattern = newPattern;
}

void setup() {
    pinMode(7, OUTPUT);
    digitalWrite(7, HIGH);

    // Add some entropy to initial random FastLED seed
    // random16_add_entropy(generate_entropy());

    // uint8_t randomDelay = random8(5) + 1;
    // delay(randomDelay * 1000);

    random16_set_seed(55555);

    // Initialize starting patterns and palettes
    currentPattern = random8(patternsLength);
    currentHSVPalette = random8(hsvPalettesLength);

    // FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB_ORDER, DATA_RATE_MHZ(8)>(
    FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, RGB_ORDER>(
        leds, NUM_LEDS).setCorrection(TypicalSMD5050);

    // Set the color temperature
    FastLED.setTemperature(CarbonArc);

    // Set the global brightness
    // FastLED.setBrightness(DEFAULT_BRIGHTNESS);
}

void loop() {
    patterns[currentPattern]();
    FastLED.show(); // Show the LED's
    FastLED.delay(1000 / FPS);  // Add a global delay at the frame rate.

    // Switch patterns every PATTERN_SECS
    EVERY_N_SECONDS(PATTERN_SECS) { nextPattern(); }

    // Switch palettes every PALETTE_SECS
    EVERY_N_SECONDS(PALETTE_SECS) { nextPalette(); }

    // Increment the "rainbow hue" every RAINBOW_MILLIS milli's.
    EVERY_N_MILLISECONDS(RAINBOW_MILLIS) { ++rainbowHue; }
}
