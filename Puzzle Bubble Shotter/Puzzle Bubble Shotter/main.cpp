#include <iostream>

#include "mem_alloc.h"
#include "engine.h"

#include "scenario.h"

#include "paths.h"


class TestCanvas : public GameObject
{
private:
	Scenario _sc;

public:
	TestCanvas(GameController* const & gc, const ScenarioProperties& props) :
		GameObject("Test Canvas"),
		_sc{ gc, props }
	{}

	~TestCanvas()
	{
		std::cout << "fe" << std::endl;
	}

	void update(const delta_t& delta)
	{
		_sc.update(delta);
	}

	void dispatchEvent(const InputEvent& event)
	{
		_sc.dispatchEvent(event);
	}

	void draw(sf::RenderTarget* const& g) override
	{
		_sc.draw(g);
	}
};


int main(int argc, char** argv)
{

	GameController gc("Puzzle Bubble Shooter");

	ScenarioProperties props{};
	props.setColumns(20);
	props.setPlayerId(PlayerId::Single);

	gc.createGameObject<TestCanvas>(&gc, props);


	TextureManager tm{};
	


	pylib::bindTextureManager(&tm);

	pylib::executePythonScript(Path::BubbleModels + "cache.py");

	Ptr<BubbleModel> model = GetBubbleModel("ColorBubble");
	
	if (model)
		std::cout << model->name << std::endl;


	gc.start();

}
