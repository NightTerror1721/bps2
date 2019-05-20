#pragma once

#include <vector>
#include <functional>
#include <exception>

#include "config.h"

namespace
{
	template<class _Ty>
	struct AllocatorList;

	template<class _Ty>
	struct Allocator final
	{
		AllocatorList<_Ty>* const _list;
		_Ty data;
		Allocator* next;
		Allocator* prev;

		template<class... _Args>
		Allocator(AllocatorList<_Ty>* const& list, _Args&&... args) :
			_list(list),
			data(args...),
			next(nullptr),
			prev(nullptr)
		{}

		Allocator(const Allocator&) = delete;
		Allocator(Allocator&&) = delete;

		Allocator& operator= (const Allocator&) = delete;
		Allocator& operator= (Allocator&&) = delete;
	};
};

#define ALLOCATOR_FRIEND template<class _Ty> friend struct Allocator


template<class _Ty>
class AllocatorIterator
{
private:
	using alloc_t = Allocator<_Ty>*;

	alloc_t& _alloc;
	bool _end;

public:
	bool operator== (const AllocatorIterator& it) const { return _end == it._end && _alloc == it._alloc; }
	bool operator!= (const AllocatorIterator& it) const { return _end != it._end && _alloc != it._alloc; }

	bool operator! () const { return _end; }
	operator bool() { return !_end; }

	AllocatorIterator& operator++ ()
	{
		if (_end)
			throw no_such_element();

		if (!_alloc->next)
			_end = true;
		else _alloc = _alloc->next;

		return *this;
	}
	AllocatorIterator operator++ (int)
	{
		AllocatorIterator old{ _alloc, _end };
		operator++();
		return old;
	}

	AllocatorIterator& operator-- ()
	{
		if (_alloc->prev)
		{
			_alloc = _alloc->prev;
			_end = false;
		}
		else throw no_such_element();

		return *this;
	}
	AllocatorIterator operator-- (int)
	{
		AllocatorIterator old{ _alloc, _end };
		operator--();
		return old;
	}

	_Ty& operator* ()
	{
		return _alloc->data;
	}
	const _Ty& operator* () const
	{
		return _alloc->data;
	}

	_Ty* operator->()
	{
		return &_alloc->data;
	}
	const _Ty* operator->() const
	{
		return &_alloc->data;
	}

	template<class _Ty>
	friend struct AllocatorList;


private:
	AllocatorIterator(alloc_t& alloc, bool end) : _alloc(alloc), _end(end) {}

public:
	class no_such_element : public std::exception {};
};


namespace
{
	template<class _Ty>
	struct AllocatorList
	{
		using Node = Allocator<_Ty>;
		Node* _head;
		Node* _tail;
		size_t _size;

		AllocatorList() :
			_head(nullptr),
			_tail(nullptr),
			_size(0),
			_begin(_head, false),
			_end(_tail, true)
		{}
		~AllocatorList() { clear(); }

		template<class... _Args>
		Node* create(_Args&&... args)
		{
			if (!_head)
			{
				_head = new Node(this, args...);
				_tail = _head;
				_size++;
				return _head;
			}
			Node* node = new Node(this, args...);
			node->prev = _tail;
			_tail->next = node;
			_tail = node;
			_size++;
			return node;
		}

		void destroy(Node* const& allocator)
		{
			if (!_head || allocator->_list != this)
				return;

			Node* current = allocator;

			if (_head == _tail)
			{
				delete _head;
				_head = nullptr;
				_tail = nullptr;
				_size = 0;
			}
			else
			{
				Node* prev = current->prev;
				Node* next = current->next;

				if (current == _head)
					_head = _head->next;
				if (current == _tail)
					_tail = _tail->prev;

				if(prev)
					prev->next = next;
				if(next)
					next->prev = prev;
				_size--;

				delete current;
			}
		}

		void clear()
		{
			Node* current = _head;
			while (current)
			{
				Node* next = current->next;
				delete current;
				current = next;
			}
			_head = nullptr;
			_tail = nullptr;
			_size = 0;
		}

		void clear(std::function<bool(_Ty&)> criteria)
		{
			if (!_head)
				return;

			if (_head == _tail)
			{
				if (criteria(_head->data))
				{
					delete _head;
					_head = nullptr;
					_tail = nullptr;
					_size = 0;
				}
			}
			else
			{
				Node* current = _head;
				while (current)
				{
					if (criteria(current->data))
					{
						Node* prev = current->prev;
						Node* next = current->next;

						if (current == _head)
							_head = _head->next;
						if (current == _tail)
							_tail = _tail->prev;

						prev->next = next;
						next->prev = prev;
						_size--;

						delete current;
						current = next;
					}
					else current = current->next;
				}

				if (!_size)
				{
					_head = nullptr;
					_tail = nullptr;
				}
			}
		}

		void forEach(std::function<void(_Ty&)> action)
		{
			for (Node* current = _head; current; current = current->next)
				action(current->data);
		}

		void forEach(std::function<void(const _Ty&)> action) const
		{
			for (Node* current = _head; current; current = current->next)
				action(current->data);
		}


