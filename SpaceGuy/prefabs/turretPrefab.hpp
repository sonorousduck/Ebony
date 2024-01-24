#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <misc/resourceManager.hpp>
#include <components/subcolliderClass.hpp>
#include "../misc/collisionLayers.hpp"
#include <components/collider.hpp>
#include <components/rigidbodyComponent.hpp>
#include <singletons/time.hpp>
#include "../components/enemyDetectionComponent.hpp"
#include <components/cppScriptComponent.hpp>
#include "../components/enemyInformation.hpp"
#include "healthBarPrefab.hpp"
#include <singletons/systemManager.hpp>
#include "../prefabs/explosionDeathPrefab.hpp"
#include <misc/renderLayers.hpp>
#include <components/aiComponent.hpp>
#include "../misc/aiInformationTypes.hpp"
#include "../components/playerInformation.hpp"
#include "../components/bulletComponent.hpp"
#include "../scripts/enemyMovementShootingScript.hpp"

namespace SpaceGuy
{
	class Turret
	{
	public:
		// Give a score value so spawned enemies aren't just worth farming forever
		static entities::EntityPtr Create(glm::vec2 startTransform, glm::vec2 scale, entities::EntityPtr player, float scoreValue = 130.0f)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();
			bool isShooting = true;
			float cooldownTimer = 10.0f;

			float detectionRange = 250.0f;
			float movementRange = 0.0f;
			float movementSpeed = 0.0f;
			glm::vec2 offset = { 0.0f, 0.0f };

			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("turret"), Ebony::Colors::White, Ebony::RenderLayer::FOREGROUND);
			scale *= glm::vec2(sprite->texture->Width, sprite->texture->Height);


			components::Subcollider aabbcollider = components::Subcollider(scale / glm::vec2(2.0f, 2.0f), scale, true, true);

			aabbcollider.onCollisionStart = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					auto otherLayer = other->getComponent<components::Collider>()->layer;

					if (otherLayer & SpaceGuy::CollisionLayers::PLAYER_BULLET)
					{
						auto enemyInformation = entity->getComponent<components::EnemyInformation>();

						enemyInformation->health -= other->getComponent<components::Bullet>()->strength;

						if (enemyInformation->health <= 0.0f)
						{
							player->getComponent<components::PlayerInformation>()->score += scoreValue;
							entity->getComponent<components::DestructionComponent>()->shouldDestroy = true;
						}

						//other->getComponent<components::DestructionComponent>()->shouldDestroy = true;
					}
					if (otherLayer & SpaceGuy::CollisionLayers::ENEMY)
					{
						entity->getComponent<components::Transform>()->position = entity->getComponent<components::Transform>()->previousPosition;
					}
				};

			aabbcollider.onCollision = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					auto otherLayer = other->getComponent<components::Collider>()->layer;

					if (otherLayer & SpaceGuy::CollisionLayers::ENEMY)
					{
						entity->getComponent<components::Transform>()->position = entity->getComponent<components::Transform>()->previousPosition;
					}
				};

			auto collider = std::make_unique<components::Collider>(aabbcollider, SpaceGuy::CollisionLayers::SPAWNER, SpaceGuy::CollisionLayers::PLAYER_BULLET | SpaceGuy::CollisionLayers::ENEMY, false);
			auto transform = std::make_unique<components::Transform>(startTransform, 0.0f, scale);
			auto rigidbody = std::make_unique<components::RigidBody>();

			std::unique_ptr<components::CppScript> script = std::make_unique<scripts::EnemyMovementShootingScript>();


			auto timedComponent = std::make_unique<components::TimedComponent>(0.1f, [=]() {});


			entity->addComponent(std::make_unique<components::EnemyInformation>(15.0f));
			entity->addComponent(std::make_unique<components::EnemyDetection>(detectionRange, movementRange, movementSpeed, offset, 400.0f, "enemy_bullet", player, 5.0f));

			entity->addComponent(std::make_unique<components::DestructionComponent>([=]()
				{
					// Eventually, this is where the animations will be inserted and played probably (Maybe just spawn an entity or something)
					Ebony::SystemManager::AddEntity(Crypt::ExplosionDeathPrefab::Create(entity->getComponent<components::Transform>()->position, glm::vec2(1.0f, 1.0f)));
					entity->getComponent<components::DestructionComponent>()->shouldRemove = true;
				}));

			entity->addComponent(std::move(script));
			entity->addComponent(std::move(timedComponent));
			entity->addComponent(std::move(collider));
			entity->addComponent(std::move(transform));
			entity->addComponent(std::move(sprite));
			entity->addComponent(std::move(rigidbody));
			//entity->addComponent(std::make_unique<components::AIComponent>(Ebony::AIType::STATE, Crypt::AiInformationTypes::ENEMY_INFORMATION));


			entities::EntityPtr healthBar = SpaceGuy::HealthBar::Create(startTransform + glm::vec2(0.0f, -20.0f), glm::vec2(scale.x, 7.0f), entity);
			Ebony::SystemManager::AddEntity(healthBar);

			return entity;
		}

	};
}