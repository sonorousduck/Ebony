#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <misc/resourceManager.hpp>
#include <components/subcolliderClass.hpp>
#include "../misc/collisionLayers.hpp"
#include <components/collider.hpp>
#include <components/rigidbodyComponent.hpp>
#include "../components/ballComponent.hpp"
#include "../components/brickComponent.hpp"
#include "../prefabs/powerupPrefab.hpp"
#include "../singletons/GameManager.hpp"
#include <Log.hpp>

namespace BrickBreaker
{
	class Brick
	{
	public:
		static entities::EntityPtr Create(float transformWidth, float transformHeight, float scaleX, float scaleY, const char* tile_image, int brickStrength, std::uint32_t pointValue, std::function<void(entities::Entity::IdType)> RemoveEntity, std::function<void(entities::EntityPtr)> AddEntity)
		{
			entities::EntityPtr brickEntity = std::make_shared<entities::Entity>();
			brickEntity->addComponent(std::make_unique<components::Transform>(glm::vec2(transformWidth, transformHeight), 0.0f, glm::vec2(scaleX, scaleY)));
			components::Subcollider subcollider = components::Subcollider(glm::vec2(scaleX / 2, scaleY / 2), glm::vec2(scaleX, scaleY), true, true);
			brickEntity->addComponent(std::make_unique<components::SoundEffect>(EbonyAudio::ENTITY));

			subcollider.onCollisionStart = [=](entities::EntityPtr self, entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{

					if (other->hasComponent<components::Ball>())
					{
						auto ball = other->getComponent<components::Ball>();
						auto brick = self->getComponent<components::Brick>();

						brick->strength -= ball->strength;

						if (brick->strength <= 0)
						{
							// The brick should explode
							std::cout << "BOOOM!" << std::endl;
							
							other->getComponent<components::SoundEffect>()->soundEffectQueue.push_back(Ebony::ResourceManager::GetSoundEffect("brick_break"));
							GameManager::addPoints(brick->pointValue);


							brick->destroyed = true;
							RemoveEntity(self->getId());


							// Spawn powerup entity
							AddEntity(Powerup::Create(glm::vec2(transformWidth, transformHeight), Powerups::LARGER_PADDLE, "bigger_paddle_powerup", RemoveEntity));
						}
					}
				};

			brickEntity->addComponent(std::make_unique<components::Collider>(subcollider, BrickBreaker::CollisionLayers::BRICK, true, false));
			brickEntity->addComponent(std::make_unique<components::RigidBody>());
			brickEntity->addComponent(std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture(tile_image), Ebony::Colors::White));
			brickEntity->addComponent(std::make_unique<components::Brick>(brickStrength, pointValue));

			return brickEntity;
		}

	};
}