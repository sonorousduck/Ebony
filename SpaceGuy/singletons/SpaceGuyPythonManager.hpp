#pragma once

#include <misc/actionSpaces.hpp>

#include <pybind11/stl.h>

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

#include <singletons/time.hpp>
#include <iostream>
#include <singletons/systemManager.hpp>

namespace SpaceGuy
{
	struct State
	{
	
		State()
		{}

		State(Ebony::Box playerPosition) : playerPosition(playerPosition) {}

		Ebony::Box playerPosition{};
		Ebony::Box keyPosition{};
		
		std::vector<float> getPlayerPosition() const { return playerPosition.box; }
		std::vector<Ebony::Box> getEnemyPositions() const { return enemyPositions; }
		std::vector<Ebony::Box> getBulletInformation() const { return bulletInformation; }
		std::vector<float> getKeyInformation() const { return keyPosition.box; }


		//Ebony::Box crosshairPosition;
		std::vector<Ebony::Box> enemyPositions;
		std::vector<Ebony::Box> bulletInformation;
		
	};

	//struct Pet {
	//	Pet(const std::string& name) : name(name) { }
	//	void setName(const std::string& name_) { name = name_; }
	//	const std::string& getName() const { return name; }

	//	std::string name;
	//};



	class SpaceGuyPythonManager
	{
	public:
		static void Update(std::chrono::microseconds elapsedTime);
		static void ProcessInput();
		static void Reset();

		static void Init(const char* filename, bool isAiStartup = false);
		static void Destroy();


		static Ebony::Action action;
		static State state;
		static Ebony::Discrete reward;


	private:
		static pybind11::module pyModule;
		static bool initialized;


	};
}