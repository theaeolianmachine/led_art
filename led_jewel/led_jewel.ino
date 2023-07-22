#include <FastLED.h>

#define BASE_HUE 0
#define BASE_VALUE 255
#define BRIGHTNESS 64
#define DATA_PIN 0
#define NUM_LEDS 7

CRGB leds[NUM_LEDS];


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
    CHSV(42, 223, 255)
);


void colorFlow() {
    uint16_t dot, index, colorIndex;
    for (index = 0; index < 256; ++index) {
        colorIndex = index;
        for (dot = 0; dot < NUM_LEDS; ++dot) {
            leds[dot] = ColorFromPalette(
                ChezCargotColorsPalette_p,
                colorIndex, BASE_VALUE, LINEARBLEND);
            colorIndex += 2;
        }
        FastLED.show();
        delay(50);
    }
}


void setup() {
    FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);
    FastLED.setBrightness(BRIGHTNESS);
}

void loop() {
    colorFlow();
}
