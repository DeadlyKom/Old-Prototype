#include "prototype\engine\include\ObjectRegistry.h"
#include "prototype\include\Behavior.h"

namespace SystemEngine
{
	// Конструктор
	ObjectRegistry::ObjectRegistry()
	{
		m_list_objects_scene.reserve(SIZE_VECTOR_SCENE_OBJECT_DEFAULT);
	}

	// Деструктор
	ObjectRegistry::~ObjectRegistry()
	{
	}

	// Возвращает экземпляр данного класса
	shared_ptr<ObjectRegistry> ObjectRegistry::GetInstance()
	{
		static shared_ptr<ObjectRegistry> object_manager_singleton(new ObjectRegistry());/* = make_shared<Device>();*/

		return object_manager_singleton;
	}

	// Регистрация объекта и возвращение его индекса
	int ObjectRegistry::RegistrationObject(shared_ptr<ScriptEngine::IObject> this_object)
	{
		int index = FindEmpty(m_list_objects);

		m_list_objects[index] = this_object;

		return index;
	}
	
	// Удаление объекта
	void ObjectRegistry::DeleteObject(int index_object)
	{
	}

	// Добавляет объект сцены в список обработки (при укозании позиции производится вставка со сдвигом)
	int ObjectRegistry::AddSceneObject(shared_ptr<ScriptEngine::SceneObject> scene_object/*, int position*/)
	{
		int index = FindEmpty(m_list_objects_scene);

		m_list_objects_scene[index] = scene_object;

		return index;
	}

	// Получение ссылки указанного объекта сцены
	shared_ptr<ScriptEngine::SceneObject> ObjectRegistry::GetSceneObject(int index_scene_object)
	{
		return m_list_objects_scene[index_scene_object];
	}

	// Вернуть список объектов сцены
	vector<shared_ptr<ScriptEngine::SceneObject>>* ObjectRegistry::GetListSceneObjects()
	{
		return &m_list_objects_scene;
	}
}