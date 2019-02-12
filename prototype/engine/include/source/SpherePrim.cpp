#include "..\SpherePrim.h"

namespace SystemEngine
{
	SpherePrim::SpherePrim(int gradation_x, int gradation_y)
	{
		m_gradation_x = gradation_x;

		m_gradation_y = gradation_y;

		if (m_cos.size() == 0)
		{
			GenericCos((float)gradation_x);
			GenericSin((float)gradation_x);
		}
	}

	SpherePrim::~SpherePrim()
	{
	}

	void SpherePrim::Create(shared_ptr<ScriptEngine::MeshFilter> mesh_filter, Vector3 pivot, float radius)
	{
		const float pi = (float)acos(-1);

		float _fi = 360.f / m_gradation_x;
		float _psi = 180.f / m_gradation_y;

		float fi = 0.f; float psi = 90.f;

		float fi_radian = pi / 180 * fi;
		float psi_radian = pi / 180 * psi;

		float _fi_radian = pi / 180 * _fi;
		float _psi_radian = pi / 180 * _psi;

		Vector3 _a, _c;

		float uv_x = 1.f / m_gradation_x;
		float uv_y = 1.f / m_gradation_y;

		for (int y = 0; y < m_gradation_y; y++)
		{
			for (int x = 0; x < m_gradation_x; x++)
			{

				float fi_radian = pi / 180 * fi;
				float psi_radian = pi / 180 * psi;

				float cos_psi = cos(psi_radian);
				float sin_psi = sin(psi_radian);
				float cos_psi_ = cos(psi_radian + _psi_radian);
				float sin_psi_ = sin(psi_radian + _psi_radian);

				float cos_fi = cos(fi_radian);
				float sin_fi = sin(fi_radian);
				float cos_fi_ = cos(fi_radian + _fi_radian);
				float sin_fi_ = sin(fi_radian + _fi_radian);

				Vector3 a = { cos_psi * cos_fi, cos_psi * sin_fi, sin_psi };
				a *= radius;
				a += pivot;

				Vector3 b = { cos_psi * cos_fi_, cos_psi * sin_fi_, sin_psi };
				b *= radius;
				b += pivot;

				Vector3 c = { cos_psi_ * cos_fi, cos_psi_ * sin_fi, sin_psi_ };
				c *= radius;
				c += pivot;

				Vector3 d = { cos_psi_ * cos_fi_, cos_psi_ * sin_fi_, sin_psi_ };
				d *= radius;
				d += pivot;
				
				if (x == 0)							{ _a = a; _c = c; }
				else if (x == (m_gradation_x - 1))	{ b = _a; d = _c; }

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

				fi += _fi;
			}

			psi += _psi;
		}

		//
		shared_ptr<ScriptEngine::Mesh> mesh = ScriptEngine::Mesh::Create("Sphere");
		//
		mesh->SetVertices(m_vertices);
		mesh->SetNormals(m_normals);
		mesh->SetUVs(0, m_uvs);
		mesh->SetTriangles(m_triangles);
		mesh_filter->SetMesh(mesh);
	}
}