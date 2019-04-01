from __stdlib import runfile, current_dir
from data.py.utils import forEachFile
import os

current_path = os.path.join("data", "textures")

def registerFolder(folder_name):
    print("register folder \"%s\":" % folder_name)
    folder_cache = os.path.join(folder_name, "cache.py")
    runfile(folder_cache)

forEachFile(current_path, lambda fn: os.path.isdir(fn), registerFolder)
