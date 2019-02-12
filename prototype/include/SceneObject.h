#pragma once

#include <list>
#include <vector>
#include <memory>
#include <string>
#include <typeinfo>

#include "Type.h"
#include "Object.h"

using namespace std;

namespace ScriptEngine
{
	enum class TPrimetive
	{
		Unknown,
		Cube,
		Sphere,
		Tor
	};

	// Илюзия обмана XD
	class Component;
	class Transform;
	// Класс SO наследован для передачи текущего указателя на класс в компонент
	class SceneObject : public IObject
	{
	private:
		// Структура хранения информации о типе компонента
		struct type_info_component
		{
			size_t				hash_code;				// хэш код класса (должен быть уникальным, по отношению к имени класса)
			string				name;					// имя класса
			shared_ptr<void>	ptr;					// ссылка на компонент
		};

	private:
		// Конструктор по умолчанию
												SceneObject();
		// Конструктор копирования
												SceneObject(const SceneObject&&) = delete;
		// Оператор присвоения
		SceneObject&							operator=(const SceneObject&&) = delete;

	public:
		// Создание объекта
		static shared_ptr<SceneObject>			Create(string name, bool empty = true, TPrimetive primitive = TPrimetive::Unknown);
		// Деструктор
		virtual									~SceneObject() {};

		// Добавляет компонент к текущему SO
		template<class type_class>
		shared_ptr<type_class>								AddComponent(string name);
		// Возвращение компонента типа get_type_component, если он добавлен к SO и empty, если не был добавлен
		template<class get_type_component>
		shared_ptr<get_type_component>						GetComponent();
		// Возвращение списка компонентов типа get_type_component, если он добавлен к SO и empty, если не был добавлен
		template<class get_type_component>
		shared_ptr<vector<shared_ptr<get_type_component>>>	GetComponents();
		// Возвращение трансформа текущего объекта сцены 
		shared_ptr<Transform>								GetTransform();
		// Установка трансформа текущего объекта сцены
		void												SetTransform(shared_ptr<Transform> transform);

	private:
		// Установливает значение текущего объекта сцены в компонент
		void												SetSceneObject(shared_ptr<Component> component);

		// Внутренние переменные
		shared_ptr<Transform>								m_transform;

		list<shared_ptr<type_info_component>>				m_list_components;		// список всех компонентов прикреплённых к SO
	}; 
	//std::shared_ptr<std::shared_ptr<ScriptEngine::Component>>
	//                std::shared_ptr<ScriptEngine::Component>
	// Добавляет компонент к текущему SO
	template<class type_component>
	inline shared_ptr<type_component> SceneObject::AddComponent(string name)
	{
		// Создание нового компонента с требуемым типом
		shared_ptr<type_component> new_component = type_component::Create(name);//make_shared<type_component>();
		// Сохраним указатель на текущий SO в новосозданном компаненте
		SetSceneObject(static_pointer_cast<Component>(new_component));
		// Создадим структуру хранения информации о добавленом компоненте
		shared_ptr<type_info_component> new_tic = make_shared<type_info_component>();
		// Заполним её именем компанента (имя класса)
		new_tic->name = typeid(type_component).name();
		// Заполним её хэш кодом компонента
		new_tic->hash_code = typeid(type_component).hash_code();
		// И сохраним ссылку на него
		new_tic->ptr = static_pointer_cast<void>(new_component);
		// Добавляем в общий список компонентов прикреплённых к текущему SO
		m_list_components.push_back(new_tic);
		// Вернём новй компонент пользователю
		return new_component;
	}
	// Возвращение компонента типа get_type_component, если он добавлен и empty, если не был добавлен
	template<class get_type_component>
	inline shared_ptr<get_type_component> SceneObject::GetComponent()
	{
		// Инициализация возвращаемого компонента (пустой - empty)
		shared_ptr<get_type_component> result_find;
		// Определим искомое имя компонента
		const string find_name = typeid(get_type_component).name();
		// Определим искомый хэш код компонента
		const size_t find_hash_code = typeid(get_type_component).hash_code();
		// Организуем цикл по всем значениям спаска
		for (auto& it_list : m_list_components)
		{
			// Определим текущий хэш код в списке
			const size_t curent_hash = (*it_list).hash_code;
			// Произведём сравнение, если они одинаковы - мы нашли что искали
			if (curent_hash == find_hash_code)
			{
				// Преобразуем сохранённый ранее указатель на компонент из void* в требуемый
				result_find = static_pointer_cast<get_type_component>((*it_list).ptr);
				// Остановка поиска
				break;
			}
		}
		// Вернём результат поиска
		return result_find;
	}
	// Возвращение списка компонентов типа get_type_component, если он добавлен к SO и empty, если не был добавлен
	template<class get_type_component>
	inline shared_ptr<vector<shared_ptr<get_type_component>>> SceneObject::GetComponents()
	{
		shared_ptr<vector<shared_ptr<get_type_component>>> result_find = make_shared<vector<shared_ptr<get_type_component>>>();

		// Определим искомое имя компонента
		const string find_name = typeid(get_type_component).name();
		// Определим искомый хэш код компонента
		const size_t find_hash_code = typeid(get_type_component).hash_code();
		// Организуем цикл по всем значениям спаска
		for (auto& it_list : m_list_components)
		{
			// Определим текущий хэш код в списке
			const size_t curent_hash = (*it_list).hash_code;
			// Произведём сравнение, если они одинаковы - мы нашли что искали
			if (curent_hash == find_hash_code)
			{
				// Преобразуем сохранённый ранее указатель на компонент из void* в требуемый
				shared_ptr<get_type_component> current = static_pointer_cast<get_type_component>((*it_list).ptr);
				// Добавим в список найденых компонентов
				result_find->push_back(current);
			}
		}
		// Вернём результат поиска
		return result_find;
	}
}