		/* Iterable part */
	private:
		AllocatorIterator<_Ty> _begin;
		AllocatorIterator<_Ty> _end;

	public:
		AllocatorIterator<_Ty> begin() { return _begin; }
		const AllocatorIterator<_Ty> begin() const { return _begin; }
		AllocatorIterator<_Ty> end() { return _end; }
		const AllocatorIterator<_Ty> end() const { return _end; }
	};

}

template<class _Ty>
class Ptr
{
private:
	using alloc_t = Allocator<_Ty>;

	alloc_t* _alloc;

public:
	Ptr() : _alloc(nullptr) {}
	Ptr(std::nullptr_t) : _alloc(nullptr) {}
	Ptr& operator= (std::nullptr_t) { _alloc = nullptr; return *this; }

	bool operator== (const Ptr& ptr) const { return _alloc == ptr._alloc; }
	bool operator!= (const Ptr& ptr) const { return _alloc != ptr._alloc; }
	bool operator> (const Ptr& ptr) const { return _alloc > ptr._alloc; }
	bool operator< (const Ptr& ptr) const { return _alloc < ptr._alloc; }
	bool operator>= (const Ptr& ptr) const { return _alloc >= ptr._alloc; }
	bool operator<= (const Ptr& ptr) const { return _alloc <= ptr._alloc; }

	bool operator== (std::nullptr_t) const { return !_alloc; }
	bool operator!= (std::nullptr_t) const { return _alloc; }

	bool operator! () const { return !_alloc; }
	operator bool() const { return _alloc; }

	inline _Ty& operator* () { return _alloc->data; }
	inline const _Ty& operator* () const { return _alloc->data; }

	inline _Ty* operator-> () { return &_alloc->data; }
	inline const _Ty* operator-> () const { return &_alloc->data; }

	inline _Ty* operator& () { return &_alloc->data; }
	inline const _Ty* operator& () const { return &_alloc->data; }

	static inline void free(Ptr& ptr)
	{
		if (ptr._alloc)
		{
			ptr._alloc->_list->destroy(ptr._alloc);
			ptr._alloc = nullptr;
		}
	}

	template<class _Ty> friend class MemoryAllocator;

private:
	Ptr(alloc_t* const& ptr) : _alloc(ptr) {}
};


template<class _Ty>
class MemoryAllocator
{
public:
	using ptr_t = Ptr<_Ty>;
	using alloc_t = Allocator<_Ty>;
	using iterator = AllocatorIterator<_Ty>;
	using const_iterator = const AllocatorIterator<_Ty>;

private:
	AllocatorList<_Ty> _mem;

public:
	MemoryAllocator() : _mem() {}
	~MemoryAllocator() {}

	template<class... _Args>
	ptr_t malloc(_Args&&... args)
	{
		return { _mem.create(args...) };
	}

	inline void free(ptr_t ptr)
	{
		_mem.destroy(ptr._alloc);
	}

	inline void clear()
	{
		_mem.clear();
	}

	inline void clear(std::function<bool(_Ty&)> criteria)
	{
		_mem.clear(criteria);
	}

	std::vector<ptr_t> find(std::function<bool(const _Ty&)> criteria)
	{
		std::vector<Ptr<_Ty>> vec;
		for (alloc_t* alloc = _mem._head; alloc; alloc = alloc->next)
			if (criteria(alloc->data))
				vec.push_back({ alloc });
		return std::move(vec);
	}

	inline void forEach(std::function<void(_Ty&)> action) { _mem.forEach(action); }
	inline void forEach(std::function<void(const _Ty&)> action) const { _mem.forEach(action); }

	inline ptr_t operator[] (const UniqueID& id) { return find(id); }
	inline const ptr_t operator[] (const UniqueID& id) const { return find(id); }

	iterator begin() { return _mem.begin(); }
	const_iterator begin() const { return _mem.begin(); }
	const_iterator cbegin() const { return _mem.begin(); }
	iterator end() { return _mem.end(); }
	const_iterator end() const { return _mem.end(); }
	const_iterator cend() const { return _mem.end(); }
};

template<class _PtrTy = void>
class CustomAllocator
{
private:
	_PtrTy* _alloc;

public:
	~CustomAllocator() { delete _alloc; }

	template<class _Ty>
	inline CustomAllocator& operator= (const _Ty& value) { *reinterpret_cast<_Ty*>(_alloc) = value; return *this; }

	template<class _Ty>
	inline CustomAllocator& operator= (_Ty&& value) { *reinterpret_cast<_Ty*>(_alloc) = value; return *this; }

	template<class _Ty>
	inline operator _Ty() { *reinterpret_cast<_Ty*>(_alloc); }

	template<class _Ty, class _PtrTy = void, class... _Args>
	friend CustomAllocator<_PtrTy> make_alloc(_Args&&... args);

private:
	CustomAllocator(_PtrTy* const& _Ptr) :
		_alloc{ _Ptr }
	{}
}; 

template<class _Ty, class _PtrTy = void, class... _Args>
CustomAllocator<_PtrTy> make_alloc(_Args&&... args)
{
	return { reinterpret_cast<_PtrTy*>(new _Ty{ args... }) };
}

using UnknownAllocator = CustomAllocator<>;
