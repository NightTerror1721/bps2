#pragma once

#include <random>
#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "manager.h"
#include "config.h"

using sf::Texture;

class TextureManager : public Manager<Texture>
{
public:
	explicit TextureManager(TextureManager* const& parent = nullptr);

	void load(const std::string& texture_path, const std::string& name);
	void load(const std::string& texture_path, const std::string& name, const sf::IntRect& dims);
	void load(const std::string& texture_path, const std::string& name, const int& x, const int& y, const int& width, const int& height);
};



class AnimatedSprite : public sf::Sprite
{
public:
	enum class Mode : u8 { Static, Sequence, Loop, Random };

private:
	u32 _x;
	u32 _y;
	u32 _w;
	u32 _h;
	u32 _frames;

	Mode _mode;

	delta_t _min;
	delta_t _max;
	delta_t _current;
	std::minstd_rand _rand;

	delta_t _it;
	delta_t _oldIt;
	delta_t _speed;

	bool _end;

public:
	void setFrameDimensions(const u32& x, const u32& y, const u32& w, const u32& h);
	void setFrameCount(const u32& frames_count);

	void setStaticMode();
	void setSequenceMode();
	void setLoopMode();
	void setRandomMode(const delta_t& min, const delta_t& max);

	bool isStaticMode() const;
	bool isSequenceMode() const;
	bool isLoopMode() const;
	bool isRandomMode() const;

	bool hasEnded() const;

	void start();
	void stop();

	void setCurrentFrame(const u32& frame);
	void setExactCurrentFrame(const delta_t& frame);

	u32 getCurrentFrame() const;
	delta_t getExactCurrentFrame() const;

	void rewind();
	void fastForward();

	u32 getFrameX() const;
	u32 getFrameY() const;
	u32 getFrameWidth() const;
	u32 getFrameHeight() const;
	u32 getFrameCount() const;

	void update(const delta_t& delta);
	void draw(sf::RenderTarget* const& g);

private:
	int state() const;
	void generateCurrent();
	void updateIterator();
};
