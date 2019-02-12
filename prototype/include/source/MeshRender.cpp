#include "prototype\include\MeshRender.h"

#include "prototype\engine\include\ObjectRegistry.h"

namespace ScriptEngine
{
	// Конструктор
	MeshRender::MeshRender()
	{
	}

	// Создание объекта
	shared_ptr<MeshRender> MeshRender::Create(string name)
	{
		shared_ptr<MeshRender> new_mesh_render(new MeshRender);

		new_mesh_render->name = NAME_MESH_RENDER + name;

		new_mesh_render->type = TObject::MeshRender;

		new_mesh_render->instance_id = SystemEngine::ObjectRegistry::GetInstance()->RegistrationObject(new_mesh_render);

		return new_mesh_render;
	}

	// Деструктор
	MeshRender::~MeshRender()
	{
	}
}