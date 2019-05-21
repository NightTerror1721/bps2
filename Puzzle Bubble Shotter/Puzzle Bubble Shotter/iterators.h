#pragma once

#include <exception>

class NoSuchElementException : public std::exception {};

template<class _Ty, template<class _Ty2 = _Ty> class Iter>
class AbstractForwardIterator
{
protected:
	virtual bool _end() const = 0;
	virtual void _next() = 0;
	virtual bool _hasNext() const = 0;
	virtual _Ty& _get() = 0;
	virtual const _Ty& _get() const = 0;

public:
	inline AbstractForwardIterator& operator++ ()
	{
		if (_end() || !_hasNext())
			throw NoSuchElementException();
		_next();
		return *this;
	}

	inline Iter<_Ty> operator++ (int)
	{
		Iter<_Ty> old{ *this };
		operator++();
		return old;
	}

	inline _Ty& operator* () { return _get(); }
	inline const _Ty& operator* () const { return _get(); }

	inline _Ty* operator-> () { return &_get(); }
	inline const _Ty* operator-> () const { return &_get(); }
};

template<class _Ty>
class ArrayForwardIterator : public AbstractForwardIterator<_Ty, ArrayForwardIterator>
{
private:
	_Ty* _array;
	size_t _size;
	size_t _idx;

public:
	ArrayForwardIterator(_Ty* const& array, const size_t& arraySize, const size_t& startIndex = 0) :
		_array(array),
		_size(arraySize),
		_idx(startIndex)
	{}

	bool _end() const override { return _idx >= _size; }
	void _next() override
	{
		if (_idx + 1 >= _size)
			throw NoSuchElementException();
		++_idx;
	}
	bool _hasNext() const override { return _idx + 1 >= _size; }
	_Ty& _get() override
	{
		if (_idx >= _size)
			throw NoSuchElementException();
		return _array[_idx];
	}
	const _Ty& _get() const override
	{
		if (_idx >= _size)
			throw NoSuchElementException();
		return _array[_idx];
	}

	bool operator== (const ArrayForwardIterator& other) const { return _end() ? other._end() : _idx == other._idx; }
	bool operator!= (const ArrayForwardIterator& other) const { return _end() ? !other._end() : _idx != other._idx; }
};




