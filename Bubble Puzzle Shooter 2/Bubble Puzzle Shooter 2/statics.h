#pragma once

#include "assets.h"
#include "bubbles.h"

class Statics
{
private:
	static Statics _instance;

	inline Statics() : texture_manager(nullptr) {}

	Statics(const Statics&) = delete;
	Statics(Statics&&) = delete;

	Statics& operator=(const Statics&) = delete;
	Statics& operator=(Statics&&) = delete;

public:
	friend Statics& statics();

	TextureManager *texture_manager;
	BubbleManager  *bubble_manager;
};

__forceinline Statics& statics() { return Statics::_instance; }


#define set_static(name, value) statics().name = value
#define get_static(name) statics().name
