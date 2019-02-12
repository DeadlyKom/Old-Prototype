#pragma once

#include <DirectXMath.h>

#include <cfloat>
#include <cmath>
#include <cstring>

using namespace std;
using namespace DirectX;

namespace AddStruct
{
#	define EPSILON FLT_EPSILON

	struct Vector2
	{
		Vector2() : x(0), y(0) {};
		Vector2(float _x, float _y) : x(_x), y(_y) {};

		bool	 compare(Vector2 cmp, float epsilon = EPSILON)
		{
			bool c_x = abs(cmp.x - x) < epsilon;
			bool c_y = abs(cmp.y - y) < epsilon;

			return c_x && c_y;
		}

		union
		{
			struct
			{
				float x;
				float y;
			};
		};
	};

	struct Vector3
	{
		Vector3()								: x(0), y(0), z(0)					{};
		Vector3(float _x, float _y, float _z)	: x(_x), y(_y), z(_z)				{};
		Vector3(const Vector3& copy)			: x(copy.x), y(copy.y), z(copy.z)	{};

		//Vector3& operator=	(const Vector3& vector) {}

		Vector3& operator+	(Vector3& plus)			{ x += plus.x;	y += plus.y;	z += plus.z;	return *this; }
		Vector3& operator+=	(Vector3& plus)			{ x += plus.x;	y += plus.y;	z += plus.z;	return *this; }
		Vector3& operator+=	(const Vector3& plus)	{ x += plus.x;	y += plus.y;	z += plus.z;	return *this; }
		Vector3& operator-	(Vector3& sub)			{ x -= sub.x;	y -= sub.y;		z -= sub.z;		return *this; }
		Vector3& operator-=	(Vector3& sub)			{ x -= sub.x;	y -= sub.y;		z -= sub.z;		return *this; }
		Vector3& operator-=	(const Vector3& plus)	{ x -= plus.x;	y -= plus.y;	z -= plus.z;	return *this; }
		Vector3& operator*	(float mult)			{ x *= mult;	y *= mult;		z *= mult;		return *this; }
		Vector3& operator*=	(float mult)			{ x *= mult;	y *= mult;		z *= mult;		return *this; }
		Vector3& operator/	(float div)				{ x /= div;		y /= div;		z /= div;		return *this; }
		Vector3& operator/=	(float div)				{ x /= div;		y /= div;		z /= div;		return *this; }
		
		static float Dot(Vector3& vector_a, Vector3& vector_b)
		{
			float dot = vector_a.x * vector_b.x + vector_a.y * vector_b.y + vector_a.z * vector_b.z;

			return dot;
		}

		//[V1.y*V2.z - V1.z*V2.y, V1.z*V2.x - V1.x*V2.z, V1.x*V2.y - V1.y*V2.x]
		static Vector3 Cross(Vector3 point_a, Vector3 point_b, Vector3 point_c)
		{
			Vector3 v1 = point_b - point_a;
			Vector3 v2 = point_c - point_a;

			Vector3 output_normal { v1.y * v2.z - v1.z * v2.y, v1.z * v2.x - v1.x * v2.z, v1.x * v2.y - v1.y * v2.x };

			return output_normal;
		}

		static float Length(Vector3 point_a, Vector3 point_b)
		{
			Vector3 vector = point_b - point_a;

			float out_length = sqrtf(vector.x * vector.x + vector.y * vector.y + vector.z * vector.z);

			return out_length;
		}

		static float Length(Vector3& vector)
		{
			float out_length = sqrtf(abs(vector.x * vector.x) + abs(vector.y * vector.y) + abs(vector.z * vector.z));

			return out_length;
		}

		static Vector3 Normalize(Vector3& vector)
		{
			float length = Length(vector);

			Vector3 normalize = vector / length;

			return normalize;
		}

		bool	 compare(Vector3 cmp, float epsilon = EPSILON)
		{
			bool c_x = abs(cmp.x - x) < epsilon;
			bool c_y = abs(cmp.y - y) < epsilon;
			bool c_z = abs(cmp.z - z) < epsilon;

			return c_x && c_y && c_z;
		}
		union
		{
			struct
			{
				float x;
				float y;
				float z;
			};
		};
	};

	struct Vector4
	{
		Vector4()										: x(0), y(0), z(0), w(0)						{};
		Vector4(float _x, float _y, float _z, float _w)	: x(_x), y(_y), z(_z), w(_w)					{};
		Vector4(const Vector4& copy)					: x(copy.x), y(copy.y), z(copy.z), w(copy.w)	{};

		union
		{
			struct
			{
				float x;
				float y;
				float z;
				float w;
			};
		};
	};

	struct Matrix4X4
	{
		Matrix4X4() :	_11(1.0f), _12(0.0f), _13(0.0f), _14(0.0f),
						_21(0.0f), _22(1.0f), _23(0.0f), _24(0.0f),
						_31(0.0f), _32(0.0f), _33(1.0f), _34(0.0f),
						_41(0.0f), _42(0.0f), _43(0.0f), _44(1.0f) {}

		union
		{
			struct
			{
				float _11, _12, _13, _14;
				float _21, _22, _23, _24;
				float _31, _32, _33, _34;
				float _41, _42, _43, _44;
			};
			float	m[4][4];
			float	ml[16];
			//__m128	m128;
		};
	};
}