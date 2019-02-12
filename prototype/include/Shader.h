#pragma once

#include <memory>
#include <string>

#include "Type.h"

#include "prototype\engine\include\ResourceManager.h"

using namespace std;

namespace ScriptEngine
{
	class Shader
	{
	private:
									Shader() = delete;
		// Конструктор
									Shader(int index);
	public:
		// Поиск шейдера по имени
		static shared_ptr<Shader>	Find(string name);
		
		friend shared_ptr<SystemEngine::ShaderRes>		SystemEngine::ResourceManager::GetShaderRes(shared_ptr<Shader> shader);

	protected:
		// Внутренние переменные
		int							m_index_res;			// индекс шейдера в общем списке
	};
}