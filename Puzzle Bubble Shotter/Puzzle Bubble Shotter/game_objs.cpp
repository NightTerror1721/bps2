#include "game_objs.h"

#include "engine.h"

GameObject::GameObject(const std::string& tag) :
	UniqueObject(),
	_tag(tag),
	_gc(nullptr)
{}

const std::string& GameObject::tag() const { return _tag; }
void GameObject::tag(const std::string& tag) { _tag = tag; }

bool GameObject::hasAttached() const { return !_gc; }

void GameObject::draw(sf::RenderTarget* const& g) {}
void GameObject::update(const delta_t& delta) {}
void GameObject::dispatchEvent(const sf::Event& event) {}

GameController* GameObject::getGameController() { return _gc; }
