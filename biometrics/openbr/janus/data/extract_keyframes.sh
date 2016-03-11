#!/bin/bash

f=$1

# get fps
fps=`ffprobe $f 2>&1 | sed -n "s/.*, \(.*\) fp.*/\1/p"`
if [ -z "$fps" ]; then
    exit 1
fi

dir="${f%.*}_${fps}"
mkdir $dir

# times of all frames
frame_times=($(ffmpeg -i $f -vf "showinfo" -f null /dev/null 2>&1 | \
    sed -n "s/.* n:\(.*\) pts:.* pts_time:\(.*\) pos:.*/\2/p"))
if [ -z "$frame_times" ]; then
    exit 1
fi

# get the frame number of keyframes
key_frames=$(ffprobe -select_streams 0:v:0 -show_frames $f 2>/dev/null | \
    grep key_frame | \
    awk "/key_frame=1/ { print FNR-1 }")
if [ -z "$key_frames" ]; then
    exit 1
fi

# extract each keyframe
for frame_num in $key_frames; do
    ffmpeg -ss ${frame_times[$frame_num]} -i $f -frames:v 1 "${dir}/${f%.*}_${frame_num}.png" 2>/dev/null
done
