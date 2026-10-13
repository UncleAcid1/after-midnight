# The PS3's XMB pictures and sound

Made for After Midnight by Thom Yorke (UncleAcid1). The package uses the files next to this
folder, made from these:

| In the package | Made from | How |
|---|---|---|
| `ICON0.PNG` (320x176, the game's icon in the Game column) | `icon.png` | scaled to 176 high, centered on a transparent 320x176 |
| `PIC1.PNG` (1920x1080, the background while the game is highlighted) | the splash art | as it is |
| `SND0.AT3` (the sound while the game is highlighted) | `sound.ogg` | ATRAC3 132 kbps stereo in a RIFF file, with [atracdenc](https://github.com/dcherednik/atracdenc) |

    convert icon.png -resize 176x176 -background none -gravity center -extent 320x176 PNG32:../ICON0.PNG
    ffmpeg -i sound.ogg -ar 44100 -ac 2 -sample_fmt s16 sound.wav
    atracdenc -e atrac3 -i sound.wav -o ../SND0.AT3 --container riff
