#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <misc/resourceManager.hpp>
#include <components/subcolliderClass.hpp>
#include "../misc/collisionLayers.hpp"
#include <components/collider.hpp>
#include <graphics2d.hpp>
#include <SDL.h>
#include <Log.hpp>
#include <components/controllerComponent.hpp>
#include <components/keyboardComponent.hpp>
#include <components/rigidbodyComponent.hpp>
#include <singletons/time.hpp>
#include <components/mouseInputComponent.hpp>
#include <components/animationControllerComponent.hpp>
#include "../components/shootingComponent.hpp"
#include "../components/enemyDetectionComponent.hpp"
#include <components/cppScriptComponent.hpp>
#include "../scripts/shootingBatScript.hpp"
#include "../components/enemyComponent.hpp"

namespace Crypt
{
	class Bat
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, glm::vec2 scale, entities::EntityPtr player, std::function<void(entities::EntityPtr entity)> AddEntity)
		{
			entities::EntityPtr bat = std::make_shared<entities::Entity>();

			float detectionRange = 300.0f;
			float movementRange = 500.0f;
			float movementSpeed = 300.0f;
			glm::vec2 offset = { 200.0f, 50.0f };

			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("bat"), Ebony::Colors::White, 0.11f);
			components::Subcollider aabbcollider = components::Subcollider(scale / glm::vec2(2.0f, 2.0f), scale, true, true);

			aabbcollider.onCollisionStart = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					if (other->getComponent<components::Collider>()->layer & Crypt::CollisionLayers::PLAYER_BULLET)
					{
						auto enemyInformation = bat->getComponent<components::Enemy>();

						enemyInformation->health -= other->getComponent<components::Bullet>()->strength;

						if (enemyInformation->health <= 0.0f)
						{
							bat->getComponent<components::DestructionComponent>()->shouldDestroy = true;
						}

						other->getComponent<components::DestructionComponent>()->shouldDestroy = true;
					}
				};

			auto collider = std::make_unique<components::Collider>(aabbcollider, Crypt::CollisionLayers::ENEMY | Crypt::CollisionLayers::PLAYER_BULLET, false);
			auto transform = std::make_unique<components::Transform>(startTransform, 0.0f, scale);
			auto rigidbody = std::make_unique<components::RigidBody>();

			std::unique_ptr<components::CppScript> script = std::make_unique<scripts::ShootingBatScript>(AddEntity);


			auto shootingComponent = std::make_unique<components::Shooting>();


			bat->addComponent(std::make_unique<components::Enemy>(3.0f));
			bat->addComponent(std::make_unique<components::EnemyDetection>(detectionRange, movementRange, movementSpeed, offset, 400.0f, "bat_attack", player));

			bat->addComponent(std::make_unique<components::DestructionComponent>([=]()
				{
					// Eventually, this is where the animations will be inserted and played probably (Maybe just spawn an entity or something)
					bat->getComponent<components::DestructionComponent>()->shouldRemove = true;
				}));

			bat->addComponent(std::move(script));
			bat->addComponent(std::move(shootingComponent));
			bat->addComponent(std::move(collider));
			bat->addComponent(std::move(transform));
			bat->addComponent(std::move(sprite));
			bat->addComponent(std::move(rigidbody));

			return bat;
		}

	};
}