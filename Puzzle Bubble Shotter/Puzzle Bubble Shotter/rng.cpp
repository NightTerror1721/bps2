#include "rng.h"

#include <chrono>

rand_value_t::rand_value_t(const unsigned int& value) :
	_value(value)
{}

rand_value_t::operator unsigned int() const { return _value; }

bool rand_value_t::operator==(const unsigned int& value) const { return _value == value; }
bool rand_value_t::operator!=(const unsigned int& value) const { return _value != value; }
bool rand_value_t::operator>(const unsigned int& value) const { return _value > value; }
bool rand_value_t::operator<(const unsigned int& value) const { return _value < value; }
bool rand_value_t::operator>=(const unsigned int& value) const { return _value >= value; }
bool rand_value_t::operator<=(const unsigned int& value) const { return _value <= value; }

bool operator==(const unsigned int& value, const rand_value_t& rand) { return value == rand._value; }
bool operator!=(const unsigned int& value, const rand_value_t& rand) { return value == rand._value; }
bool operator>(const unsigned int& value, const rand_value_t& rand) { return value == rand._value; }
bool operator<(const unsigned int& value, const rand_value_t& rand) { return value == rand._value; }
bool operator>=(const unsigned int& value, const rand_value_t& rand) { return value == rand._value; }
bool operator<=(const unsigned int& value, const rand_value_t& rand) { return value == rand._value; }



RNG::RNG(const seed_t& seed) :
	_rand{ seed == 0 ? static_cast<seed_t>(std::chrono::system_clock::now().time_since_epoch().count()) : seed }
{}

void RNG::seed(const seed_t& seed) { _rand.seed(seed == 0 ? static_cast<seed_t>(std::chrono::system_clock::now().time_since_epoch().count()) : seed); }

rand_value_t RNG::operator() () { return { _rand() }; }
rand_value_t RNG::operator() (const rand_value_t& max) { return { _rand() % max }; }
rand_value_t RNG::operator() (const rand_value_t& min, const rand_value_t& max)
{
	return { (_rand() % (max - min)) + min };
}

float RNG::generateFloat() { return static_cast<float>(_rand()) / static_cast<float>(max()); }
