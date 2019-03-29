from __stdlib import runfile, current_dir
import os

current_path = os.path.join("data", "textures")

def registerFolder(folder_name):
    folder_cache = os.path.join(folder_name, "cache.py")
    runfile(folder_cache)

for filename in os.listdir(current_path):
    filename = os.path.join(current_path, filename)
    if os.path.isdir(filename):
        print("register folder \"%s\":" % filename)
        registerFolder(filename)

