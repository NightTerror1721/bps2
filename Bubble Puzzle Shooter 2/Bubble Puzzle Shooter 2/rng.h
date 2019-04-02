#pragma once

#include <random>
#include <chrono>

using seed_t = unsigned int;
using rand_value_t = unsigned int;

class RNG
{
private:
	std::minstd_rand _rand;
	rand_value_t _max;
	rand_value_t _min;

public:
	RNG(const seed_t& seed) : _rand(seed), _max(_rand.max()), _min(_rand.min()) {}
	inline RNG() : RNG(static_cast<seed_t>(std::chrono::system_clock::now().time_since_epoch().count())) {}

	inline void seed(const seed_t& seed) { _rand.seed(seed); }

	inline rand_value_t max() const { return _max; }
	inline void max(const rand_value_t& max) { minmax(_min, max); }

	inline rand_value_t min() const { return _min; }
	inline void min(const rand_value_t& min) { minmax(min, _max); }

	__forceinline rand_value_t operator() () { return (*this)(_min, _max); }

	__forceinline rand_value_t operator() (const rand_value_t& max) { return (*this)(_min, max); }

	rand_value_t operator() (const rand_value_t& min, const rand_value_t& max)
	{
		const rand_value_t __max = std::max(min, max);
		const rand_value_t __min = std::min(min, max);
		return (_rand() % (__max - __min)) + __min;
	}

	inline float generateFloat() { _rand() / static_cast<float>(_rand.max()); }

#define CHECK(op, fnc_name) \
inline bool fnc_name(const rand_value_t& value) { return (*this)() op value; } \
inline bool fnc_name(const rand_value_t& max, const rand_value_t& value) { return (*this)(max) op value; } \
inline bool fnc_name(const rand_value_t& min, const rand_value_t& max, const rand_value_t& value) { return (*this)(min, max) op value; } \
inline bool fnc_name(const float& value) { return generateFloat() op value; }

	CHECK(<, check_ls)
	CHECK(>, check_gr)
	CHECK(==, check_eq)
	CHECK(!=, check_ne)

#undef CHECK


private:
	void minmax(const rand_value_t& min, const rand_value_t& max)
	{
		_max = std::max(min, max);
		_min = std::min(min, max);
	}
};

RNG& operator>> (RNG& rng, rand_value_t& value) { value = rng(); return rng; }
RNG& operator>> (RNG& rng, float& value) { value = rng.generateFloat(); return rng; }

bool operator< (RNG& rng, const rand_value_t& value) { return rng() < value; }
bool operator< (RNG& rng, const float& value) { return rng() < value; }

bool operator== (RNG& rng, const rand_value_t& value) { return rng() == value; }
bool operator== (RNG& rng, const float& value) { return rng() == value; }

bool operator!= (RNG& rng, const rand_value_t& value) { return rng() != value; }
bool operator!= (RNG& rng, const float& value) { return rng() != value; }

bool operator> (RNG& rng, const rand_value_t& value) { return rng() > value; }
bool operator> (RNG& rng, const float& value) { return rng() > value; }
