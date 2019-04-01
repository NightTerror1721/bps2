import __bubbles as bubs

if not bubs.isRegisteredBubbleModel("ColorBubble"):
    model = bubs.registerBubbleModel("ColorBubble")

    def __model__init(self):
        pass



    model.init = __model__init
    
    print(model)
    print(model.init)

