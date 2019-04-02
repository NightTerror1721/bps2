#pragma once
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>

using int8  = sf::Int8;
using int16 = sf::Int16;
using int32 = sf::Int32;
using int64 = sf::Int64;

using uint8 =  sf::Uint8;
using uint16 = sf::Uint16;
using uint32 = sf::Uint32;
using uint64 = sf::Uint64;

using Pixel = uint32;

using Color = sf::Color;

using Vec2 = sf::Vector2f;
using Vec2f = Vec2;
using Vec2i = sf::Vector2i;
using Vec2u = sf::Vector2u;

using delta_t = float;

using std::byte;


/* UTILS */
class uuid
{
private:
	uint64 code;

	inline uuid(uint64 code) : code(code) {}

public:

	static uuid generate();

	inline bool operator== (const uuid& id) const { return code == id.code; }
	inline bool operator!= (const uuid& id) const { return code != id.code; }
	inline bool operator> (const uuid& id) const { return code > id.code; }
	inline bool operator>= (const uuid& id) const { return code >= id.code; }
	inline bool operator< (const uuid& id) const { return code < id.code; }
	inline bool operator<= (const uuid& id) const { return code <= id.code; }
	inline bool operator! () const { return !code; }

	friend std::ostream& operator<< (std::ostream&, uuid&);
	friend std::istream& operator>> (std::istream&, uuid&);
};

inline std::ostream& operator<< (std::ostream& os, uuid& id) { return os << id.code; }
inline std::istream& operator>> (std::istream& is, uuid& id) { return is >> id.code; }

#define minmax_range(_min, _max, __val) std::min((_min), std::max((_max), (__val)))



template<class __Ty, size_t __Rows, size_t __Cols>
class Matrix2
{
private:
	using T = __Ty;

	T _data[__Rows * __Cols];

public:
	class MatrixRow
	{
	private:
		const size_t& _row;

	public:
		__forceinline MatrixRow(const size_t& row) : _row(row) {}
		__forceinline T& operator[](const size_t& column) { return _data[_row + column]; }
		__forceinline const T& operator[](const size_t& column) const { return _data[_row + column]; }
	};

public:
	constexpr size_t rows() { return __Rows; }
	constexpr size_t columns() { return __Cols; }
	constexpr size_t matrix_size() { return __Rows * __Cols; }

	__forceinline MatrixRow&& operator[](const size_t& row) { return { row / __Cols }; }
	__forceinline const MatrixRow& operator[](const size_t& row) const { return { row / __Cols }; }

	__forceinline T& operator() (const size_t& idx) { return _data[idx]; }
};

template<class __Ty>
class DynMatrix2
{
private:
	using T = __Ty;

	std::vector<T> _data;
	const size_t _rows;
	const size_t _cols;
	const size_t _size;

public: 
	class MatrixRow
	{
	private:
		const size_t& _row;

	public:
		__forceinline MatrixRow(const size_t& row) : _row(row) {}
		__forceinline T& operator[](const size_t& column) { return _data[_row + column]; }
		__forceinline const T& operator[](const size_t& column) const { return _data[_row + column]; }
	};

public:
	inline DynMatrix2(const size_t& rows, const size_t& columns) : _rows(rows), _cols(columns), _size(_rows * _cols), _data(_size) {}
	DynMatrix2() = delete;
	DynMatrix2(const DynMatrix2&) = delete;
	~DynMatrix2()
	{
		_data.clear();
	}

	DynMatrix2& operator= (const DynMatrix2&) = delete;

	inline size_t rows() { return _rows; }
	inline size_t columns() { return _cols; }
	inline size_t matrix_size() { return _size; }

	inline void fill(const T& value) { _data.assign(_size, value); }

	__forceinline MatrixRow&& operator[](const size_t& row) { return { row / __Cols }; }
	__forceinline const MatrixRow& operator[](const size_t& row) const { return { row / __Cols }; }

	__forceinline T& operator() (const size_t& idx) { return _data[idx]; }

};

__forceinline byte operator"" _b(unsigned long long int value) { return static_cast<byte>(value & 0xff); }



/* EXCEPTIONS */
class Throwable
{
protected:
	const std::string message;
	const Throwable* cause;

public:
	Throwable(const std::string msg = "", const Throwable* cause = nullptr);
	~Throwable();

	inline std::string getMessage() const { return message; }

	inline bool hasCause() { return cause != nullptr; }
	inline const Throwable* getCause() const { return cause; }
};

class BPSException : public Throwable
{
public:
	BPSException(const std::string msg = "", const Throwable* cause = nullptr);
};

class singleton
{
protected:
	singleton() {}
	~singleton() {}

private:
	singleton(const singleton&);
	singleton(singleton&&);

	singleton& operator= (const singleton&);
	singleton& operator= (singleton&&);
};

#define init_vector(vec, len, statement) (vec).reserve((len)); for(unsigned __i__=0;__i__<(len);__i__++) (vec).emplace_back((statement))
