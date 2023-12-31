#include "ballSystem.hpp"
#include <singletons/time.hpp>

void systems::BallSystem::Update(std::chrono::microseconds elapsedTime)
{
	this->updateImpl(elapsedTime);
}

void systems::BallSystem::updateImpl(std::chrono::microseconds elapsedTime)
{
	for (auto& [id, entity] : m_Entities)
	{
		// Update movement
		auto rigidBody = entity->getComponent<components::RigidBody>();
		auto transform = entity->getComponent<components::Transform>();
		auto ball = entity->getComponent<components::Ball>();


		if (!ball->isAttachedToPaddle)
		{
			// Add forces for the physics engine to move the ball with
			float speed = ball->speed * Ebony::Time::GetDeltaTimeFloat();
			glm::vec2 direction = ball->direction;
			rigidBody->addScriptedMovement(direction * speed);
			ball->directionChangedThisFrame = false;
		}
		else
		{
			auto paddleTransform = ball->paddle->getComponent<components::Transform>();
			transform->position.x = paddleTransform->position.x + (paddleTransform->scale.x / 2);
		}



	}


}
