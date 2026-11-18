import os
from PIL import Image, ImageDraw

files = ["sort1.txt", "sort2.txt", "sort3.txt", "sort4.txt", "sort5.txt"]

names = {
    "sort1": "Bubble", "sort2": "Shaker", "sort3": "Merge",
    "sort4": "Insertion", "sort5": "Shell"
}

def read_file(filename):
    frames = []
    with open(filename, "r") as f:
        for line in f:
            line = line.strip()
            if line:
                frames.append([int(x) for x in line.split()])
    return frames

def draw(filename, frames):
    short_name = filename.replace(".txt", "")
    folder = short_name + "_frames"
    os.makedirs(folder, exist_ok=True)

    w, h = 1000, 600
    max_num = max(max(frame) for frame in frames) or 1
    prev = None

    print(f"Делаю {short_name}...")

    for i, frame in enumerate(frames):
        changed = []
        if prev is not None:
            if len(frame) == len(prev):
                for j in range(len(frame)):
                    if frame[j] != prev[j]:
                        changed.append(j)

        img = Image.new("RGB", (w, h), (25, 25, 35))
        d = ImageDraw.Draw(img)
        bar_w = w / len(frame)

        for j, num in enumerate(frame):
            bar_h = int((num / max_num) * (h - 60))
            x0 = int(j * bar_w) + 1
            x1 = int((j + 1) * bar_w) - 1
            y0 = h - 20 - bar_h
            
            color = (240, 90, 90) if j in changed else (100, 180, 255)
            d.rectangle([x0, y0, x1, h - 20], fill=color)
        
        title = names.get(short_name, short_name)
        d.text((15, 15), f"{title} | Шаг {i+1} из {len(frames)}", fill=(255, 255, 255))
        img.save(os.path.join(folder, f"frame_{i:05d}.png"))
        prev = frame

for f in files:
    if os.path.exists(f):
        draw(f, read_file(f))
    else:
        print(f"Нет файла {f}")

print("Готово! Папки с картинками созданы.")
