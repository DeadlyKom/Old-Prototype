#pragma once

#include <array>
#include <memory>
#include <vector>

#include "Color.h"
#include "Object.h"
#include "Type.h"
#include "Vector.h"

#include "prototype\engine\include\ResourceManager.h"

using namespace std;
using namespace AddStruct;

namespace ScriptEngine
{
	class Mesh : public IObject
	{
	private:
									Mesh() = delete;
		// Конструктор по умолчанию
									Mesh(string name);
		// Конструктор копирования
									Mesh(const Mesh&&) = delete;
		// Оператор присвоения
		Mesh&						operator=(const Mesh&&) = delete;

	public:
		// Создание объекта
		static shared_ptr<Mesh>		Create(string name);
		// Деструктор
		virtual						~Mesh();
		// Возвращает количество подмешей
		int							GetSubMeshCount();
		// Возвращает массив индексов (обхода треугольников)
		shared_ptr<vector<uint16>>	GetTriangles(int index_sub_mesh);
		// Возвращает массив вершин
		shared_ptr<vector<Vector3>>	GetVerices();
		// Возвращает массив нормалей
		shared_ptr<vector<Vector3>>	GetNormals();
		// Возвращает массив UV координат
		shared_ptr<vector<Vector2>>	GetUVs(int index_uvs);
		// Возвращает массив 32 битных цветов
		shared_ptr<vector<Color32>> GetColors();
		// Устанавливает количество подмешей
		void						SetSubMeshCount(int sub_mesh_count);
		// Устанавливает массив индексов (обхода треугольников) в указанный индекс
		bool						SetTriangles(vector<uint16>& triangles, int submesh = 0);
		// Устанавливает массив вершин
		void						SetVertices(vector<Vector3>& vertices);
		// Устанавливает массив нормалей
		void						SetNormals(vector<Vector3>& normals);
		// Устанавливает массив UV координат в указанный индекс
		bool						SetUVs(int channel, vector<Vector2>& uvs);
		// Устанавливает массив 32 битных цветов
		void						SetColor(shared_ptr<vector<Color32>> colors);

		// Очистка массивов (по умолчанию все)
		void						Clear();
		// Применить все значения (сформировать массив вершин, заполнить его значениями и передать в Видео Память GPU),
		//  по умолчанию формируется массив только из вершин
		void						Apply();
		
		friend shared_ptr<SystemEngine::VerticesRes>		SystemEngine::ResourceManager::GetVerticesRes(shared_ptr<Mesh> mesh);
		friend void											SystemEngine::ResourceManager::GetIndexesRes(shared_ptr<Mesh> mesh, vector<shared_ptr<IndexRes>>& out_list_indexes_resource);

	protected:
		// Внутренние переменные
		shared_ptr<vector<shared_ptr<vector<uint16>>>>		m_triangles;			// массив индексов (обхода треугольников)
		shared_ptr<vector<Vector3>>							m_vertices;				// массив вершин
		shared_ptr<vector<Vector3>>							m_normals;				// массив нормалей
		shared_ptr<array<shared_ptr<vector<Vector2>>, 4>>	m_uvs;					// массив UV координат
		shared_ptr<vector<Color32>>							m_colors;				// массив 32 битных цветов

		int													m_sub_mesh_count;		// количество сабмешей в текущем меше

		int													m_index_vertices_res;		// индекс на ресурс вершин
		vector<int>											m_list_index_triangles_res;	// список индексов на ресурс обхода треугольников
	};
}