#include <iostream>

#include "mem_alloc.h"


int main(int argc, char** argv)
{

	MemoryAllocator<UniqueObject> mem;

	auto ptr = mem.create();

	mem.find([&ptr](const UniqueObject& obj) -> bool { return ptr->uuid() == obj.uuid(); });

	std::cout << ptr->uuid() << std::endl;

}
