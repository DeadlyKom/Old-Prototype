#include "prototype\include\Object.h"
#include "prototype\include\Renderer.h"

#include "prototype\include\MeshFilter.h"

#include "prototype\engine\include\RendererManager.h"
#include "prototype\engine\include\ResourceManager.h"

namespace ScriptEngine
{
	// Конструктор
	Renderer::Renderer()
	{
		m_materials = make_shared<vector<shared_ptr<Material>>>();
	}

	// Деструктор
	Renderer::~Renderer()
	{
		shared_ptr<SystemEngine::ObjectRegistry> object_registry = SystemEngine::ObjectRegistry::GetInstance();

		// Освободим все материалы
		for (auto& it_material : *m_materials)
		{ object_registry->DeleteObject(it_material->GetInstanceID()); }
	}

	// Возвращает ссылку на материал (первый из массива, если их несколько)
	shared_ptr<Material> Renderer::GetMaterial()
	{
		// Проверим на пустоту 
		if (m_materials->empty()) { return nullptr; }
		// Вернём первый материал
		return (*m_materials)[0];
	}

	shared_ptr<vector<shared_ptr<Material>>> Renderer::GetMaterials()
	{
		return m_materials;
	}

	// Установка ссылки материала (по умолчанию в самый первый)
	bool Renderer::SetMaterial(shared_ptr<Material> material)
	{
		//
		if(m_materials->empty())	{ m_materials->push_back(material); }
		else						{ (*m_materials)[0] = material;		}
		// Добавим материал в список рендера
		SystemEngine::RendererManager::GetInstance()->AddMaterial(SystemEngine::ResourceManager::GetInstance()->GetShaderRes(material->GetShader()));

		// 
		shared_ptr<Mesh> mesh = this->GetComponent<MeshFilter>()->GetMesh();
		//
		if (mesh == nullptr) { return false; }
		//
		shared_ptr<SystemEngine::VerticesRes> vertices = SystemEngine::ResourceManager::GetInstance()->GetVerticesRes(mesh);
		//
		vector<shared_ptr<SystemEngine::IndexRes>> list_triangles;
		//
		SystemEngine::ResourceManager::GetInstance()->GetIndexesRes(mesh, list_triangles);

		for (auto& it_index : list_triangles)
		{
			SystemEngine::RendererManager::GetInstance()->AddPrimitive(SystemEngine::ResourceManager::GetInstance()->GetShaderRes(material->GetShader()), 
				SystemEngine::ResourceManager::GetInstance()->GetMaterialPropertyBlockRes(material->GetPropertyBlock()), 
				vertices, it_index, SystemEngine::ResourceManager::GetInstance()->GetTextureRes(material->GetTexture()));
		}

		// Вернём успешность установки материала
		return false;
	}

	// Установка массива материалов (старые освобождаются)
	void Renderer::SetMaterials(shared_ptr<vector<shared_ptr<Material>>> materials)
	{
		m_materials = materials;
		// Добавим массив материалов
		for (auto& it_material : *materials)
		{
			SystemEngine::RendererManager::GetInstance()->AddMaterial(SystemEngine::ResourceManager::GetInstance()->GetShaderRes(it_material->GetShader()));
		}
	}
}