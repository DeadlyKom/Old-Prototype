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

	// ��������� �������
	interface IObject
	{
		string		name;					// ��� �������

		int			GetInstanceID();					//
		static void Destroy(shared_ptr<IObject> obj);	//
	
	protected:
		TObject		type;					// ��� �������
		int			instance_id;
	};
}