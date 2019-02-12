#pragma once

#include <memory>

#include "Renderer.h"
#include "Type.h"

using namespace std;

namespace ScriptEngine
{
	class MeshRender : public Renderer
	{
	private:
		// Конструктор по умолчанию
											MeshRender();
		// Конструктор копирования
											MeshRender(const MeshRender&&) = delete;
		// Оператор присвоения
		MeshRender&							operator=(const MeshRender&&) = delete;

	public:
		// Создание объекта
		static shared_ptr<MeshRender>		Create(string name);
		// Деструктор
		virtual								~MeshRender();
		// Возвращение компонента типа get_type_component, если он добавлен и empty, если не был добавлен
		template<class get_type_component>
		shared_ptr<get_type_component>		GetComponent();
	};

	// Возвращение компонента типа get_type_component, если он добавлен и empty, если не был добавлен
	template<class get_type_component>
	inline shared_ptr<get_type_component> MeshRender::GetComponent()
	{
		return m_scene_object->GetComponent<get_type_component>();
	}
}