#!/usr/bin/env python3
"""Generates the title-screen menu artwork this version adds into extras/ ("Game Data
Extras" in the app). New labels are set in Plate Gothic Bold, the font the original
embossed buttons were made from, and given the same bevel, outline and shadow. The rows that
exist in the original art are copied unchanged.

Everything else in extras/ (option tabs, sounds, player sprites) is hand-made artwork and is
not generated here.

Usage: tools/make_extras.py            (needs: pip install pillow numpy)
"""
import os
import numpy as np
from PIL import Image, ImageDraw, ImageFont, ImageFilter

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..")
ORIG = os.path.join(ROOT, "original", "Midnight Mansion HD.app", "Contents", "Resources", "Game Data")
OUT = os.path.join(ROOT, "extras")
FONT = os.path.join(ROOT, "tools", "fonts", "PlateGothic-Bold.otf")

# top highlight, face, bottom (sampled from the original buttons)
YELLOW = [np.array(c, dtype=np.float32) for c in ((235, 235, 120), (205, 205, 30), (120, 120, 0))]
CYAN = [np.array(c, dtype=np.float32) for c in ((150, 255, 235), (20, 235, 195), (0, 120, 100))]

# Rows added after the original seven; their order is the frame order the game expects.
NEW_ROWS = ["Custom Mansions", "Random Mansion", "Trophy Room", "Multiplayer"]

# The multiplayer screen's game types, in the order of kModes in MMMultiplayerScreen.c.
MODE_TITLES = ["Co-op", "Arcade Split Screen", "Ghost Race"]


def text_mask(text, size=49, small=1.12, track=-4.0, space=0.16, stretch=1.12):
    """Plate Gothic's lowercase letters are small capitals; they are set a little larger and
    tighter than the font's defaults, then widened, to match the original lettering."""
    big = ImageFont.truetype(FONT, size)
    sm = ImageFont.truetype(FONT, int(round(size * small)))
    im = Image.new("L", (int(size * len(text) * 1.2) + 40, size * 2), 0)
    d = ImageDraw.Draw(im)
    x, base = 20.0, int(size * 1.3)
    for ch in text:
        if ch == " ":
            x += size * space
            continue
        f = big if ch.isupper() or not ch.isalpha() else sm
        d.text((x, base), ch, font=f, fill=255, anchor="ls")
        x += f.getlength(ch) + track
    bb = im.getbbox()
    out = Image.new("L", (bb[2] - bb[0] + 40, bb[3] - bb[1] + 40), 0)
    out.paste(im.crop(bb), (20, 20))
    return out.resize((int(out.width * stretch), out.height), Image.LANCZOS)


def emboss(mask, colors):
    """Lighter band along the top of the letters, darker toward the bottom, a bevel lit from
    the upper left, a thin black outline and a soft drop shadow."""
    hi, mid, lo = colors
    a = np.asarray(mask, dtype=np.float32) / 255.0
    h, w = a.shape
    ys = np.nonzero(a > 0.5)[0]
    t = np.clip((np.arange(h) - ys.min()) / max(1, ys.max() - ys.min()), 0, 1)[:, None, None]
    base = np.where(t < 0.18, hi * (1 - t / 0.18) + mid * (t / 0.18),
                    mid * (1 - (t - 0.18) / 0.82) + lo * ((t - 0.18) / 0.82))
    base = base + np.zeros((h, w, 1), dtype=np.float32)
    soft = np.asarray(mask.filter(ImageFilter.GaussianBlur(1.6)), dtype=np.float32) / 255.0
    gy, gx = np.gradient(soft)
    light = np.clip(-(gx * 0.6 + gy) * 3.0, -0.6, 0.6)[..., None]
    rgb = np.clip(base * (1 + light), 0, 255)

    out = Image.new("RGBA", mask.size, (0, 0, 0, 0))
    outer = mask.filter(ImageFilter.MaxFilter(3))
    shadow = Image.new("RGBA", mask.size, (0, 0, 0, 0))
    shadow.putalpha(outer.filter(ImageFilter.GaussianBlur(2.5)).point(lambda v: int(v * 0.9)))
    out.alpha_composite(shadow, (2, 4))
    outline = Image.new("RGBA", mask.size, (0, 0, 0, 255))
    outline.putalpha(outer)
    out.alpha_composite(outline)
    face = Image.fromarray(rgb.astype(np.uint8), "RGB").convert("RGBA")
    face.putalpha(mask)
    out.alpha_composite(face)
    return out


def menu_cell(text, colors, w=360, h=60, top=4):
    """One 360x60 cell; labels wider than the cell are narrowed to fit."""
    im = emboss(text_mask(text), colors)
    im = im.crop(im.getbbox())
    if im.width > w - 8:
        im = im.resize((w - 8, im.height), Image.LANCZOS)
    cell = Image.new("RGBA", (w, h), (0, 0, 0, 0))
    cell.alpha_composite(im, ((w - im.width) // 2 + 3, top))
    return cell


def title_buttons():
    """buttons.png from the original plus the new rows (normal on the left, hover on the right)."""
    orig = Image.open(os.path.join(ORIG, "Screens", "titleScreen", "buttons.png")).convert("RGBA")
    rows = orig.height // 60
    sheet = Image.new("RGBA", (720, 60 * (rows + len(NEW_ROWS))), (0, 0, 0, 0))
    sheet.alpha_composite(orig, (0, 0))
    for i, label in enumerate(NEW_ROWS):
        sheet.alpha_composite(menu_cell(label, YELLOW), (0, 60 * (rows + i)))
        sheet.alpha_composite(menu_cell(label, CYAN), (360, 60 * (rows + i)))
    save(sheet, "Screens/titleScreen/buttons_extra.png")


def mode_titles():
    """The game types' names on the multiplayer screen, lettered like the menu buttons: one
    640x64 row each, the name at the left; normal (yellow) on the left, hover (cyan) on the
    right."""
    w, h = 640, 64
    sheet = Image.new("RGBA", (w * 2, h * len(MODE_TITLES)), (0, 0, 0, 0))
    for i, label in enumerate(MODE_TITLES):
        for col, colors in enumerate((YELLOW, CYAN)):
            im = emboss(text_mask(label), colors)
            im = im.crop(im.getbbox())
            if im.width > w - 8:
                im = im.resize((w - 8, im.height), Image.LANCZOS)
            sheet.alpha_composite(im, (w * col + 2, h * i + (h - im.height) // 2))
    save(sheet, "Screens/multiplayer/mode_titles.png")


def save(im, rel):
    path = os.path.join(OUT, rel)
    os.makedirs(os.path.dirname(path), exist_ok=True)
    im.save(path, optimize=True)
    print("wrote", os.path.relpath(path, ROOT))


if __name__ == "__main__":
    title_buttons()
    mode_titles()
