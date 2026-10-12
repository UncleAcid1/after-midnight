#!/usr/bin/env python3
"""Draws the Mac disk image's window background (platform/macos/dmg/background.png and
background@2x.png) from the original title screen's art: the night sky, the moon, the logo,
"After Midnight" in Black Chancery, the fog, and a gold arrow from the game to Applications.

The icon positions must match make_dmg.sh (APP_X, APPS_X, ICON_Y). Finder writes the icons'
names in black (light mode) or white (dark mode), so they sit on a mid-toned band of fog
where either reads.

Usage: tools/make_dmg_background.py        (needs: pip install pillow)
"""
import os
from PIL import Image, ImageDraw, ImageFont, ImageFilter

ROOT = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..")
G = os.path.join(ROOT, "original", "Midnight Mansion HD.app", "Contents", "Resources", "Game Data") + "/"
T = G + "Screens/titleScreen/"
F = G + "Fonts/"
OUT = os.path.join(ROOT, "platform", "macos", "dmg") + "/"

S = 2                                   # drawn at 2x (Retina), scaled down for the 1x copy
W, H = 660 * S, 420 * S                 # the window (make_dmg.sh: --window-size)
APP_X, APPS_X, ICON_Y = 175, 485, 235   # icon centres at 1x (make_dmg.sh: --icon, --app-drop-link)


def shadowed(img, xy, text, font, fill, blur=3):
    s = Image.new("RGBA", img.size, (0, 0, 0, 0))
    ImageDraw.Draw(s).text((xy[0] + 2 * S, xy[1] + 3 * S), text, font=font, fill=(0, 0, 0, 220), anchor="mm")
    img.alpha_composite(s.filter(ImageFilter.GaussianBlur(blur * S)))
    ImageDraw.Draw(img).text(xy, text, font=font, fill=fill, anchor="mm")


def main():
    bg = Image.open(T + "starBackground.jpg").convert("RGBA").resize((W, int(W * 768 / 1024))).crop((0, 0, W, H))

        # the moon, off to the right of the logo
    mx, my, md = W - 128 * S, 44 * S, 118 * S
    glow = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    ImageDraw.Draw(glow).ellipse((mx, my, mx + md, my + md), fill=(120, 220, 230, 70))
    bg.alpha_composite(glow.filter(ImageFilter.GaussianBlur(30 * S)))
    bg.alpha_composite(Image.open(T + "full_moon.png").convert("RGBA").resize((md, md)), (mx, my))
    for name, xy, sc in (("cloud_2.png", (-60, 80), 0.5), ("cloud_3.png", (520, 130), 0.45)):
        c = Image.open(T + name).convert("RGBA")
        bg.alpha_composite(c.resize((int(c.width * sc * S), int(c.height * sc * S))), (xy[0] * S, xy[1] * S))

        # the logo and the edition's name
    logo = Image.open(T + "logo.png").convert("RGBA")
    lw = 480 * S
    logo = logo.resize((lw, int(logo.height * lw / logo.width)), Image.LANCZOS)
    bg.alpha_composite(logo, ((W - lw) // 2 - 20 * S, 18 * S))
    shadowed(bg, (W // 2 - 20 * S, 112 * S), "After Midnight", ImageFont.truetype(F + "BlackChancery.TTF", 40 * S),
             (242, 220, 140))

        # the fog the icons' names sit on
    band = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    bd = ImageDraw.Draw(band)
    for y in range(H):
        t = (y / S - 262) / 46
        bd.line([(0, y), (W, y)], fill=(104, 122, 146, int(215 * (0 if t < 0 else min(1, t)))))
    bg.alpha_composite(band)
    fog = Image.open(T + "foreground_fog.png").convert("RGBA").resize((int(W * 1.25), 190 * S))
    bg.alpha_composite(fog, (-int(W * 0.12), H - 190 * S))

        # soft light behind each icon, and the arrow between them
    halo = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    hd = ImageDraw.Draw(halo)
    for x in (APP_X, APPS_X):
        hd.ellipse(((x - 70) * S, (ICON_Y - 66) * S, (x + 70) * S, (ICON_Y + 66) * S), fill=(255, 225, 150, 46))
    bg.alpha_composite(halo.filter(ImageFilter.GaussianBlur(18 * S)))
    arrow = Image.new("RGBA", (W, H), (0, 0, 0, 0))
    ad = ImageDraw.Draw(arrow)
    x0, x1, y = (APP_X + 78) * S, (APPS_X - 80) * S, ICON_Y * S
    gold = (242, 214, 120, 255)
    for i in range(0, x1 - 26 * S - x0, 16 * S):
        ad.rounded_rectangle((x0 + i, y - 3 * S, x0 + i + 9 * S, y + 3 * S), 3 * S, fill=gold)
    ad.polygon([(x1, y), (x1 - 24 * S, y - 15 * S), (x1 - 24 * S, y + 15 * S)], fill=gold)
    shadow = Image.new("RGBA", (W, H), (0, 0, 0, 255))
    shadow.putalpha(arrow.getchannel("A"))
    bg.alpha_composite(shadow.filter(ImageFilter.GaussianBlur(3 * S)), (2 * S, 3 * S))
    bg.alpha_composite(arrow)
    shadowed(bg, ((APP_X + APPS_X) // 2 * S, (ICON_Y - 30) * S), "drag to install",
             ImageFont.truetype(F + "Action Man.ttf", 15 * S), (236, 236, 228))
    shadowed(bg, (W // 2, 384 * S), "Midnight Mansion created by Vern Jensen   -   An UncleAcid1 project",
             ImageFont.truetype(F + "Action Man.ttf", 11 * S), (220, 224, 232), blur=2)

    os.makedirs(OUT, exist_ok=True)
    bg.convert("RGB").save(OUT + "background@2x.png", optimize=True)
    bg.convert("RGB").resize((W // S, H // S), Image.LANCZOS).save(OUT + "background.png", optimize=True)
    print("wrote", OUT + "background.png", "and background@2x.png")


if __name__ == "__main__":
    main()
