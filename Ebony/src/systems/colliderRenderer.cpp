#include "colliderRenderer.hpp"

namespace systems
{
	void ColliderRenderer::Update(std::shared_ptr<Ebony::Graphics2d> graphics)
	{
		for (auto&& [id, entity] : m_Entities)
		{
			auto sprite = entity->getComponent<components::ColliderSprite>();
			auto transform = entity->getComponent<components::Transform>();


			graphics->Draw(sprite->shader, sprite->texture, transform->position, transform->scale, transform->rotation, sprite->spriteColor, sprite->depth);
		}
	}
}

