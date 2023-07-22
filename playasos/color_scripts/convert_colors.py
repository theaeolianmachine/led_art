import colorsys

colors = [
    # Red
    [
        (1, 0, 0), (1, 0.388, 0.388)
    ],
    # Orange/Yellow
    [
        (1, 0.573, 0), (1, 0.737, 0.388)
    ],
    # Blue
    [
        (0.043, 0.38, 0.643), (0.314, 0.549, 0.733)
    ],
    # Cyan
    [
        (0, 0.6, 0.6), (.275, .698, .698)
    ],
    # Yellow
    [
        (1, 0.827, 0), (1, 0.894, 0.388)
    ],
    # Yellow-Green
    [
        (0.8, 0.965, 0), (0.871, 0.973, 0.38),
    ],
    # Blue
    [
        (0.224, 0.078, 0.686), (0.451, 0.353, 0.765),
    ],
    # Violet
    [
        (0.804, 0, 0.455), (0.851, 0.333, 0.627),
    ]
]


def print_for_arduino(color):
    return 'CHSV({}, {}, {})'.format(*color)


def convert_to_hsv(color_set):
    hsvs = [colorsys.rgb_to_hsv(*color) for color in color_set]
    return [
        print_for_arduino([int(round(255 * i)) for i in color])
        for color in hsvs
    ]


if __name__ == '__main__':
    for color in colors:
        for c in convert_to_hsv(color):
            print c
