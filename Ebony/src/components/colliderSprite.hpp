#pragma once

#include "component.hpp"

#include "colors.hpp"
#include <texture.hpp>
#include <shader.hpp>

#include <optional>
#include <memory>

namespace components
{
	class ColliderSprite : public PolymorphicComparable<Component, ColliderSprite>
	{
	public:
		ColliderSprite(std::shared_ptr<Shader> shader, std::shared_ptr<Texture2D> texture, Ebony::Color spriteColor, float depth = 0.0f) : texture(texture), spriteColor(spriteColor), depth(depth), shader(shader)
		{}


		std::shared_ptr<Texture2D> texture;
		Ebony::Color spriteColor;
		float depth;
		std::shared_ptr<Shader> shader;
	};
}