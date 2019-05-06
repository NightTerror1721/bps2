#pragma once

#include <vector>
#include <functional>

#include "config.h"

namespace
{
	template<class _Ty>
	struct Allocator final
	{
		_Ty data;
		Allocator* next;
		Allocator* prev;

		template<class... _Args>
		Allocator(_Args&&... args) :
			data(args...),
			next(nullptr),
			prev(nullptr)
		{}

		Allocator(const Allocator&) = delete;
		Allocator(Allocator&&) = delete;

		Allocator& operator= (const Allocator&) = delete;
		Allocator& operator= (Allocator&&) = delete;
	};

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
			_size(0)
		{}
		~AllocatorList() { clear(); }

		template<class... _Args>
		Node* create(_Args&&... args)
		{
			if (!_head)
			{
				_head = new Node(args...);
				_tail = _head;
				_size++;
				return _head;
			}
			Node* node = new Node(args...);
			node->prev = _tail;
			_tail->next = node;
			_tail = node;
			_size++;
			return node;
		}

		void destroy(Node* const& allocator)
		{
			if (!_head)
				return;

			for (Node* current = _head; current; current = current->next)
			{
				if (current != allocator)
					continue;

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

					if (node == _head)
						_head = _head->next;
					if (node == _tail)
						_tail = _tail->prev;

					prev->next = next;
					next->prev = prev;
					_size--;

					delete current;
				}
				break;
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
	};

}

template<class _Ty>
class Ptr
{
private:
	using alloc_t = Allocator<_Ty>;

	alloc_t* _alloc;

public:
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

	template<class _Ty> friend class MemoryAllocator;

private:
	Ptr(alloc_t* const& ptr) : _alloc(ptr) {}
};


template<class _Ty>
class MemoryAllocator
{
	static_assert(std::is_base_of<UniqueObject, _Ty>::value, "_Ty must be derive from UniqueObject");

public:
	using ptr_t = Ptr<_Ty>;
	using alloc_t = Allocator<_Ty>;

private:
	AllocatorList<_Ty> _mem;
	std::unordered_map<UUID, alloc_t*, UUID_Hash> _map;

public:
	MemoryAllocator() : _mem(), _map() {}
	~MemoryAllocator() {}

	template<class... _Args>
	ptr_t create(_Args&&... args)
	{
		alloc_t* alloc = _mem.create(args...);
		_map[alloc->data.uuid()] = alloc;
		return { alloc };
	}

	ptr_t find(const UUID& id)
	{
		auto it = _map.find(id);
		if (it == _map.end())
			return nullptr;
		return { *it };
	}

	const ptr_t find(const UUID& id) const
	{
		auto it = _map.find(id);
		if (it == _map.cend())
			return nullptr;
		return { *it };
	}

	void destroy(const UUID& id)
	{
		auto it = _map.find(id);
		if (it == _map::end())
			return;

		alloc_t* alloc = *it;
		_map.erase(it);
		_mem.destroy(alloc);
	}

	std::vector<ptr_t> find(std::function<bool(const _Ty&)> criteria)
	{
		std::vector<Ptr<_Ty>> vec;
		for (Allocator<_Ty>* alloc = _mem._head; alloc; alloc = alloc->next)
			if (criteria(alloc->data))
				vec.push_back({ alloc });
		return std::move(vec);
	}

	inline void forEach(std::function<void(_Ty&)> action) { _mem.forEach(action); }
	inline void forEach(std::function<void(const _Ty&)> action) const { _mem.forEach(action); }

	inline void destroy(ptr_t ptr) { destroy(ptr->uuid()); }

	inline ptr_t operator[] (const UUID& id) { return find(id); }
	inline const ptr_t operator[] (const UUID& id) const { return find(id); }
};
