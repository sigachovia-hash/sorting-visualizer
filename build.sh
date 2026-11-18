#!/bin/bash
set -e
cd "$(dirname "$0")"

FPS=60

gcc Sorting.c -o sorting
./sorting
python3 render.py

rm -f list.txt
for i in 1 2 3 4 5; do
    echo "Video sort$i..."
    ffmpeg -y -loglevel error -framerate $FPS -i sort${i}_frames/frame_%05d.png \
        -c:v libx264 -pix_fmt yuv420p sort$i.mp4
    echo "file 'sort$i.mp4'" >> list.txt
done

ffmpeg -y -loglevel error -f concat -safe 0 -i list.txt -c copy all_sorts.mp4
rm list.txt
echo "Done: all_sorts.mp4"
