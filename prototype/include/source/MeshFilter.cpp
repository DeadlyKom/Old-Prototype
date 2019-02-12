#include "prototype\include\MeshFilter.h"

#include "prototype\include\MeshRender.h"

#include "prototype\engine\include\IndexRes.h"
#include "prototype\engine\include\ObjectRegistry.h"
#include "prototype\engine\include\RendererManager.h"
#include "prototype\engine\include\VerticesRes.h"

namespace ScriptEngine
{
	// Конструктор
	MeshFilter::MeshFilter()
	{
	}

	// Создание объекта
	shared_ptr<MeshFilter> MeshFilter::Create(string name)
	{
		shared_ptr<MeshFilter> new_mesh_filter(new MeshFilter);

		new_mesh_filter->name = NAME_MESH_RENDER + name;

		new_mesh_filter->type = TObject::MeshFilter;

		new_mesh_filter->instance_id = SystemEngine::ObjectRegistry::GetInstance()->RegistrationObject(new_mesh_filter);

		return new_mesh_filter;
	}

	// Деструктор
	MeshFilter::~MeshFilter()
	{
	}

	// Возращает ссылку на меш
	shared_ptr<Mesh> MeshFilter::GetMesh()
	{
		return m_mesh;
	}

	// Устанавливает новую ссылку на меш
	void MeshFilter::SetMesh(shared_ptr<Mesh> mesh)
	{
		m_mesh = mesh;
		// Получим список материалов для сопастовления их с обходом треугольников
		shared_ptr<vector<shared_ptr<Material>>> materials = this->GetComponent<MeshRender>()->GetMaterials();
		//
		if (materials->size() == 0) { return; }
		//
		shared_ptr<SystemEngine::VerticesRes> vertices = SystemEngine::ResourceManager::GetInstance()->GetVerticesRes(mesh);
		//
		vector<shared_ptr<SystemEngine::IndexRes>> list_triangles;
		//
		for (auto& it_material : *materials)
		{
			SystemEngine::ResourceManager::GetInstance()->GetIndexesRes(mesh, list_triangles);

			for(auto& it_index : list_triangles)
			{ SystemEngine::RendererManager::GetInstance()->AddPrimitive(SystemEngine::ResourceManager::GetInstance()->GetShaderRes(it_material->GetShader()),
				SystemEngine::ResourceManager::GetInstance()->GetMaterialPropertyBlockRes(it_material->GetPropertyBlock()),
				vertices, it_index, SystemEngine::ResourceManager::GetInstance()->GetTextureRes(it_material->GetTexture())); }
		}
	}
}