#include "input.hpp"
#include <any>

namespace Ebony
{
	Input::Input()
	{
	}

	void Input::onCursorPos(double xPosIn, double yPosIn)
	{
		float x = static_cast<float>(xPosIn);
		float y = static_cast<float>(yPosIn);

		if (firstMouse)
		{
			lastMosX = x;
			lastMosY = y;
			firstMouse = false;
		}


		float xOffset = x - lastMosX;
		float yOffset = lastMosY - y;
		lastMosX = x;
		lastMosY = y;
	}

	void Input::onMouseButton(int button, int action, int mods)
	{
		if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
		{
			std::cout << "Pressed" << std::endl;
		}
		std::cout << button << ", " << action << ", " << mods << std::endl;
	}

	// Good to keep, so you don't have to check all the buttons and which ones are pressed. You can just immediately add them to the map
	void Input::onKeyInput(int button, int scancode, int action, int mods)
	{
		if (button == GLFW_KEY_E && action == GLFW_PRESS)
		{
			std::cout << "E PRESSED!" << std::endl;
		}

		keyPressed[button].previous = keyPressed[button].current;
		if (action == GLFW_PRESS)
		{
			keyPressed[button].current = PressedState::PRESSED;
		}
		else if (action == GLFW_RELEASE)
		{
			keyPressed[button].current = PressedState::NONE;
		}
	}

	void Input::onScroll(double xOffset, double yOffset)
	{
		std::cout << "Scrolling" << std::endl;
	}

	void Input::setupJoystickInputs()
	{
		gamepadStates.reserve(maxNumControllersConnected);

		for (unsigned int i = 0; i < maxNumControllersConnected; i++)
		{
			gamepadStates.push_back(GLFWgamepadstate());
		}
	}

	// This will eventually not take the camera or deltaTime. It should really just create a map that other things can read from to see what keys are pressed
	void Input::ProcessInput(GLFWwindow* window, Camera& camera, float deltaTime)
	{
		
		for (auto i = keyPressed.begin(); i != keyPressed.end(); i++)
		{

			if (i->second.current != i->second.previous && i->second.previous == PressedState::NONE)
			{
				// Calling onPress callback
				std::cout << "Pressed " << i->first << std::endl;
				i->second.previous = i->second.current;
				pressedKeys[i->first] = true;
			}
			else if (i->second.current == PressedState::NONE && i->second.previous == PressedState::HELD)
			{
				// Calling onRelease
				std::cout << "Released " << i->first << std::endl;
				i->second.previous = PressedState::NONE;
				i->second.current = PressedState::NONE;
				releasedKeys[i->first] = true;

			}
			else if (i->second.current == PressedState::PRESSED && i->second.previous == PressedState::PRESSED)
			{
				// Calling onHeld
				std::cout << "Held " << i->first << std::endl;
				i->second.current = PressedState::HELD;
				i->second.previous = PressedState::HELD;
				heldKeys[i->first] = true;
			}
		}


		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
		if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(FORWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(BACKWARD, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(LEFT, deltaTime);
		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.ProcessKeyboard(RIGHT, deltaTime);
		}

		for (int i = 0; i < maxNumControllersConnected; i++)
		{
			if (glfwGetGamepadState(i, &gamepadStates[i]))
			{
				if (numControllersConnected < i + 1)
				{
					std::cout << "Connected" << std::endl;
					numControllersConnected++;
				}


				if (gamepadStates[i].buttons[GLFW_GAMEPAD_BUTTON_A])
				{
					std::cout << "X pressed" << std::endl;
				}

			}
			else
			{
				if (numControllersConnected > i)
				{
					std::cout << "Disconnected" << std::endl;
					numControllersConnected--;
				}

			}
		}
	}
}
