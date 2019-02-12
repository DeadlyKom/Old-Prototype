#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "prototype\include\Type.h"
#include "prototype\include\Object.h"
#include "prototype\include\SceneObject.h"

using namespace std;

namespace ScriptEngine
{
	class Behavior;
}

namespace SystemEngine
{
#	define SIZE_VECTOR_SCENE_OBJECT_DEFAULT	16					// размер списка объектов сцены по умолчанию
	
	class ObjectRegistry
	{
		private:
		// Конструктор по умолчанию
												ObjectRegistry();
		// Конструктор копирования
												ObjectRegistry(const ObjectRegistry&&) = delete;
		// Оператор присвоения
		ObjectRegistry&							operator=(const ObjectRegistry&&) = delete;

	public:
		// Деструктор
		virtual											~ObjectRegistry();
		// Возвращает экземпляр данного класса
		static shared_ptr<ObjectRegistry>				GetInstance();

		// Регистрация объекта и возвращение его индекса
		int												RegistrationObject(shared_ptr<ScriptEngine::IObject> this_object);
		// Получение ссылки указанного объекта
		template <class get_object>
		shared_ptr<get_object>							GetObject(int index_object);
		// Удаление объекта
		void											DeleteObject(int index_object);
		// Добавляет объект сцены в список обработки (при укозании позиции производится вставка со сдвигом)
		int												AddSceneObject(shared_ptr<ScriptEngine::SceneObject> scene_object/*, int position = UNKNOW_INDEX_ELEMENTS*/);
		// Получение ссылки указанного объекта сцены
		shared_ptr<ScriptEngine::SceneObject>			GetSceneObject(int index_scene_object);
		// Вернуть список объектов сцены
		vector<shared_ptr<ScriptEngine::SceneObject>>*	GetListSceneObjects();

	protected:
		// Поиск свободного элемента в списке
		template<class find>
		int												FindEmpty(find& array_find);

		// Внутренние переменные
		vector<shared_ptr<ScriptEngine::SceneObject>>	m_list_objects_scene;		// список объектов сцены
		vector<shared_ptr<ScriptEngine::IObject>>		m_list_objects;				// список объектов (объекты сцены здесь не хранятся)
	};

	// Получение ссылки указанного объекта
	template<class get_object>
	inline shared_ptr<get_object> ObjectRegistry::GetObject(int index_object)
	{
		return shared_ptr<get_object>();
	}
	// Поиск свободного элемента в списке
	template<class find>
	inline int ObjectRegistry::FindEmpty(find& array_find)
	{
		int return_index = UNKNOW_INDEX_ELEMENTS;
		// Проверим на пустоту списка
		if (array_find.empty())
		{
			// Изменим размер массива
			array_find.push_back(nullptr);
			// Укажем его индекс в списке
			return_index = array_find.size() - 1;
		}
		// Список не пустой
		else
		{
			// Инициализация позиции
			int current_position = 0;
			// По умолчанию список полный
			bool full_list = true;
			// Пройдёмся по всему списку, с целью найти пустое место
			for (auto& it_find : array_find)
			{
				// Проверим текущую позицию на пустоту
				if (it_find == nullptr)
				{
					// Укажем что список был не полным
					full_list = false;
					// Укажем пустой индек
					return_index = current_position;
					// Остановка поиска в списке
					break;
				}
				// Увеличим позицию индекса
				current_position++;
			}
			// Провери на полноту списка, если он полный добавим новый элемент в конец
			if (full_list)
			{
				// Изменим размер
				array_find.push_back(nullptr);
				// Укажем индекс добавленного
				return_index = array_find.size() - 1;
			}
		}
		// Вернём индекс добавленного
		return return_index;
	}
}