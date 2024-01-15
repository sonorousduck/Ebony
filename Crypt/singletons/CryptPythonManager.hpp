#pragma once

#include <misc/actionSpaces.hpp>

#include <pybind11/stl.h>

#include <pybind11/pybind11.h>
#include <pybind11/embed.h>

#include <singletons/time.hpp>
#include <iostream>


namespace Crypt
{
	struct State
	{
	
		State()
		{}

		State(Ebony::Box playerPosition) : playerPosition(playerPosition) {}

		Ebony::Box playerPosition;
		std::vector<float> getPlayerPosition() const { return playerPosition.box; }
		float testPlayerPosition() const { return playerPosition.box[0]; }

		//Ebony::Box crosshairPosition;
		//std::vector<Ebony::Box> enemyPositions;
	};

	struct Pet {
		Pet(const std::string& name) : name(name) { }
		void setName(const std::string& name_) { name = name_; }
		const std::string& getName() const { return name; }

		std::string name;
	};



	class CryptPythonManager
	{
	public:
		static void Update(std::chrono::microseconds elapsedTime);
		static void ProcessInput();

		static void Init(const char* filename, std::uint64_t environmentCount = 1);
		static void Destroy();


		static std::vector<std::vector<Ebony::Box>> actions;
		static std::vector<std::vector<State>> states;

	private:
		static pybind11::module pyModule;
		//static pybind11::module stateModule;
		static bool initialized;


	};
}