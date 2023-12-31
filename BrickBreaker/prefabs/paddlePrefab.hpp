#pragma once
#include <components/rigidbodyComponent.hpp>
#include <components/keyboardComponent.hpp>
#include <components/controllerComponent.hpp>
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
#include <singletons/time.hpp>
#include <components/soundEffectComponent.hpp>
#include <components/mouseInputComponent.hpp>

namespace BrickBreaker
{
	class Paddle
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, int windowWidth)
		{
			entities::EntityPtr paddle = std::make_shared<entities::Entity>();


			std::unique_ptr<components::ControllerInput> controllerInputComponent = std::make_unique<components::ControllerInput>(0);
			std::unique_ptr<components::KeyboardInput> keyboardInputComponent = std::make_unique<components::KeyboardInput>();

			controllerInputComponent->bindings.insert({ SDL_CONTROLLER_BUTTON_B, "print" });
			controllerInputComponent->bindings.insert({ SDL_CONTROLLER_BUTTON_Y, "printRelease" });


			controllerInputComponent->onHeldActions.insert({ "print", [=]() {std::cout << "Circle was called (OnHeld)" << std::endl; } });
			controllerInputComponent->onReleaseActions.insert({ "printRelease", [=]() {std::cout << "Triangle was called" << std::endl; } });
			controllerInputComponent->joystickBindings.insert({ SDL_CONTROLLER_AXIS_LEFTX, "paddleMovement" });

			controllerInputComponent->joystickActions.insert({ "paddleMovement", [=](float value) {
				if (abs(value) > 0.10)
				{
					auto rigidBody = paddle->getComponent<components::RigidBody>();
					auto transform = paddle->getComponent<components::Transform>();
					auto collider = paddle->getComponent<components::Collider>();

					if (value < 0 && transform->position.x > 0)
					{
						std::cout << Ebony::Time::GetDeltaTimeFloat() << std::endl;
						rigidBody->addScriptedMovement(glm::vec2{ 700.0f * Ebony::Time::GetDeltaTimeFloat() * value, 0.0f });
					}
					else if (value > 0 && transform->position.x + transform->scale.x < windowWidth)
					{
						rigidBody->addScriptedMovement(glm::vec2{ 700.0f * Ebony::Time::GetDeltaTimeFloat() * value, 0.0f });
					}
				}
			} });
		
			keyboardInputComponent->onReleaseActions.insert({ "print", [=]() { std::cout << "E was called" << std::endl; } });
			keyboardInputComponent->onHeldActions.insert({ "print", [=]() { std::cout << "E was called" << std::endl; } });
			keyboardInputComponent->bindings.insert({ SDLK_a, "paddleLeft" });
			keyboardInputComponent->bindings.insert({ SDLK_d, "paddleRight" });
			keyboardInputComponent->bindings.insert({ SDLK_e, "growShrink" });

			keyboardInputComponent->onHeldActions.insert({ "paddleLeft", [=]()
			{
				auto rigidBody = paddle->getComponent<components::RigidBody>();
				auto transform = paddle->getComponent<components::Transform>();
				auto collider = paddle->getComponent<components::Collider>();


				if (transform->position.x > 0)
				{
					rigidBody->addScriptedMovement(glm::vec2{ -700.0f * Ebony::Time::GetDeltaTimeFloat(), 0.0f });
				}
			} });

			keyboardInputComponent->onHeldActions.insert({ "paddleRight", [=]()
			{
				auto rigidBody = paddle->getComponent<components::RigidBody>();
				auto transform = paddle->getComponent<components::Transform>();
				auto collider = paddle->getComponent<components::Collider>();
				if (transform->position.x + transform->scale.x < windowWidth)
				{
					rigidBody->addScriptedMovement(glm::vec2{ 700.0f * Ebony::Time::GetDeltaTimeFloat(), 0.0f });
				}
			}
				});


			keyboardInputComponent->onPressActions.insert({ "growShrink", [=]()
			{
				auto transform = paddle->getComponent<components::Transform>();

				if (transform->scale.x == 150.0f)
				{
					transform->scale.x += 50.0f;
				}
				else
				{
					transform->scale.x = 150.f;
				}
			} });



			auto mouseComponent = std::make_unique<components::MouseInput>();

			mouseComponent->bindings.insert({SDL_BUTTON_LEFT, "mousePress" });
			mouseComponent->onPressActions.insert({ "mousePress", [=](Ebony::MousePress& mousePress) {std::cout << "Button pressed at " << mousePress.mouseClick.x << ", " << mousePress.mouseClick.y << std::endl; }});
			mouseComponent->onReleaseActions.insert({ "mousePress",[=](Ebony::MousePress&) {std::cout << "Mouse Button released!" << std::endl; } });

			//mouseComponent->loadMouseBindings("../mouseBindings.json");
			//mouseComponent->saveMouseBindings("../mouseBindings.json");
			paddle->addComponent(std::move(mouseComponent));

			paddle->addComponent(std::make_unique<components::SoundEffect>(Ebony::ENTITY));


			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("paddle_0"), Ebony::Colors::White);
			components::Subcollider aabbcollider = components::Subcollider(glm::vec2(75.0f, 25.0f), glm::vec2(150.0f, 50.0f), true, true);
			auto collider = std::make_unique<components::Collider>(aabbcollider, BrickBreaker::CollisionLayers::PADDLE | BrickBreaker::CollisionLayers::POWERUP, false);
			auto transform = std::make_unique<components::Transform>(startTransform, 0.0f, glm::vec2(150.0f, 50.0f));
			auto rigidbody = std::make_unique<components::RigidBody>();


			paddle->addComponent(std::move(collider));

			paddle->addComponent(std::move(transform));
			paddle->addComponent(std::move(sprite));
			paddle->addComponent(std::move(keyboardInputComponent));
			paddle->addComponent(std::move(controllerInputComponent));
			paddle->addComponent(std::move(rigidbody));
		
			return paddle;
		}

	};
}