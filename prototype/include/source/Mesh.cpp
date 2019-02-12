#include "prototype\include\Mesh.h"

#include "prototype\engine\include\IndexRes.h"
#include "prototype\engine\include\ObjectRegistry.h"
#include "prototype\engine\include\VerticesRes.h"

namespace ScriptEngine
{
	// Конструктор
	Mesh::Mesh(string name)
	{
		IObject::name = NAME_MESH + name;

		IObject::type = TObject::Mesh;
		//
		m_triangles = make_shared<vector<shared_ptr<vector<uint16>>>>();
		//
		m_vertices = make_shared<vector<Vector3>>();
		//
		m_normals = make_shared<vector<Vector3>>();
		//
		m_uvs = make_shared<array<shared_ptr<vector<Vector2>>, 4>>();
		//
		for (auto& it_uv : *m_uvs)
		{
			it_uv = make_shared<vector<Vector2>>();
		}
		//
		SetSubMeshCount(1);
		//
		m_index_vertices_res = SystemEngine::ResourceManager::GetInstance()->CreateResource<SystemEngine::VerticesRes>(IObject::name + NAME_VERTICES, SystemEngine::TResource::Vertices);
	}

	// Создание объекта
	shared_ptr<Mesh> Mesh::Create(string name)
	{
		shared_ptr<Mesh> new_mesh(new Mesh(name));

		new_mesh->instance_id = SystemEngine::ObjectRegistry::GetInstance()->RegistrationObject(new_mesh);

		return new_mesh;
	}

	// Деструктор
	Mesh::~Mesh()
	{
	}

	// Возвращает количество подмешей
	int Mesh::GetSubMeshCount()
	{
		return m_sub_mesh_count;
	}

	// Устанавливает количество подмешей
	void Mesh::SetSubMeshCount(int sub_mesh_count)
	{
		//
		m_sub_mesh_count = sub_mesh_count;
		//
		m_triangles->resize(sub_mesh_count);
		//
		m_list_index_triangles_res.resize(sub_mesh_count, UNKNOW_INDEX_ELEMENTS);
	}

	// Устанавливает массив индексов (обхода треугольников) в указанный индекс
	bool Mesh::SetTriangles(vector<uint16>& triangles, int submesh)
	{
		// Проверим на превышение размерности массива
		if ((int)m_triangles->size() <= submesh) { return true; }
		//
		if ((*m_triangles)[submesh] == nullptr)
		//
		{ (*m_triangles)[submesh] = make_shared<vector<uint16>>(); }
		//
		(*m_triangles)[submesh]->resize(triangles.size());
		//
		copy(triangles.begin(), triangles.end(), (*m_triangles)[submesh]->begin());
		//
		if (m_list_index_triangles_res[submesh] == UNKNOW_INDEX_ELEMENTS)
		{
			m_list_index_triangles_res[submesh] = SystemEngine::ResourceManager::GetInstance()->CreateResource<SystemEngine::IndexRes>(IObject::name + NAME_INDEXES + to_string(m_list_index_triangles_res.size() - 1));
		}
		//
		SystemEngine::ResourceManager::GetInstance()->GetResource<SystemEngine::IndexRes>(m_list_index_triangles_res[submesh])->SetTriangles((*m_triangles)[submesh]);
		// Успешный выход
		return false;
	}

	// Устанавливает массив вершин
	void Mesh::SetVertices(vector<Vector3>& vertices)
	{
		//
		m_vertices->resize(vertices.size());
		//
		copy(vertices.begin(), vertices.end(), m_vertices->begin());
		//
		SystemEngine::ResourceManager::GetInstance()->GetResource<SystemEngine::VerticesRes>(m_index_vertices_res)->SetVertices(m_vertices);
	}

	// Устанавливает массив нормалей
	void Mesh::SetNormals(vector<Vector3>& normals)
	{
		m_normals->resize(normals.size());
		//
		copy(normals.begin(), normals.end(), m_normals->begin());
		//
		SystemEngine::ResourceManager::GetInstance()->GetResource<SystemEngine::VerticesRes>(m_index_vertices_res)->SetNormals(m_normals);
	}

	// Устанавливает массив UV координат в указанный индекс
	bool Mesh::SetUVs(int channel, vector<Vector2>& uvs)
	{
		// Проверим на превышение размерности массива
		if (channel > 3 ) { return true; }
		//
		(*m_uvs)[channel]->resize(uvs.size());
		//
		copy(uvs.begin(), uvs.end(), (*m_uvs)[channel]->begin());
		//
		SystemEngine::ResourceManager::GetInstance()->GetResource<SystemEngine::VerticesRes>(m_index_vertices_res)->SetUVs(channel, (*m_uvs)[channel]);
		// Успешный выход
		return false;
	}

	// Устанавливает массив 32 битных цветов
	void Mesh::SetColor(shared_ptr<vector<Color32>> colors)
	{
		// Установим массив
		m_colors = colors;
		//
		SystemEngine::ResourceManager::GetInstance()->GetResource<SystemEngine::VerticesRes>(m_index_vertices_res)->SetColor(colors);
	}

	// Очистка массивов (по умолчанию все)
	void Mesh::Clear()
	{
		m_vertices.reset();
		m_normals.reset();
		(*m_uvs)[0].reset();
		(*m_uvs)[1].reset();
		(*m_uvs)[2].reset();
		(*m_uvs)[3].reset();
		m_colors.reset();
	}

	// Применить все значения (сформировать массив вершин, заполнить его значениями и передать в Видео Память GPU), по умолчанию формируется массив только из вершин
	void Mesh::Apply()
	{
		SystemEngine::ResourceManager::GetInstance()->GetResource<SystemEngine::VerticesRes>(m_index_vertices_res)->Release();
		for(auto& it_triangles : m_list_index_triangles_res)
		{ SystemEngine::ResourceManager::GetInstance()->GetResource<SystemEngine::IndexRes>(it_triangles)->Release(); }
		
		SystemEngine::ResourceManager::GetInstance()->GetResource<SystemEngine::VerticesRes>(m_index_vertices_res)->Reload();
		for (auto& it_triangles : m_list_index_triangles_res)
		{ SystemEngine::ResourceManager::GetInstance()->GetResource<SystemEngine::IndexRes>(it_triangles)->Reload(); }
	}
}