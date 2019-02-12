#pragma once

#include <memory>

#include "Object.h"
#include "SceneObject.h"

using namespace std;

namespace ScriptEngine
{
	class Transform;

	class Component : public IObject
	{
	protected:
		// Конструктор по умолчанию
										Component();
		// Конструктор копирования
										Component(const Component&) = delete;
		// Оператор присвоения
		Component&						operator=(const Component&) = delete;
		// Возвращение компонента типа get_type_component, если он добавлен к SO и empty, если не был добавлен
		template<class get_type_component>
		shared_ptr<get_type_component>	GetComponent();
	public:

		// Деструктор
		virtual							~Component() {}
		// Возвращение трансформа связанного с объектом сцены у текущего компонента
		shared_ptr<Transform>			GetTransform();
		// Установка трансформа связанного с объектом сцены у текущего компонента
		void							SetTransform() {};


		shared_ptr<SceneObject>			m_scene_object;		// ссылка на SO к которому прикреплён данный компонент
	};

	// ---------------------------------------- Реализация ----------------------------------------
	// Конструктор по умолчанию
	inline Component::Component()
	{ m_scene_object.reset(); }

	// Возвращение трансформа связанного с объектом сцены у текущего компонента
	inline shared_ptr<Transform> Component::GetTransform()
	{
		return m_scene_object->GetTransform();
	}
	// Возвращение компонента типа get_type_component, если он добавлен к SO и empty, если не был добавлен
	template<class get_type_component>
	inline shared_ptr<get_type_component> Component::GetComponent()
	{ return m_scene_object->GetComponent<get_type_component>(); }
}