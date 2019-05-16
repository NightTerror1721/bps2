#pragma once

#include <random>

using seed_t = unsigned int;

class RNG;

class rand_value_t
{
private:
	unsigned int _value;

public:
	operator unsigned int() const;

	bool operator==(const unsigned int& value) const;
	bool operator!=(const unsigned int& value) const;
	bool operator>(const unsigned int& value) const;
	bool operator<(const unsigned int& value) const;
	bool operator>=(const unsigned int& value) const;
	bool operator<=(const unsigned int& value) const;

	friend bool operator==(const unsigned int& value, const rand_value_t& rand);
	friend bool operator!=(const unsigned int& value, const rand_value_t& rand);
	friend bool operator>(const unsigned int& value, const rand_value_t& rand);
	friend bool operator<(const unsigned int& value, const rand_value_t& rand);
	friend bool operator>=(const unsigned int& value, const rand_value_t& rand);
	friend bool operator<=(const unsigned int& value, const rand_value_t& rand);

	friend class RNG;

private:
	rand_value_t(const unsigned int& value);
};

bool operator==(const unsigned int& value, const rand_value_t& rand);
bool operator!=(const unsigned int& value, const rand_value_t& rand);
bool operator>(const unsigned int& value, const rand_value_t& rand);
bool operator<(const unsigned int& value, const rand_value_t& rand);
bool operator>=(const unsigned int& value, const rand_value_t& rand);
bool operator<=(const unsigned int& value, const rand_value_t& rand);

class RNG
{
private:
	std::minstd_rand _rand;

public:
	RNG(const seed_t& seed = 0);

	void seed(const seed_t& seed);

	rand_value_t operator() ();
	rand_value_t operator() (const rand_value_t& max);
	rand_value_t operator() (const rand_value_t& min, const rand_value_t& max);

	float generateFloat();

	static constexpr unsigned int max() { return std::minstd_rand::max(); }
	static constexpr unsigned int min() { return std::minstd_rand::min(); }
};
