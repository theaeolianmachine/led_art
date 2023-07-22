import argparse
import colorsys


def rgb(i):
    r = (i & 0xFF0000) >> 16
    g = (i & 0xFF00) >> 8
    b = i & 0xFF
    return (r, g, b)


def int_to_rgb_str(i):
    r, g, b = rgb(i)
    return "    CRGB({}, {}, {}),".format(r, g, b)


def int_to_hsv_str(i):
    r, g, b = rgb(i)
    nr = r / 255.0
    ng = g / 255.0
    nb = b / 255.0
    h, s, v = (int(round(e * 255.0)) for e in colorsys.rgb_to_hsv(nr, ng, nb))
    return "    CHSV({}, {}, {}),".format(h, s, v)


def get_hex_palettes(input_fname):
    with open(input_fname) as fobj:
        lines = fobj.read()
    return [palette.strip().split('\n') for palette in lines.split('\n\n')]


def hsv_palette(palette):
    integers = [int(color, base=16) for color in palette]
    return [int_to_hsv_str(i) for i in integers]


def rgb_palette(palette):
    integers = [int(color, base=16) for color in palette]
    return [int_to_rgb_str(i) for i in integers]


def write_palettes(palettes, fname):
    palette_str = '\n\n'.join(
        ['\n'.join(palette) for palette in palettes])
    with open(fname, 'w') as palette_file:
        palette_file.write(palette_str)


def gen_parser():
    parser = argparse.ArgumentParser("Hex -> RGB/HSV Palette Converter")
    parser.add_argument("input_file")

    return parser


def main():
    args = gen_parser().parse_args()
    hex_palettes = get_hex_palettes(args.input_file)
    crgb_palettes = [rgb_palette(palette) for palette in hex_palettes]
    chsv_palettes = [hsv_palette(palette) for palette in hex_palettes]
    write_palettes(crgb_palettes, 'crgb_palettes.txt')
    write_palettes(chsv_palettes, 'chsv_palettes.txt')


if __name__ == '__main__':
    main()
