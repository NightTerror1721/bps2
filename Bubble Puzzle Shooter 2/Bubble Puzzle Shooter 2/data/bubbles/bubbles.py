from data.py.utils import forEachFile
from __stdlib import runfile
import __bubbles as bubs
import os

current_path = os.path.join("data", "bubbles")

def registerBubbleModel(fn):
    print("Import bubble model: %s" % fn)
    locals = {}
    runfile(fn, locals)

forEachFile(current_path, lambda fn: os.path.isfile(fn) and fn.endswith(".py") and not fn.endswith("bubbles.py"), registerBubbleModel)


# FUNCTIONS TO BE IMPORT #

def createBubble(modelName, *args):
    pass


