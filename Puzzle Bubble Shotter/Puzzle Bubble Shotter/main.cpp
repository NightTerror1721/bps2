#include <iostream>

#include "mem_alloc.h"
#include "engine.h"

#include "scenario.h"

#include "paths.h"


class TestCanvas : public GameObject
{
public:
	Scenario _sc;

public:
	TestCanvas(GameController* const & gc, TextureManager* const& tm, const ScenarioProperties& props) :
		GameObject("Test Canvas"),
		_sc{ gc, tm, props }
	{}

	~TestCanvas() override
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
	TextureManager tm{};

	pylib::bindTextureManager(&tm);

	pylib::executePythonScript(Path::BubbleModels + "cache.py");

	Ptr<BubbleModel> model = GetBubbleModel("ColorBubble");
	
	if (model)
		std::cout << model->name << std::endl;



	ScenarioProperties props{};
	props.setColumns(20);
	props.setPlayerId(PlayerId::Single);

	for (u32 i = 0; i < BubbleBoard::VisibleRows; i++)
		props.addNewBubbleRow();

	props.setNewBubbleToRow(13, 0, "ColorBubble", BubbleColor::Green);
	props.setNewBubbleToRow(13, 1, "ColorBubble", BubbleColor::Blue);
	props.setNewBubbleToRow(13, 2, "ColorBubble", BubbleColor::Gray);
	props.setNewBubbleToRow(12, 1, "ColorBubble", BubbleColor::Red);

	gc.createGameObject<TestCanvas>(&gc, &tm, props);



	gc.start();

}
