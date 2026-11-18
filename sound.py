import math
import os
import sys
import wave
from array import array

RATE = 44100
LOW_HZ = 200
HIGH_HZ = 1200
VOLUME = 0.25
FADE = 0.15


def read_file(filename):
    frames = []
    with open(filename, "r") as f:
        for line in f:
            line = line.strip()
            if line:
                frames.append([int(x) for x in line.split()])
    return frames


def make_sound(filename, fps):
    frames = read_file(filename)
    max_num = max(max(frame) for frame in frames) or 1
    total = len(frames) * RATE // fps
    samples = array("h", [0]) * total
    prev = None

    for i, frame in enumerate(frames):
        start = i * RATE // fps
        end = (i + 1) * RATE // fps
        changed = [frame[j] for j in range(len(frame)) if prev is not None and frame[j] != prev[j]]
        prev = frame
        if not changed:
            continue

        freq = LOW_HZ + max(changed) / max_num * (HIGH_HZ - LOW_HZ)
        length = end - start
        fade = max(1, int(length * FADE))
        for k in range(length):
            env = min(1.0, k / fade, (length - k) / fade)
            value = math.sin(2 * math.pi * freq * k / RATE)
            samples[start + k] = int(32767 * VOLUME * env * value)

    out = os.path.splitext(filename)[0] + ".wav"
    with wave.open(out, "wb") as w:
        w.setnchannels(1)
        w.setsampwidth(2)
        w.setframerate(RATE)
        w.writeframes(samples.tobytes())
    print(f"{filename} -> {out}")


def main():
    fps = int(sys.argv[1]) if len(sys.argv) > 1 else 60
    files = sys.argv[2:] or [f"sort{k}.txt" for k in range(1, 6)]
    for f in files:
        if os.path.exists(f):
            make_sound(f, fps)
        else:
            print(f"File not found: {f}")


if __name__ == "__main__":
    main()
