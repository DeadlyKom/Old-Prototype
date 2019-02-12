#pragma once

#include <memory>
#include <combaseapi.h>
#include <string>

using namespace std;

namespace ScriptEngine
{	
	//
	enum class TObject
	{
		Behavior,
		Material,
		Mesh,
		MeshFilter,
		MeshRender,
		Texture2D,
		Transform,
		SceneObject
	};

	// Интерфейс ресурса
	interface IObject
	{
		string		name;					// имя объекта

		int			GetInstanceID();					//
		static void Destroy(shared_ptr<IObject> obj);	//
	
	protected:
		TObject		type;					// тип объекта
		int			instance_id;
	};
}