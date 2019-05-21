#include "bubble_gen.h"
#include "bub_board.h"

#include <map>

RandomBubbleModelSelector::RandomBubbleModelSelector() :
	_models(),
	_count(0)
{}

std::string RandomBubbleModelSelector::select(RNG& rand) const
{
	if (!_count || _models.empty())
		return "";
	u32 value = rand(_count);

	for (auto& model : _models)
	{
		if (model.second > value)
			return model.first;
		value -= model.second;
	}
	return "";
}

void RandomBubbleModelSelector::setPercent(const std::string& modelName, u16 percent)
{
	if (HasBubbleModel(modelName) && percent)
	{
		_count += percent;
		_models[modelName] = percent;
	}
}

u16 RandomBubbleModelSelector::getPercent(const std::string& modelName) const
{
	const auto& it = _models.find(modelName);
	return it != _models.end() ? it->second : 0;
}




BubbleColorSelector::BubbleColorSelector(const RNG& rand) :
	_rand{ rand },
	_availableColors{ 0 },
	_colors{},
	_lastColor{ BubbleColor::defaultColor() }
{}

void BubbleColorSelector::setAvailableColors(colormask_t colors)
{
	_availableColors = colors;
	update();
}

void BubbleColorSelector::setColorEnabled(const BubbleColor& color, bool enabled)
{
	_availableColors = enabled ? _availableColors + color : _availableColors - color;
	update();
}

bool BubbleColorSelector::isColorEnabled(const BubbleColor& color) const { return _availableColors & color; }

const BubbleColor& BubbleColorSelector::select()
{
	_lastColor = _colors.empty() ? _lastColor : _colors[_rand(_colors.size())];
	return _lastColor;
}

const BubbleColor& BubbleColorSelector::select(const BubbleBoard* board)
{
	std::map<BubbleColor, u32> scores;
	u32 baseScore = 1;
	u32 totalScore = 0;
	for (const BubbleRow* row = board->getFirstVisibleRow(); row; row = row->bottom())
	{
		for(const BubbleCell& cell : *row)
			if (cell && cell->getColorType() == ColorType::NormalColor)
			{
				const BubbleColor& color = cell->getColor();
				if (!(_availableColors & color) || board->isBubbleInaccesible(*cell))
					continue;

				const auto& it = scores.find(color);
				if (it == scores.end())
					scores[color] = baseScore;
				else it->second += baseScore;
				totalScore += baseScore;
			}
		++baseScore;
	}

	if (!totalScore)
	{
		if (scores.empty())
			return _lastColor;
		u32 value = _rand(scores.size());
		auto it = scores.begin();
		while (value)
		{
			++it;
			++value;
		}
		return it->first;
	}

	u32 value = _rand(totalScore);
	for (auto& scoredColor : scores)
	{
		if (scoredColor.second && value < scoredColor.second)
			return scoredColor.first;
		value -= scoredColor.second;
	}
	return select();
}

void BubbleColorSelector::update()
{
	_colors.clear();
	for (auto& color : BubbleColor::all())
		if (_availableColors & color)
			_colors.push_back(color);
}
