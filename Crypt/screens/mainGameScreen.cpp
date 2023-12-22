#include "mainGameScreen.hpp"

#include "../prefabs/playerPrefab.hpp"
#include "../prefabs/groundPrefab.hpp"
#include "../prefabs/crosshairPrefab.hpp"
#include "../prefabs/batPrefab.hpp"

namespace Crypt
{

	void MainGameScreen::LoadContent()
	{
		Ebony::ResourceManager::LoadAtlas("character_run.tx", "character_run", "Crypt", 4, 1);
		Ebony::ResourceManager::LoadTexture("Default.tx", "default", "Crypt");
		Ebony::ResourceManager::LoadTexture("particle.tx", "collider", "Crypt");
		Ebony::ResourceManager::LoadTexture("empty.tx", "empty", "Crypt");
		Ebony::ResourceManager::LoadTexture("new_aim.tx", "aim", "Crypt");
		Ebony::ResourceManager::LoadTexture("FireBall.tx", "fire_bullet", "Crypt");
		Ebony::ResourceManager::LoadTexture("IceBolt.tx", "ice_bullet", "Crypt");
		Ebony::ResourceManager::LoadTexture("Bat1.tx", "bat", "Crypt");

	}

	void MainGameScreen::Start()
	{
		Shader& s = Ebony::ResourceManager::LoadShader("shaders/sprite.vert", "shaders/sprite.frag", "default");
		Ebony::ResourceManager::LoadShader("shaders/font.vert", "shaders/font.frag", "text");

		Ebony::Graphics2d::InitializeTextDrawing(Ebony::ResourceManager::GetShader("text"));
		this->clearColor = Ebony::Colors::CornflowerBlue;


		s.use();
		s.setInt("image", 0);
		s.setMat4("projection", Ebony::Graphics2d::projection);

		physicsSystem = systems::PhysicsSystem();
		spriteRenderer = systems::SpriteRenderer();
		animationSystem = systems::Animation2d();
		animationRenderer = systems::AnimationRenderer();
		playerSystem = systems::PlayerSystem();
		inputSystem = systems::InputSystem();
		audioSystem = systems::AudioSystem();
		cppScriptingSystem = systems::CppScriptingSystem();
		timingSystem = systems::TimingSystem();
		destructionSystem = systems::DestructionSystem([=](entities::Entity::IdType entityId) {RemoveEntity(entityId); });

		spriteRenderer.debug = true;


		auto player = Crypt::Player::Create(glm::vec2(20.0f, 50.0f), windowWidth);

		// Create prefabs
		AddEntity(player);
		AddEntity(Crypt::Ground::Create(glm::vec2(0.0f, windowHeight - 5.0f), windowWidth));
		AddEntity(Crypt::Ground::Create(glm::vec2(0.0f, 0.0f), windowWidth));
		AddEntity(Crypt::Crosshair::Create(glm::vec2(25.0f, 0.0f), player, [=](entities::EntityPtr entity) {AddEntity(entity); }));

		AddEntity(Crypt::Bat::Create(glm::vec2(150.0f, 50.0f), glm::vec2(75.0f, 75.0f)));
		AddNewEntities();
	}

	void MainGameScreen::Init(int windowWidth, int windowHeight)
	{
		camera = std::make_shared<Camera>(glm::vec3(0.0f, 0.0f, 1.0f));

		this->windowHeight = windowHeight;
		this->windowWidth = windowWidth;
		mainRenderTarget = Ebony::RenderTarget2D::Create(windowWidth, windowHeight, GL_LINEAR, GL_NEAREST);
		clearColor = Ebony::Colors::Black;

		Ebony::Graphics2d::SetMainCamera(camera);

	}

	

