#include "Application.hpp"
#include "EntryPoint.hpp"
#include <graphics2d.hpp>
#include "screens/screen.hpp"
#include "screens/screenEnums.hpp"
#include "screens/gameScreen.hpp"
#include <singletons/time.hpp>
#include <singletons/inputManager.hpp>
#include "../Ebony/src/singletons/audioManager.hpp"
#include "screens/mainScreen.hpp"

namespace Ebony {

	class BrickBreakerGame : public Application
	{
	public:
		BrickBreakerGame()
		{
	
		}

		~BrickBreakerGame()
		{
			Ebony::Graphics2d::Cleanup();
		}
			
		void Init() override
		{
			// Set up graphics here
			Application::Init();

			Ebony::Graphics2d::Initialize("Brick Breaker", windowWidth, windowHeight);
			Ebony::AudioManager::Init();

			InputManager::Initialize();


			// Add screens here as well
			screens[BrickBreaker::ScreenEnum::GAME] = std::make_shared<BrickBreaker::GameScreen>();
			screens[BrickBreaker::ScreenEnum::MAIN_MENU] = std::make_shared<BrickBreaker::MainScreen>();
			

			currentScreen = screens[BrickBreaker::ScreenEnum::MAIN_MENU];
			nextScreenEnum = BrickBreaker::ScreenEnum::MAIN_MENU;
			// TODO: Get ResourceManager to register fonts in a good way, but for now, use the graphics.LoadFont way
			// Also, register any default fonts that I want to include throughout all the project

			for (auto& screen : screens)
			{
				screen.second->Init(windowWidth, windowHeight);
			}
		}

		void LoadContent() override
		{
			for (auto& screen : screens)
			{
				screen.second->LoadContent();
			}

			Application::LoadContent();
		}

		void ProcessInput(std::chrono::microseconds elapsedTime) override
		{
			// Update the SDL information here
			quit = InputManager::HandleInput();

			currentScreen->ProcessInput(elapsedTime);

			Application::ProcessInput(elapsedTime);
		}

		void Update(std::chrono::microseconds elapsedTime) override
		{
			currentScreen->windowHeight = windowHeight;
			currentScreen->windowWidth = windowWidth;

			if (newScreenFocused)
			{
				currentScreen->nextScreen = currentScreen->screen;
				currentScreen->OnScreenFocus(lastScreenEnum);
				newScreenFocused = false;
			}

			nextScreenEnum = currentScreen->Update(elapsedTime);


			if (screens[nextScreenEnum] != currentScreen)
			{
				lastScreenEnum = this->currentScreenEnum;
				currentScreen->OnScreenDefocus(nextScreenEnum);
				newScreenFocused = true;
			}
		

			Application::Update(elapsedTime);
		}


		void Draw(std::chrono::microseconds elapsedTime) override
		{
			Ebony::Graphics2d::BeginDraw(clearColor);
			currentScreen->Draw(elapsedTime);


			Application::Draw(elapsedTime);
			//Ebony::Graphics2d::EndImgui();
			Ebony::Graphics2d::EndDraw();
		}
		
		void ChangeScreens() override
		{
			// Reset the screen to have the next screen of itself so it doesn't infinitely loop


			currentScreen = screens[nextScreenEnum];
			currentScreenEnum = this->nextScreenEnum;

			if (nextScreenEnum == BrickBreaker::QUIT)
			{
				quit = true;
			}
		}

		void AddNewEntities() override
		{
			currentScreen->AddNewEntities();
		}

		void RemoveOldEntities() override
		{
			currentScreen->RemoveOldEntities();
		}


		void Run() override
		{
			Init();
			LoadContent();

			std::shared_ptr<Shader> s1 = Ebony::ResourceManager::LoadShader("shaders/screenTexture.vert", "shaders/screenTexture.frag", "screenTexture");

			s1->use();
			s1->setInt("screenTexture", 0);


			std::shared_ptr<Shader> s = Ebony::ResourceManager::LoadShader("shaders/spritesheet3d.vert", "shaders/spritesheet3d.frag", "spritesheet");

			s->use();
			s->setInt("spritesheet", 0);
			s->setMat4("projection", Ebony::Graphics2d::projection);

			std::shared_ptr<Shader> s2 = Ebony::ResourceManager::LoadShader("shaders/particle.vert", "shaders/particle.frag", "defaultParticle");

			s2->use();
			s2->setInt("particleTexture", 0);
			s2->setMat4("projection", Ebony::Graphics2d::projection);



			auto firstTime = std::chrono::system_clock::now();
			auto previousTime = std::chrono::system_clock::now();
			int frame = 0;

			currentScreen->Start();

			while (!quit)
			{
				auto currentTime = std::chrono::system_clock::now();
				auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime);
				Ebony::Time::SetDeltaTime(elapsedTime);
				previousTime = currentTime;

				Ebony::InputManager::ResetInput();

				//EB_TRACE(frame);
				ProcessInput(elapsedTime);
				Update(elapsedTime);
				Draw(elapsedTime);

				RemoveOldEntities();
				AddNewEntities();

				if (newScreenFocused)
				{
					ChangeScreens();
				}

				frame++;
			}
			
			ThreadPool::terminate();
			//EbonyAudio::AudioManager::StopAll();
			Ebony::ResourceManager::Clear();
		}

	public:

		int windowWidth = 800;
		int windowHeight = 600;
		bool quit = false;

		bool newScreenFocused = false;

	private:
		std::shared_ptr<Screen> currentScreen;
		std::unordered_map<std::uint16_t, std::shared_ptr<Screen>> screens{};
		
		std::uint64_t currentScreenEnum = Ebony::ScreenEnum::DEFAULT;
		std::uint64_t lastScreenEnum = Ebony::ScreenEnum::DEFAULT;
		std::uint64_t nextScreenEnum = Ebony::ScreenEnum::DEFAULT;
		Ebony::Color clearColor = Ebony::Colors::CornflowerBlue;

	};

	Ebony::Application* Ebony::CreateApplication()
	{
		return new BrickBreakerGame();
	}


}