#pragma once

#include "rng.h"
#include "bubbles.h"

#include <vector>
#include <map>


class Scenario;
class BubbleBoard;

class RandomBubbleModelSelector
{
private:
	std::map<std::string, u16> _models;
	u32 _count;

public:
	RandomBubbleModelSelector();

	std::string select(RNG& rand) const;

	void setPercent(const std::string& modelName, u16 percent);
	u16 getPercent(const std::string& modelName) const;
};



class BubbleColorSelector
{
private:
	RNG _rand;
	colormask_t _availableColors;
	std::vector<BubbleColor> _colors;
	BubbleColor _lastColor;

public:
	BubbleColorSelector(const RNG& rand);

	void setAvailableColors(colormask_t colors);

	void setColorEnabled(const BubbleColor& color, bool enabled);
	bool isColorEnabled(const BubbleColor& color) const;

	const BubbleColor& select();
	const BubbleColor& select(const BubbleBoard* board);

private:
	void update();
};



/*class BubbleGenerator
{
private:
	Scenario* const _sc;
	RNG _arrowRand;
	RNG _boardRand;
	RandomBubbleModelSelector _arrowSelector;
	RandomBubbleModelSelector _boardSelector;

public:
	BubbleGenerator(Scenario* sc, seed_t seed);



private:
	const RandomBubbleModelSelector& selector(bool isArrow) const;
	RNG& rand(bool isArrow);
};*/
