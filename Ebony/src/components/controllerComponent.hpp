#pragma once

#include <functional>
#include "component.hpp"
#define RAPIDJSON_HAS_STDSTRING 1
#include "rapidjson/prettywriter.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"
#include <fstream>
#include <rapidjson/ostreamwrapper.h>
#include <rapidjson/istreamwrapper.h>
#include "../entity.hpp"

namespace components
{
	// I think that derived classes could be handled PolymorphicComparable<Input, MouseInput> or something like that. That way they will all show up under input
	class ControllerInput : public PolymorphicComparable<Component, ControllerInput>
	{
	public:
		ControllerInput() {};
		ControllerInput(int joystickId) : joystickId(joystickId) {};

		std::unordered_map<int, std::string> bindings{}; // This is what defines our controller bindings. i.e. pressing space gives the string_view "jump"
		std::unordered_map<std::string_view, std::function<void([[maybe_unused]] entities::EntityPtr& entity)>> onPressActions{}; // This defines our on initial press actions. i.e. "jump" causes the player to... jump...
		std::unordered_map<std::string_view, std::function<void([[maybe_unused]] entities::EntityPtr& entity)>> onHeldActions{}; // This defines our on held actions. i.e. holding w keeps going forward
		std::unordered_map<std::string_view, std::function<void([[maybe_unused]] entities::EntityPtr& entity)>> onReleaseActions{}; // This defines our on released actions. i.e. releasing at the correct time to time something


		//std::unordered_map<int, std::function<void()>> controllerActionKeyPairs{};
		std::unordered_map<int, std::string> joystickBindings{};
		std::unordered_map<std::string_view, std::function<void(float)>> joystickActions{};

		//std::unordered_map<int, bool> actions{};
		//std::unordered_map<int, bool> previousActions{};

		// TODO: Probably need to have a controller id attributed to an input as well (i.e. so you can have it known this is player one's input)
		int joystickId = -1;


	public:
		void saveControllerBindings(std::string_view filepathBindings, std::string_view filepathJoystickBindings)
		{
			{
				rapidjson::Document saveDocument{};
				rapidjson::Document::AllocatorType& allocator = saveDocument.GetAllocator();
				saveDocument.SetObject();


				for (auto iter = this->bindings.begin(); iter != this->bindings.end(); iter++)
				{
					std::string intAsString = std::to_string(iter->first);
					rapidjson::Value index(intAsString.c_str(), static_cast<rapidjson::SizeType>(intAsString.size()), allocator);

					// Luckily, this save method won't happen often, so even though it is terrible to fully copy a string_view, it will be okay
					rapidjson::Value v(std::string(iter->second), allocator);
					saveDocument.AddMember(index, v, allocator);
				}

				// https://rapidjson.org/md_doc_stream.html
				std::ofstream ofs(filepathBindings.data());
				rapidjson::OStreamWrapper osw(ofs);

				rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
				saveDocument.Accept(writer);
			}

			{
				rapidjson::Document saveJoystickDocument{};
				rapidjson::Document::AllocatorType& allocator = saveJoystickDocument.GetAllocator();
				saveJoystickDocument.SetObject();


				for (auto iter = this->joystickBindings.begin(); iter != this->joystickBindings.end(); iter++)
				{
					std::string intAsString = std::to_string(iter->first);
					rapidjson::Value index(intAsString.c_str(), static_cast<rapidjson::SizeType>(intAsString.size()), allocator);

					// Luckily, this save method won't happen often, so even though it is terrible to fully copy a string_view, it will be okay
					rapidjson::Value v(std::string(iter->second), allocator);
					saveJoystickDocument.AddMember(index, v, allocator);
				}

				// https://rapidjson.org/md_doc_stream.html
				std::ofstream ofs(filepathJoystickBindings.data());
				rapidjson::OStreamWrapper osw(ofs);

				rapidjson::Writer<rapidjson::OStreamWrapper> writer(osw);
				saveJoystickDocument.Accept(writer);
			}
		}

		void loadControllerBindings(std::string_view filepathBindings, std::string_view filepathJoystickBindings)
		{
			{
				rapidjson::Document document;
				std::ifstream ifs(filepathBindings.data());
				rapidjson::IStreamWrapper isw(ifs);

				document.ParseStream(isw);

				// We now have the bindings, so it is our job to load them in.
				// int -> string
				for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); itr++)
				{
					this->bindings.insert({ std::stoi(itr->name.GetString()), itr->value.GetString() });
				}
			}

			{
				rapidjson::Document document;
				std::ifstream ifs(filepathJoystickBindings.data());
				rapidjson::IStreamWrapper isw(ifs);

				document.ParseStream(isw);

				// We now have the bindings, so it is our job to load them in.
				// int -> string
				for (rapidjson::Value::ConstMemberIterator itr = document.MemberBegin(); itr != document.MemberEnd(); itr++)
				{
					this->joystickBindings.insert({ std::stoi(itr->name.GetString()), itr->value.GetString() });
				}
			}
		}


	};
}