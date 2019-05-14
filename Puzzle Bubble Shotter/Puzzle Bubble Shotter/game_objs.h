#pragma once

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Window/Event.hpp>

#include "config.h"
#include "inputs.h"

class GameController;

class GameObject : public UniqueObject
{
private:
	std::string _tag;
	GameController* _gc;

public:
	GameObject(const std::string& tag);
	virtual ~GameObject();

	const std::string& tag() const;
	void tag(const std::string& tag);

	bool hasAttached() const;

	virtual void draw(sf::RenderTarget* const& g);
	virtual void update(const delta_t& delta);
	virtual void dispatchEvent(const InputEvent& event);

	friend class GameController;

protected:
	GameController* getGameController();
};
