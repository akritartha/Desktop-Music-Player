import os
import re

files = ["topbar.cpp", "nowplaying.cpp", "controls.cpp", "nowcard.cpp", "songlist.cpp", "playlistbox.cpp"]

for f in files:
    with open(f, "r") as file:
        content = file.read()
    
    if "#define FONT_SCALE" not in content:
        content = re.sub(r'(#include "raylib.h"\n)', r'\1\n#define FONT_SCALE 0.9f\n', content)
    
    def repl(m):
        font_size = m.group(5).strip()
        if "FONT_SCALE" not in font_size:
            font_size = font_size + " * FONT_SCALE"
        return f"{m.group(1)}{m.group(2)}, {m.group(3)}, {m.group(4)}, {font_size}, {m.group(6)}, {m.group(7)})"

    content = re.sub(r'(DrawTextEx\s*\(\s*)([^,]+),\s*([^,]+),\s*([^,]+),\s*([^,]+),\s*([^,]+),\s*([^)]+)\)', repl, content)
    
    def repl_measure(m):
        font_size = m.group(4).strip()
        if "FONT_SCALE" not in font_size:
            font_size = font_size + " * FONT_SCALE"
        return f"{m.group(1)}{m.group(2)}, {m.group(3)}, {font_size}, {m.group(5)})"

    content = re.sub(r'(MeasureTextEx\s*\(\s*)([^,]+),\s*([^,]+),\s*([^,]+),\s*([^)]+)\)', repl_measure, content)

    with open(f, "w") as file:
        file.write(content)

print("Done")
