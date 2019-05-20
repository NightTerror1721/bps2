import PBS_bubbles as Bubbles
import PBS_textures as Textures

import os

ModelName = "ColorBubble"

if not Bubbles.existsBubbleModel(ModelName):

    def init(bubble, color, editorMode):
        sprite = bubble.getSprite()
        sprite.setTexture("bubble.colorbubble." + color.name)
        sprite.setFrameDimensions(0, 0, 32, 32)
        sprite.setFrameCount(12)
        sprite.setFrameSpeed(20.0)
        sprite.setRandomMode(1.5, 6)
        sprite.start()
        
        bubble.setColor(color)
        bubble.updateSpriteScale()
        
    def onCollide(self, other):
        pass
        
    def onInserted(self):
        pass

    def onExplode(self):
        pass
        
    def onNeighborInserted(bubble, color, editorMode):
        pass

    def onNeighborExplode(bubble, color, editorMode):
        pass
        
        
    def precache():
        all_colors = Bubbles.BubbleColor.all()
        index = 0
        for color in all_colors:
            name = color.name
            Textures.loadTextureFromPart("bubbles/color.png", "bubble.colorbubble." + name, 0, (32 * index), 384, 32)
            Textures.loadTextureFromPart("explosions/color.png", "explosion.colorbubble." + name, 0, (45 * index), 315, 45)
            Textures.loadTextureFromPart("explosions/returns.png", "explosion.particle." + name, (16 * index), 0, 16, 16)
            index += 1
        pass



    model = Bubbles.createBubbleModel(ModelName, precache)
    
    model.colorType = Bubbles.ColorType.NormalColor
    
    model.floating = False
    model.destroyInBottom = False
    model.requireDestroyToClear = False
    
    model.resistence = 0
    
    model.pointsOfTurnsToDown = 1.0
    
    model.localInts = 0
    model.localFloats = 0
    model.localStrings = 0
    
    model.init = init
    model.onCollide = onCollide
    model.onInserted = onInserted
    model.onExplode = onExplode
    model.onNeighborInserted = onNeighborInserted
    model.onNeighborExplode = onNeighborExplode
    
