#include "prototype\include\Component.h"
#include "prototype\include\MeshFilter.h"
#include "prototype\include\MeshRender.h"
#include "prototype\include\Transform.h"
#include "prototype\include\SceneObject.h"

#include "prototype\engine\include\ObjectRegistry.h"
#include "prototype\engine\include\SpherePrim.h"
#include "prototype\engine\include\TorPrim.h"

namespace ScriptEngine
{
	// Конструктор
	SceneObject::SceneObject()
	{
	}

	// Создание объекта
	shared_ptr<SceneObject> SceneObject::Create(string name, bool empty, TPrimetive primitive)
	{	
		shared_ptr<SceneObject> new_scene_object(new SceneObject);

		new_scene_object->name = "SO " + name;

		new_scene_object->type = TObject::SceneObject;

		new_scene_object->instance_id = SystemEngine::ObjectRegistry::GetInstance()->AddSceneObject(new_scene_object);

		if (!empty)
		{
			shared_ptr<MeshFilter> mesh_filter = new_scene_object->AddComponent<MeshFilter>(NAME_DEFAULT);
			new_scene_object->AddComponent<MeshRender>(NAME_DEFAULT);
#			if _DEBUG
			SystemEngine::SpherePrim	prim_sphere	(10, 5);
			SystemEngine::TorPrim		prim_tor	(10, 5);
#			else
			SystemEngine::SpherePrim	prim_sphere(100, 50);
			SystemEngine::TorPrim		prim_tor	(100, 50);
#			endif
			switch (primitive)
			{
			case TPrimetive::Sphere:
				prim_sphere.Create(mesh_filter, Vector3(0, 0, 0), 0.1f);
				break;
			case TPrimetive::Tor:
				prim_tor.Create(mesh_filter, Vector3(0, 0, 0), 0.1f, 0.05f);
				break;
			}
		}

		shared_ptr<Transform> add_new_transform = new_scene_object->AddComponent<Transform>(NAME_DEFAULT);

		new_scene_object->SetTransform(add_new_transform);

		return new_scene_object;
	}

	// Возвращение трансформа текущего объекта сцены
	shared_ptr<Transform> SceneObject::GetTransform()
	{
		return m_transform;
	}

	// Установка трансформа текущего объекта сцены
	void SceneObject::SetTransform(shared_ptr<Transform> transform)
	{
		m_transform = transform;
	}

	// Установливает значение текущего объекта сцены в компонент
	void SceneObject::SetSceneObject(shared_ptr<Component> component)
	{
		component->m_scene_object = SystemEngine::ObjectRegistry::GetInstance()->GetSceneObject(instance_id);
	}
}