	std::uint64_t MainGameScreen::Update(std::chrono::microseconds elapsedTime)
	{
		auto firstTime = std::chrono::system_clock::now();
		Ebony::Graphics2d::mainCamera->Position = glm::vec3(Ebony::Graphics2d::mainCamera->Position.x + 0.01f, Ebony::Graphics2d::mainCamera->Position.y, Ebony::Graphics2d::mainCamera->Position.z);

		std::latch graphDone{ 1 };

		auto taskGraph = Ebony::ThreadPool::instance().createTaskGraph(
			[&graphDone]()
			{
				graphDone.count_down();
			});

		auto audioTask = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				audioSystem.Update(elapsedTime);
			}
		);

		auto inputTask = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				inputSystem.Update();
			}
		);

		auto animationTask = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				animationSystem.Update(elapsedTime);
			}
		);

		auto playerTask = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				playerSystem.Update(elapsedTime);
			}
		);

		auto scriptingTask = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				cppScriptingSystem.Update(elapsedTime);
			}
		);

		auto shootingTask = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				shootingSystem.Update(elapsedTime);
			}
		);

		auto timerTask = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				timingSystem.Update(elapsedTime);
			}
		);

		auto destructionTask = Ebony::ThreadPool::instance().createTask(
			taskGraph,
			[this, elapsedTime]()
			{
				destructionSystem.Update(elapsedTime);
			}
		);



		Ebony::ThreadPool::instance().submitTaskGraph(taskGraph);
		graphDone.wait();

		// Put this outside the update loop so in the future, I can use all the threads to then do multi-threaded physics updates
		physicsSystem.Update(elapsedTime);

		return nextScreen;
	}

	void MainGameScreen::Draw(std::chrono::microseconds elapsedTime)
	{
		Ebony::Graphics2d::SetRenderTarget(mainRenderTarget, clearColor);

		// Draw things!
		spriteRenderer.Update();
		animationRenderer.Update();

		Ebony::Graphics2d::UnbindRenderTarget(clearColor);

		Ebony::Graphics2d::DrawRenderTarget(Ebony::ResourceManager::GetShader("screenTexture"), mainRenderTarget);
	}

	void MainGameScreen::ProcessInput(std::chrono::microseconds elapsedTime)
	{

	}

	void MainGameScreen::AddEntity(entities::EntityPtr entity)
	{
		std::lock_guard<std::mutex> lock(mutexEntities);

		newEntities.push_back(entity);
	}

	void MainGameScreen::RemoveEntity(entities::Entity::IdType id)
	{
		std::lock_guard<std::mutex> lock(mutexEntities);

		removeEntities.insert(id);
	}

	void MainGameScreen::OnScreenDefocus()
	{
		nextScreen = screen;

		
	}

	void MainGameScreen::OnScreenFocus()
	{
		Start();

		
	}

    void MainGameScreen::SetNextScreen(std::uint16_t nextScreenIncoming)
	{
		nextScreen = nextScreenIncoming;
	}


	void MainGameScreen::AddNewEntities()
	{
		for (auto&& entity : newEntities)
		{
			spriteRenderer.AddEntity(entity);
			physicsSystem.AddEntity(entity);
			audioSystem.AddEntity(entity);
			inputSystem.AddEntity(entity);
			animationSystem.AddEntity(entity);
			animationRenderer.AddEntity(entity);
			playerSystem.AddEntity(entity);
			cppScriptingSystem.AddEntity(entity);
			shootingSystem.AddEntity(entity);
			destructionSystem.AddEntity(entity);
			timingSystem.AddEntity(entity);

			allEntities[entity->getId()] = entity;
		}
		newEntities.clear();
	}

	void MainGameScreen::RemoveOldEntities()
	{
		for (auto&& entityId : removeEntities)
		{
			allEntities.erase(entityId);
			spriteRenderer.RemoveEntity(entityId);
			physicsSystem.RemoveEntity(entityId);
			audioSystem.RemoveEntity(entityId);
			inputSystem.RemoveEntity(entityId);
			animationSystem.RemoveEntity(entityId);
			animationRenderer.RemoveEntity(entityId);
			playerSystem.RemoveEntity(entityId);
			cppScriptingSystem.RemoveEntity(entityId);
			shootingSystem.RemoveEntity(entityId);
			destructionSystem.RemoveEntity(entityId);
			timingSystem.RemoveEntity(entityId);
		}

		removeEntities.clear();
	}

}