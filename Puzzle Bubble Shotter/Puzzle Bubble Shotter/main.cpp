#include <iostream>

#include "mem_alloc.h"
#include "engine.h"

#include "assets.h"
#include "bubbles.h"

#include "paths.h"


int main(int argc, char** argv)
{

	GameController gc("Puzzle Bubble Shooter");


	TextureManager tm{};

	pylib::bindTextureManager(&tm);

	pylib::executePythonScript(Path::BubbleModels + "cache.py");

	Ptr<BubbleModel> model = GetBubbleModel("ColorBubble");
	
	if (model)
		std::cout << model->name << std::endl;


	gc.start();

}
