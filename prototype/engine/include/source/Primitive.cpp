#include "..\Primitive.h"

namespace SystemEngine
{
	Primitive::Primitive()
	{
		m_pi = (float)acos(-1);
	}
	Primitive::~Primitive()
	{
	}

	void Primitive::GenericCos(float gradient)
	{
		float angel		= 360.f / gradient;
		float angel_rad = m_pi / 180.f * angel;

		for (int g = 0; g < gradient; g++)
		{
			m_cos.push_back(cos(angel_rad * g));
		}
	}

	void Primitive::GenericSin(float gradient)
	{
		float angel = 360.f / gradient;
		float angel_rad = m_pi / 180.f * angel;

		for (int g = 0; g < gradient; g++)
		{
			m_sin.push_back(sin(angel_rad * g));
		}
	}

	void Primitive::AddTriangles(Vector3 point_a, Vector3 point_b, Vector3 point_c, Vector3 normal_a, Vector3 normal_b, Vector3 normal_c, Vector2 uv_a, Vector2 uv_b, Vector2 uv_c)
	{
		m_triangles.push_back(AddVertex(point_a, normal_a, uv_a));
		m_triangles.push_back(AddVertex(point_b, normal_b, uv_b));
		m_triangles.push_back(AddVertex(point_c, normal_c, uv_c));
	}

	int Primitive::AddVertex(Vector3 vertex, Vector3 normal, Vector2 uv)
	{
		int pos;
		int size = m_vertices.size();

		for (pos = 0; pos < size; pos++)
		{
			if ((m_vertices[pos]).compare(vertex) &&
				(m_normals[pos]).compare(normal)  &&
				(m_uvs[pos].compare(uv)))				{ return pos; }
		}

		m_vertices.push_back(vertex);
		m_normals.push_back(normal);
		m_uvs.push_back(uv);

		return m_vertices.size() - 1;
	}
}