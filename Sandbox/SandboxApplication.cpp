#include "Ebony.hpp"
#include "camera.hpp"


#include "glm/glm.hpp"
#include <resourceManager.hpp>
#include <filesystem>
#include <spritefont.hpp>

namespace Ebony {

	class Sandbox : public Application
	{
	public:
		Sandbox()
		{

		}

		~Sandbox()
		{
			graphics.Cleanup();

		}

		void Init() override
		{
			Camera camera(glm::vec3(0.0f, 0.0f, 0.0f));

			graphics.Initialize("Ebony", 800, 600);
			graphics.SetMainCamera(camera);
			Ebony::KeyInput::setupKeyInputs(graphics.window);

			std::vector<int> keys = { GLFW_KEY_E, GLFW_KEY_ESCAPE };
			keyInput.setKeysToMonitorInit(keys);


            Texture2D& faceTexture = ResourceManager::LoadTexture("../Graphics/textures/awesomeface.tx", "face");
            Shader& s = ResourceManager::LoadShader("../Graphics/shaders/sprite.vert", "../Graphics/shaders/sprite.frag", "default");
			ResourceManager::LoadShader("../Graphics/shaders/font.vert", "../Graphics/shaders/font.frag", "text");
            
			Color clearColor = Color::Color(0.1f, 0.1f, 0.1f, 1.0f);

            s.use();
            s.setInt("image", 0);
            s.setMat4("projection", graphics.projection);
			clearColor = Color::Color(0.1f, 0.1f, 0.1f, 1.0f);

			s.use();
			s.setInt("image", 0);
			s.setMat4("projection", graphics.projection);
			glEnable(GL_DEPTH_TEST);
		}


		void ProcessInput() override
		{
			Ebony::PressedState keyState = keyInput.getIsKeyDown(GLFW_KEY_E);

			if (keyState == PressedState::HELD)
			{
				std::cout << "Held" << std::endl;
			}
			else if (keyState == PressedState::PRESSED)
			{
				std::cout << "Pressed" << std::endl;
			}
			else if (keyState == PressedState::RELEASED)
			{
				std::cout << "Released" << std::endl;
			}

			if (keyInput.getKeyPressedOrHeld(GLFW_KEY_ESCAPE))
			{
				glfwSetWindowShouldClose(graphics.window.getWindow(), true);
			}

		}


		void Update() override
		{
			float currentFrame = static_cast<float>(glfwGetTime());
			deltaTime = currentFrame - lastFrame;
			lastFrame = currentFrame;

		}


		void Draw() override
		{
			
			//std::cout << "Drawing!" << std::endl;
			graphics.BeginDraw(clearColor);


			graphics.Draw(ResourceManager::GetTexture("face"), glm::vec2(200.0f, 0.0f), glm::vec2(300.0f, 400.0f), 45.0f, Colors::Red);
			graphics.DrawString(ResourceManager::GetShader("text"), spriteFont, "Test", 25.0f, 100.0f, 1.0f, Colors::Red);


			graphics.EndDraw();

		}

		void Run() override
		{
			WindowResizeEvent e(1280, 720);

			std::cout << "Beep" << std::endl;
			EB_TRACE(e);

			Init();
			graphics.InitializeTextDrawing(ResourceManager::GetShader("text"));
			spriteFont.LoadFont("../Graphics/fonts/super-indie-font/SuperIndie.ttf");


			while (!glfwWindowShouldClose(graphics.window.getWindow()))
			{
				ProcessInput();
				Update();
				Draw();
				glfwPollEvents();
			}

			glfwTerminate();
		}


	public:
		Graphics2d graphics;
		KeyInput keyInput;
		Color clearColor;
		SpriteFont spriteFont;
		float deltaTime = 0.0f;
		float lastFrame = 0.0f;
	
	};


	Ebony::Application* Ebony::CreateApplication()
	{
		return new Sandbox();
	}

}