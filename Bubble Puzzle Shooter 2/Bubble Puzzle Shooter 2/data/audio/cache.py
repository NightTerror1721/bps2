from __audio import loadSound
import os

current_path = os.path.join("data", "audio")
sound_path = os.path.join(current_path, "sounds")
music_path = os.path.join(current_path, "music")

def valid_file(fn):
    return os.path.isfile(fn) and fn.endswith(".ogg")

def importSound(sound_file, tag):
    path = os.path.join(sound_path, sound_file)
    if not valid_file(path):
        return
    loadSound(path, tag)
    print("Sound \"%s\" from file \"%s\" loaded!" % (tag, path))

importSound("bombBubbleExpl.ogg", "bombBubbleExpl")
importSound("bubbleExpl.ogg", "bubbleExpl")
importSound("combo1.ogg", "combo1")
importSound("combo2.ogg", "combo2")
importSound("combo3.ogg", "combo3")
importSound("combo4.ogg", "combo4")
importSound("crateCollect.ogg", "crateCollect")
importSound("fire.ogg", "fire")
importSound("freeze.ogg", "freeze")
importSound("frozenBubbleExpl.ogg", "frozenBubbleExpl")
importSound("hitBubble.ogg", "hitBubble")
importSound("imminent1.ogg", "imminent1")
importSound("imminent2.ogg", "imminent2")
importSound("ironBubbleExpl.ogg", "ironBubbleExpl")
importSound("lightningExpl.ogg", "lightningExpl")
importSound("lose.ogg", "lose")
importSound("magicExpl.ogg", "magicExpl")
importSound("move.ogg", "move")
importSound("paintingBubbleExpl.ogg", "paintingBubbleExpl")
importSound("ready.ogg", "ready")
importSound("select.ogg", "select")
importSound("starBubbleExpl.ogg", "starBubbleExpl")
importSound("todown.ogg", "todown")
importSound("won.ogg", "won")

