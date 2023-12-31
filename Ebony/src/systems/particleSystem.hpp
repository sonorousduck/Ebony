#pragma once
#include "system.hpp"
#include <glm/glm.hpp>
#include "../components/particle.hpp"
#include "../components/transform.hpp"

namespace systems
{
	class ParticleSystem : public System
	{
	public:
		ParticleSystem() : System({ ctti::unnamed_type_id<components::ParticleGroup>(), ctti::unnamed_type_id<components::Transform>() })
		{
		}

		void Update(std::chrono::microseconds elapsedTime) override;

	private:
		void updateImpl(std::chrono::microseconds elapsedTime);
		void Preallocate(components::ParticleGroup* particleGroup);
		int firstUnusedParticle(components::ParticleGroup* particleGroup);
		void respawnParticle(Particle& particle, float lowerBound, float upperBound, components::ParticleGroup* particleGroup, components::Transform* transform);
	};


}