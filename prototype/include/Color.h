#pragma once

#include "prototype\include\Type.h"
#include "prototype\include\Vector.h"

namespace AddStruct
{
	struct Color32
	{
		Color32()									: r(0), g(0), b(0), a(0) {};

		Vector4		GetFloat()
		{
			Vector4 floats;

			floats.x = (float)r / 255.0f;
			floats.y = (float)g / 255.0f;
			floats.z = (float)b / 255.0f;
			floats.w = (float)a / 255.0f;

			return floats;
		}

		union
		{
			struct
			{
				byte r : 8;
				byte g : 8;
				byte b : 8;
				byte a : 8;
			};
		};
	};

	struct ColorFloat
	{
		ColorFloat()										: r(0.0f), g(0.0f), b(0.0f), a(0.0f)			{};
		ColorFloat(float _r, float _g, float _b, float _a)	: vector(Vector4(_r, _g, _b, _a))				{};
		ColorFloat(const ColorFloat& copy)					: r(copy.r), g(copy.g), b(copy.b), a(copy.a)	{};

		union
		{
			struct
			{
				float r;
				float g;
				float b;
				float a;
			};
			Vector4 vector;
		};
	};

	namespace Colors
	{
#		define GLOBALCONST extern const __declspec(selectany)

		GLOBALCONST ColorFloat Transparent = { 0.0f, 0.0f, 0.0f, 0.0f };
	}
}