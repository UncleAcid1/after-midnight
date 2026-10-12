#!/usr/bin/env python3
"""Make the split screen's background from the game's own dark brick wall tile:

  extras/Screens/split/brick.png   tiles the space around and between the players' views

(Game Data/Sprites/00209.png holds the dark tile family; its top 2x2 tiles are the brick.)
"""
import os
from PIL import Image, ImageEnhance

ROOT = os.path.join(os.path.dirname(__file__), "..")
SRC = os.path.join(ROOT, "original/Midnight Mansion HD.app/Contents/Resources/Game Data/Sprites/00209.png")
OUT = os.path.join(ROOT, "extras/Screens/split")


def main():
    os.makedirs(OUT, exist_ok=True)
    brick = Image.open(SRC).convert("RGB").crop((0, 0, 64, 64))
    brick = ImageEnhance.Brightness(brick).enhance(0.55)    # as dark as the mansion's unlit walls
    brick.save(os.path.join(OUT, "brick.png"))


if __name__ == "__main__":
    main()
