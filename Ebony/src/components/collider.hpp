#pragma once

#include <vector>

#include "component.hpp"
#include "subcolliderClass.hpp"

namespace components
{
	class Collider : public PolymorphicComparable<Component, Collider>
	{
	public:
		Collider(Subcollider aabbCollider, std::uint16_t layer) : aabbCollider(aabbCollider), layer(layer) {};

		Collider(Subcollider aabbCollider, std::vector<Subcollider> subcolliders, std::uint16_t layer) : aabbCollider(aabbCollider), subcolliders(subcolliders), preciseSubcolliderDetection(true), layer(layer) {}


		Subcollider aabbCollider;
		std::vector<Subcollider> subcolliders{};

		bool preciseSubcolliderDetection = false;
		bool isEnabled = true;
		bool isCollidingLastFrame = false;

		// Need to figure out how to do the bitwise comparisons
		std::uint16_t layer;
		
		

	private:



	};
}
