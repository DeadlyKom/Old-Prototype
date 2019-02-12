#pragma once

#include <memory>
#include <vector>

#include "prototype\include\Type.h"
#include "prototype\include\Vector.h"
#include "prototype\include\MeshFilter.h"

using namespace std;
using namespace AddStruct;

namespace SystemEngine
{
	class Primitive
	{
	public:
							Primitive();
		virtual				~Primitive();

	protected:
		void				GenericCos(float gradient);
		void				GenericSin(float gradient);

		void				AddTriangles(Vector3 point_a, Vector3 point_b, Vector3 point_c, Vector3 normal_a, Vector3 normal_b, Vector3 normal_c, Vector2 uv_a, Vector2 uv_b, Vector2 uv_c);

		int					AddVertex(Vector3 vertex, Vector3 normal, Vector2 uv);

		vector<Vector3>		m_vertices;
		vector<Vector3>		m_normals;
		vector<Vector2>		m_uvs;
		vector<uint16>		m_triangles;

		vector<float>		m_cos;
		vector<float>		m_sin;
		float				m_pi;
	};
}