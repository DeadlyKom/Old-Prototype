#include "..\TorPrim.h"
namespace SystemEngine
{
	TorPrim::TorPrim(int gradient_x, int gradient_y)
	{
		m_gradation_x = gradient_x;
		m_gradation_y = gradient_y;

		if (m_cos.size() == 0)
		{
			GenericCos((float)gradient_x);
			GenericSin((float)gradient_x);
		}

	}
	TorPrim::~TorPrim()
	{
	}

	void TorPrim::Create(shared_ptr<ScriptEngine::MeshFilter> mesh_filter, Vector3 pivot, float radius_1, float radius_2)
	{
		int size_x = m_cos.size();
		int size_y = m_sin.size();

		float uv_x = 1.f / m_gradation_x;
		float uv_y = 0.5f / m_gradation_y;

		int _x, _y;

		for (int y = 0; y < size_y; y++)
		{
			for (int x = 0; x < size_x; x++)
			{
				Vector3 a = { (radius_1 + radius_2 * m_cos[x]) * m_cos[y], radius_2 * m_sin[x], -(radius_1 + radius_2 * m_cos[x]) * m_sin[y] };

				if ((x + 1) == size_x) { _x = 0; } else { _x = x + 1; }

				Vector3 b = { (radius_1 + radius_2 *  m_cos[_x]) * m_cos[y], radius_2 * m_sin[_x], -(radius_1 + radius_2 * m_cos[_x]) * m_sin[y] };
				
				if ((y + 1) == size_y) { _y = 0; } else { _y = y + 1; }

				Vector3 c = { (radius_1 + radius_2 *  m_cos[x]) * m_cos[_y], radius_2 * m_sin[x], -(radius_1 + radius_2 * m_cos[x]) * m_sin[_y] };

				Vector3 d = { (radius_1 + radius_2 *  m_cos[_x]) * m_cos[_y], radius_2 * m_sin[_x], -(radius_1 + radius_2 * m_cos[_x]) * m_sin[_y] };
				
				Vector3 normal = Vector3::Normalize(Vector3::Cross(b, a, c));

				Vector3 normal_a = normal;
				Vector3 normal_b = normal;
				Vector3 normal_c = normal;
				Vector3 normal_d = normal;

				Vector2 uv_a = { uv_x * x, uv_y * y };
				Vector2 uv_b = { uv_x * x + uv_x, uv_y * y };
				Vector2 uv_c = { uv_x * x, uv_y * y + uv_y };
				Vector2 uv_d = { uv_x * x + uv_x, uv_y * y + uv_y };

				AddTriangles(a, b, c, normal_a, normal_b, normal_c, uv_a, uv_b, uv_c);

				AddTriangles(b, d, c, normal_b, normal_d, normal_c, uv_b, uv_d, uv_c);
			}
		}

		//
		shared_ptr<ScriptEngine::Mesh> mesh = ScriptEngine::Mesh::Create("Tor");
		//
		mesh->SetVertices(m_vertices);
		mesh->SetNormals(m_normals);
		mesh->SetUVs(0, m_uvs);
		mesh->SetTriangles(m_triangles);
		mesh_filter->SetMesh(mesh);
	}
}