#pragma once

#include "Primitive.h"


namespace SystemEngine
{
	class TorPrim : public Primitive
	{
	public:
							TorPrim(int gradient_x, int gradient_y);
		virtual				~TorPrim();

		void				Create(shared_ptr<ScriptEngine::MeshFilter> mesh_filter, Vector3 pivot, float radius_1, float radius_2);

	private:
		int					m_gradation_x;
		int					m_gradation_y;

	};
}