#!/bin/bash
set -e
cd "$(dirname "$0")"

FPS=60
TITLE_SEC=2

gcc Sorting.c -o sorting
./sorting
python3 render.py
python3 sound.py $FPS

rm -f list.txt
for i in 1 2 3 4 5; do
    echo "Video sort$i..."
    ffmpeg -y -loglevel error \
        -loop 1 -framerate $FPS -t $TITLE_SEC -i sort${i}_title.png \
        -f lavfi -t $TITLE_SEC -i anullsrc=r=44100:cl=mono \
        -c:v libx264 -pix_fmt yuv420p -c:a aac -shortest title$i.mp4
    ffmpeg -y -loglevel error \
        -framerate $FPS -i sort${i}_frames/frame_%05d.png -i sort$i.wav \
        -c:v libx264 -pix_fmt yuv420p -c:a aac -shortest sort$i.mp4
    echo "file 'title$i.mp4'" >> list.txt
    echo "file 'sort$i.mp4'" >> list.txt
done

ffmpeg -y -loglevel error -f concat -safe 0 -i list.txt -c copy all_sorts.mp4
rm list.txt
echo "Done: all_sorts.mp4"
