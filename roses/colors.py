from colorsys import rgb_to_hsv
from math import floor
import re

def to_hsv(r, g, b):
    norm_r = r / 255
    norm_g = g / 255
    norm_b  = b / 255

    h, s, v = rgb_to_hsv(norm_r, norm_g, norm_b)

    return (int(floor(h * 255)), int(floor(s * 255)), int(floor(v * 255)))


def print_hsv(r, g, b):
    print(f"CHSV{repr(to_hsv(r, g, b))},")


def rgb_regex_convert(lines):
    matches = [re.search(r"rgb\(([\s\d]+),([\s\d]+),([\s\d]+)\)", line) for line in lines]
    matches = [m.group(1, 2, 3) for m in matches if m is not None]
    matches = [[int(i) for i in g] for g in matches]

    for m in matches:
        print_hsv(*m)


def hex_convert(lines):
    rgbs = []
    nums = [int(l, base=16) for l in lines]
    for n in nums:
        b = n & 0x0000FF
        g = (n & 0x00FF00) >> 8
        r = (n & 0xFF0000) >> 16
        rgbs.append((r, g, b))

    for rgb in rgbs:
        print_hsv(*rgb)


def main():
    with open('colors.txt', 'r') as fobj:
        lines = [l.strip() for l in fobj.readlines()]

    # rgb_regex_convert(lines)
    hex_convert(lines)


if __name__ == '__main__':
    main()
