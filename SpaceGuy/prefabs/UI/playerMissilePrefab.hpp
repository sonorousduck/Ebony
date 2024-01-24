#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <misc/resourceManager.hpp>
#include <misc/renderLayers.hpp>
#include <singletons/systemManager.hpp>
#include "../../scripts/playerMissileUpdateScript.hpp"
#include "playerMissileTextPrefab.hpp"

namespace SpaceGuy
{
	class PlayerMissilePrefab
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startPosition, entities::EntityPtr player)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("missile_ui"), Ebony::Colors::White, Ebony::RenderLayer::UI_RENDER, true);

			entity->addComponent(std::make_unique<components::Transform>(startPosition, 0.0f, sprite->GetDimensions()));
			entity->addComponent(std::move(sprite));

			Ebony::SystemManager::AddEntity(PlayerMissileCount::Create(startPosition, player));
			return entity;
		}
	};
}