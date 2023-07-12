#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Ebony
{

	class Color
	{
	public:
		Color();
		Color(glm::vec3 color);
		Color(glm::vec4 color);
		Color(float r, float b, float g);
		Color(float r, float b, float g, float a);


		

		glm::vec3 GetRGB();
		glm::vec4 GetRGBA();
		float r() { return rgba.r; };
		float g() { return rgba.g; };
		float b() { return rgba.b;  };
		float a() { return rgba.a; };

	private:
		glm::vec4 rgba;
	};

	namespace Colors
	{
		static Color Red = Color(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
		static Color Blue = Color(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));;
		static Color Yellow = Color(glm::vec4(1.0f, 1.0f, 0.0f, 1.0f));
		static Color Black = Color(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
		static Color White = Color(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		static Color Green = Color(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
		static Color CornflowerBlue = Color(glm::vec4(0.576f, 0.8f, 0.917f, 1.0f));

	}
	

}