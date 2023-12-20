#pragma once

#include <iostream>
#include "../Ebony/src/components/cppScriptComponent.hpp"
#include "../BrickBreaker/screens/screenEnums.hpp"
#include <components/transform.hpp>
#include <graphics2d.hpp>

namespace scripts
{
	struct CrosshairScript : components::CppScript
	{
	public:
		CrosshairScript(entities::EntityPtr player) : player(player)
		{
		}

		void MoveCrosshair(int screenPositionX, int screenPositionY)
		{
			// Keep a radius of 1 away from the player, but point towards the direction
			auto playerTransform = player->getComponent<components::Transform>();
			glm::vec3 cameraPosition = Ebony::Graphics2d::mainCamera->Position;
			glm::vec2 currentDirection = glm::normalize((glm::vec2(screenPositionX, screenPositionY) + glm::vec2(cameraPosition.x, cameraPosition.y)) - playerTransform->position) * glm::vec2(100, 100);

			aimLastDirection = currentDirection;
			//entity->getComponent<components::Transform>()->position = aimLastDirection + playerTransform->position;
		}

		void MoveCrosshairController(float x, float y)
		{
			if (x != 0.0f && y != 0.0f)
			{
				glm::vec3 cameraPosition = Ebony::Graphics2d::mainCamera->Position;
				glm::vec2 currentDirection = glm::normalize(glm::vec2(x, y)) * glm::vec2(100, 100);

				aimLastDirection = currentDirection;
			}

		}

		void Update(std::chrono::microseconds elapsedTime) override
		{
			components::MouseInput* mouseInput;
			components::ControllerInput* controllerInput;

			if (entity->tryGetComponent(mouseInput))
			{
				// Need to check so if the player is using controller it doesn't keep setting back
				if (mouseInput->positionX != mouseInput->previousX || mouseInput->positionY != mouseInput->previousY)
				{
					MoveCrosshair(mouseInput->positionX, mouseInput->positionY);
				}

			}

			if (entity->tryGetComponent(controllerInput))
			{
				if (controllerInput->rightJoystickXPosition != controllerInput->rightLastJoystickXPosition
					|| controllerInput->rightJoystickYPosition != controllerInput->rightLastJoystickYPosition
					&& (controllerInput->rightJoystickXPosition != 0.0f && controllerInput->rightJoystickYPosition != 0.0f))
				{
					MoveCrosshairController(controllerInput->rightJoystickXPosition, controllerInput->rightJoystickYPosition);
				}
			}
			auto playerTransform = player->getComponent<components::Transform>();
			entity->getComponent<components::Transform>()->position = aimLastDirection + playerTransform->position;
		}

		entities::EntityPtr player;
		glm::vec2 aimLastDirection{};

	};
}