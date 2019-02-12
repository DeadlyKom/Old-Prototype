#include "prototype\include\Shader.h"

#include "prototype\engine\include\ResourceManager.h"

namespace ScriptEngine
{
	// Конструктор
	Shader::Shader(int index)
	{
		m_index_res = index;
	}

	// Поиск шейдера по имени
	shared_ptr<Shader> Shader::Find(string name)
	{
		shared_ptr<Shader> return_shader;
		// Найдём индекс шейдера в списке шейдеров устройства отображения
		int index = SystemEngine::ResourceManager::GetInstance()->FindShader(NAME_SHADER + name);
		// Проверим на корректность поска шейдера по имени
		if (index != UNKNOW_INDEX_ELEMENTS)
		// Нужный шейдер найден
		{ return_shader = shared_ptr<Shader>(new Shader(index)); }
		// Возвращаем результат поиска
		return return_shader;
	}
}