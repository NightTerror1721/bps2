#pragma once

#include "mem_alloc.h"
#include <map>

template<class _Ty>
class Manager
{
public:
	using elem_t = Ptr<_Ty>;

private:
	MemoryAllocator<_Ty> _alloc;
	std::map<std::string, elem_t> _map;
	Manager* const _parent;

public:
	explicit Manager(Manager* const& parent = nullptr) :
		_alloc(),
		_map(),
		_parent(parent)
	{}
	~Manager() {}

	bool has(const std::string& name) const
	{
		if (_map.find(name) != _map.end())
			return true;
		return _parent && _parent->has(name);
	}

	elem_t get(const std::string& name)
	{
		auto it = _map.find(name);
		if (it == _map.end())
		{
			if (!_parent)
				return nullptr;
			return _parent->get(name);
		}
		return *it;
	}

	const elem_t get(const std::string& name) const
	{
		auto it = _map.find(name);
		if (it == _map.end())
		{
			if (!_parent)
				return nullptr;
			return _parent->get(name);
		}
		return *it;
	}

	void destroy(const std::string& name)
	{
		auto it = _map.find(name);
		if (it == _map.end())
			return;

		elem_t e = *it;
		_map.erase(it);
		_alloc.destroy(e);
	}

	void clear()
	{
		_map.clear();
		_alloc.clear();
	}

	inline elem_t operator[] (const std::string& name) { return get(name); }
	inline const elem_t operator[] (const std::string& name) const { return get(name); }

protected:
	template<class... _Args>
	elem_t create(const std::string& name, _Args... args)
	{
		if (has(name))
			return nullptr;

		elem_t e = _alloc.create(args...);
		_map[name] = e;
		return e;
	}
};
