from data.py.utils import forEachFile
from __stdlib import runfile
import __bubbles as bubs
import os

current_path = os.path.join("data", "bubbles")

def registerBubbleModel(fn):
    print("Import bubble model: %s" % fn)
    runfile(fn)

forEachFile(current_path, lambda fn: os.path.isfile(fn) and fn.endswith(".py") and not fn.endswith("cache.py"), registerBubbleModel)


# FUNCTIONS TO BE IMPORT #


