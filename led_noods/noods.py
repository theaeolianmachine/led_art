# type: ignore
# Adafruit nOOds analog brightness control using AW9523 LED driver breakout.
# Uses 3 nOOds, anode (+) to VIN row, cathode (-) to pins labeled 13-15.

import math
import random
import time

import adafruit_aw9523
import board

GAMMA = 2.6  # For perceptually-linear brightness
PINS = (13, 14, 15)  # List of pins, one per nOOd
PATTERN_LENGTH = 24  # 5 Seconds

# Instantiate AW9523 on STEMMA I2C bus. This was tested on QT Py RP2040.
# Other boards might require board.I2C() instead of board.STEMMA_I2C().
aw = adafruit_aw9523.AW9523(board.STEMMA_I2C())
for pin in PINS:
    aw.get_pin(pin).switch_to_output(value=True)  # Activate pin, initialize OFF
    aw.LED_modes |= 1 << pin  # Enable constant-current on pin

last_time = time.monotonic()


def phased_out_pattern():
    for i, pin in enumerate(PINS):
        phase_offset = i * (360 / len(PINS))
        sine_wave_pulse(pin, phase_offset)


def in_sync_pattern():
    for i, pin in enumerate(PINS):
        sine_wave_pulse(pin, 0)


def sine_wave_pulse(pin, phase_offset):
    # phase = (time.monotonic() - 2 * i / len(PINS)) * math.pi
    phase = (time.monotonic() * math.pi) - math.radians(phase_offset)

    # Calc sine wave, phase offset for each pin, with gamma correction.
    brightness = int((math.sin(phase) + 1.0) * 0.5**GAMMA * 255 + 0.5)
    aw.set_constant_current(pin, brightness)


patterns = [phased_out_pattern, in_sync_pattern]
current_pattern = random.choice(patterns)


def loop():
    global current_pattern
    global last_time
    global patterns

    current_time = time.monotonic()

    if last_time + PATTERN_LENGTH <= current_time:
        new_pattern = random.choice(patterns)
        while new_pattern == current_pattern:
            new_pattern = random.choice(patterns)
        current_pattern = new_pattern
        last_time = current_time

    current_pattern()


while True:
    loop()
