import colorsys

HUES = (5, 127, 42, 170, 234, 191)
SAT = ((255 + 1) / 256.0)
VALUE = ((127 + 1) / 256.0)


def convert_ratios(ratio):
    return [max(int(num * 256) - 1, 0) for num in ratio]


def rgb_str(rgb_tuple):
    return '({}, {}, {})'.format(*rgb_tuple)


def main():
    hue_ratios = [(hue + 1) / 256.0 for hue in HUES]
    rgb_ratios = [colorsys.hsv_to_rgb(hue, SAT, VALUE) for hue in hue_ratios]
    rgb_values = [convert_ratios(rgb_ratio) for rgb_ratio in rgb_ratios]
    print 'RGB Values:'
    for rgb in rgb_values:
        print rgb_str(rgb)


if __name__ == '__main__':
    main()
