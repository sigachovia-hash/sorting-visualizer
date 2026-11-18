import os
import shutil
import sys

from PIL import Image, ImageDraw, ImageFont

NAMES = {
    "sort1": ("Bubble sort", "O(n^2)"),
    "sort2": ("Shaker sort", "O(n^2)"),
    "sort3": ("Merge sort", "O(n log n)"),
    "sort4": ("Insertion sort", "O(n^2)"),
    "sort5": ("Shell sort", "about O(n^1.5)"),
}

WIDTH, HEIGHT = 1280, 720
BACKGROUND = (25, 25, 35)
BAR = (100, 180, 255)
CHANGED = (240, 90, 90)
TEXT = (255, 255, 255)
SUBTEXT = (150, 160, 180)


def load_font(size):
    try:
        return ImageFont.load_default(size=size)
    except TypeError:
        return ImageFont.load_default()


def read_file(filename):
    frames = []
    with open(filename, "r") as f:
        for line in f:
            line = line.strip()
            if line:
                frames.append([int(x) for x in line.split()])
    return frames


def draw_centered(d, y, text, font, fill):
    left, top, right, bottom = d.textbbox((0, 0), text, font=font)
    d.text(((WIDTH - (right - left)) / 2, y), text, font=font, fill=fill)


def draw_title(path, title, complexity, n, steps):
    img = Image.new("RGB", (WIDTH, HEIGHT), BACKGROUND)
    d = ImageDraw.Draw(img)
    draw_centered(d, 250, title, load_font(96), TEXT)
    draw_centered(d, 390, f"{complexity}   |   n = {n}   |   {steps} steps", load_font(40), SUBTEXT)
    img.save(path)


def draw(filename, frames, font):
    short_name = os.path.splitext(os.path.basename(filename))[0]
    base = os.path.dirname(filename)
    folder = os.path.join(base, short_name + "_frames")
    shutil.rmtree(folder, ignore_errors=True)
    os.makedirs(folder)

    title, complexity = NAMES.get(short_name, (short_name, ""))
    draw_title(os.path.join(base, short_name + "_title.png"), title, complexity, len(frames[0]), len(frames) - 1)

    max_num = max(max(frame) for frame in frames) or 1
    bar_w = WIDTH / len(frames[0])
    top = 70
    bottom = HEIGHT - 20
    prev = None

    print(f"{title}: {len(frames)} frames -> {folder}")

    for i, frame in enumerate(frames):
        if len(frame) != len(frames[0]):
            raise ValueError(f"{filename}, line {i + 1}: expected {len(frames[0])} numbers, got {len(frame)}")

        changed = set()
        if prev is not None:
            changed = {j for j in range(len(frame)) if frame[j] != prev[j]}

        img = Image.new("RGB", (WIDTH, HEIGHT), BACKGROUND)
        d = ImageDraw.Draw(img)

        for j, num in enumerate(frame):
            bar_h = int(num / max_num * (bottom - top))
            x0 = int(j * bar_w) + 1
            x1 = max(x0, int((j + 1) * bar_w) - 1)
            color = CHANGED if j in changed else BAR
            d.rectangle([x0, bottom - bar_h, x1, bottom], fill=color)

        d.text((20, 20), f"{title}   step {i + 1} / {len(frames)}", fill=TEXT, font=font)
        img.save(os.path.join(folder, f"frame_{i:05d}.png"))
        prev = frame


def main():
    files = sys.argv[1:] or [f"sort{k}.txt" for k in range(1, 6)]
    font = load_font(32)
    for f in files:
        if os.path.exists(f):
            draw(f, read_file(f), font)
        else:
            print(f"File not found: {f}")


if __name__ == "__main__":
    main()
