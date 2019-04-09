import __texture_import as Textures

def loadTexture(texture_filename:str, tag:str, x:int, y:int, w:int, h:int):
    texman = Textures.GetTextureManager();
    if(texman.hasTexture(tag)):
        print("Texture tag \"%s\" already exists" % tag)
        return
    if(texman.loadTexture(texture_filename, tag, x, y, w, h)):
        print("Texture Loaded: \"%s\" with tag \"%s\"" % (texture_filename, tag))
    else:
        print("Fail texture load: \"%s\" with tag \"%s\"" % (texture_filename, tag))

