# type: ignore
# Adafruit nOOds analog brightness control using AW9523 LED driver breakout.
# Uses 3 nOOds, anode (+) to VIN row, cathode (-) to pins labeled 13-15.

import math
import random
import time

import adafruit_aw9523
import board

FPS = 120
GAMMA = 2.6  # For perceptually-linear brightness
MAX_BRIGHTNESS = 245
PATTERN_LENGTH = 24
PINS = (6, 7, 12, 13, 14, 15)  # List of pins, one per nOOd

# Instantiate AW9523 on STEMMA I2C bus. This was tested on QT Py RP2040.
# Other boards might require board.I2C() instead of board.STEMMA_I2C().
aw = adafruit_aw9523.AW9523(board.STEMMA_I2C())
for pin in PINS:
    aw.get_pin(pin).switch_to_output(value=True)  # Activate pin, initialize OFF
    aw.LED_modes |= 1 << pin  # Enable constant-current on pin

pin_brightness = {pin: 0 for pin in PINS}

last_pattern_time = time.monotonic()


def phased_out_pattern():
    for i, pin in enumerate(PINS):
        phase_offset = i * (360 / len(PINS))
        sine_wave_pulse(pin, phase_offset)


def in_sync_pattern():
    for i, pin in enumerate(PINS):
        sine_wave_pulse(pin, 0)


def sine_wave_pulse(pin, phase_offset):
    global pin_brightness
    # phase = (time.monotonic() - 2 * i / len(PINS)) * math.pi
    phase = (time.monotonic() * math.pi) - math.radians(phase_offset)

    # Calc sine wave, phase offset for each pin, with gamma correction.
    normalized_sine = (math.sin(phase) + 1.0) / 2.0
    gc_sine = normalized_sine**GAMMA
    brightness = int((gc_sine * MAX_BRIGHTNESS) + 0.5)
    aw.set_constant_current(pin, brightness)


def make_sequence_pattern():
    global pin_brightness
    last_time = time.monotonic()
    last_pin = random.choice(PINS)

    def sequence_pattern():
        nonlocal last_time
        nonlocal last_pin
        current_time = time.monotonic()
        for pin in PINS:
            old_brightness = pin_brightness[pin]
            new_brightness = old_brightness * 0.99  # darken by 1%
            gc_new_brightness = int(
                (((new_brightness / MAX_BRIGHTNESS) ** GAMMA) * MAX_BRIGHTNESS) + 0.5
            )

            aw.set_constant_current(pin, gc_new_brightness)
            pin_brightness[pin] = new_brightness

        if last_time + 0.5 <= current_time:
            random_pin = random.choice(PINS)
            while random_pin == last_pin:
                random_pin = random.choice(PINS)

            brightness_value = MAX_BRIGHTNESS
            gc_brightness_value = int(
                (((brightness_value / MAX_BRIGHTNESS) ** 2.6) * MAX_BRIGHTNESS) + 0.5
            )
            aw.set_constant_current(random_pin, gc_brightness_value)
            pin_brightness[random_pin] = gc_brightness_value

            last_time = current_time
            last_pin = random_pin

    return sequence_pattern


patterns = [phased_out_pattern, in_sync_pattern, make_sequence_pattern()]
current_pattern = random.choice(patterns)


def loop():
    global current_pattern
    global last_pattern_time
    global patterns
    global pin_brightness

    current_time = time.monotonic()
    current_pattern()

    if len(patterns) > 1 and last_pattern_time + PATTERN_LENGTH <= current_time:
        new_pattern = random.choice(patterns)
        while new_pattern == current_pattern:
            new_pattern = random.choice(patterns)
        current_pattern = new_pattern
        last_pattern_time = current_time

    time.sleep(1 / FPS)


while True:
    loop()
