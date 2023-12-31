/*
Copyright (c) 2022 James Dean Mathias

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

Thanks for Dean Mathias for base implementation of Entity base class
*/

#include "entity.hpp"

#include <memory>

namespace entities
{
	std::atomic_uint32_t Entity::nextId = 0;

	

	//EntityPtr Entity::clone()
	//{
	//	auto clone = std::make_shared<Entity>();

	//	// This needs the exact same id. Don't mix this with the live entities or will cause
	//	// game breaking bugs that make no sense
	//	clone->m_Id = m_Id;

	//	for (auto&& [id, component] : m_Components)
	//	{
	//		auto&& [typeId, ptr] = component->clone();
	//		clone->m_Components[typeId] = std::move(ptr);
	//	}

	//	return clone;
	//}

	bool Entity::operator==(const Entity& rhs)
	{
		// First test if the number of components are different. If they are, they aren't equal (fastest check)
		bool areEqual{ this->m_Components.size() == rhs.m_Components.size() };

		if (areEqual)
		{
			for (auto&& [id, lhsComponent] : m_Components)
			{
				if (rhs.m_Components.contains(id))
				{
					areEqual = areEqual && *(lhsComponent.get()) == *(rhs.m_Components.at(id).get());

					// Only keep going until it isn't equal. You can stop checking there
					if (!areEqual)
					{
						break;
					}
				}
				else
				{
					// Only keep going until it isn't equal. You can stop checking there
					areEqual = false;
					break;
				}
			}
		}

		return areEqual;
	}

	bool Entity::operator!=(const Entity& rhs)
	{
		return !(*this == rhs);
	}

}