import __texture_import as cpp_tex

global __textures
__textures = {}

def registerTexture(texture_filename:str, tag:str, x:int, y:int, w:int, h:int):
    global __textures
    if tag in __textures:
        print("Texture tag \"%s\" already exists" % tag)
        return
    
    __textures[tag] = (texture_filename, x, y, w, h)
    print("Texture Registered: \"%s\" with tag \"%s\"" % (texture_filename, tag))

def loadTexture(textureManager, tag:str):
    global __textures
    if tag not in __textures:
        return
    filename, x, y, w, h = __texture_import[tag]
    textureManager.loadTexture(filename, tag, x, y, w, h)

