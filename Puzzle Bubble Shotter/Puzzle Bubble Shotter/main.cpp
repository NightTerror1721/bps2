#include <iostream>

#include "mem_alloc.h"
#include "engine.h"

#include "assets.h"
#include "bubbles.h"

#include "paths.h"


class TestCanvas : public GameObject
{
private:
	GameController* _gc;

public:
	TestCanvas(GameController* const & gc) :
		GameObject("Test Canvas"),
		_gc(gc)
	{}

	~TestCanvas()
	{
		std::cout << "fe" << std::endl;
	}

	void draw(sf::RenderTarget* const& g) override
	{
		sf::View view = _gc->getWindow()->getDefaultView();
		view.zoom(2.f);
		_gc->getWindow()->setView(view);

		sf::RectangleShape square{ { 100, 100 } };
		square.setFillColor(sf::Color::Blue);
		square.setPosition(-200, -100);

		g->draw(square);

		_gc->getWindow()->setView(_gc->getWindow()->getDefaultView());
	}
};


int main(int argc, char** argv)
{

	GameController gc("Puzzle Bubble Shooter");

	gc.createGameObject<TestCanvas>(&gc);


	TextureManager tm{};
	


	pylib::bindTextureManager(&tm);

	pylib::executePythonScript(Path::BubbleModels + "cache.py");

	Ptr<BubbleModel> model = GetBubbleModel("ColorBubble");
	
	if (model)
		std::cout << model->name << std::endl;


	gc.start();

}
