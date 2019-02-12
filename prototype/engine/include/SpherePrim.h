#pragma once

#include "Primitive.h"

namespace SystemEngine
{
	class SpherePrim : public Primitive
	{
	public:
							SpherePrim(int gradation_x, int gradation_y);
		virtual				~SpherePrim();

		void				Create(shared_ptr<ScriptEngine::MeshFilter> mesh_filter, Vector3 pivot, float radius);

	private:
		int					m_gradation_x;
		int					m_gradation_y;
	};
}