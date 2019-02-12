#pragma once

#include "prototype\include\Color.h"
#include "prototype\include\Component.h"
#include "prototype\include\Type.h"
#include "prototype\include\Texture.h"
#include "prototype\include\Vector.h"

#include "prototype\engine\include\ResourceManager.h"

using namespace AddStruct;

namespace ScriptEngine
{
	using namespace std;

	class Sprite
	{
	public:
		//
		enum DataTypesSprite
		{
			Vertices	= 0x00000001,			// вершины (всегда присутствуют)
			Normals		= 0x00000010,			// флаг использования нормалей
			UVs_0		= 0x00000100,			// флаг использования текстурных координат 0 массив
			Colors		= 0x01000000,			// флаг использования цвета вершин
			All			= 0x11111111			// все флаги включены
		};

	public:
		// Конструктор по умолчанию
											Sprite();
		// Деструктор
		virtual								~Sprite();

		// Установка текстуры
		void								SetTexture(const shared_ptr<Texture2D> texture)				{ m_texture = texture; }
		// Установка массива обхода треугольников
		void								SetTriangles(const shared_ptr<vector<uint16>> triangles)	{ m_triangles = triangles; }
		// Установка массива вершин
		void								SetVertices(const shared_ptr<vector<Vector3>> vertices)		{ m_vertices = vertices; }
		// Установка массива нормалей
		void								SetNormals(const shared_ptr<vector<Vector3>> normals)		{ m_normals = normals; }
		// Установка массива UV координат текстуры
		void								SetUV(const shared_ptr<vector<Vector2>> uvs)				{ m_uvs = uvs; }
		// Получения текстуры
		const shared_ptr<Texture2D>			GetTexture()												{ return m_texture; }
		// Получение массива обхода треугольников
		const shared_ptr<vector<uint16>>	GetTriangles()												{ return m_triangles; }
		// Получение массива вершин
		const shared_ptr<vector<Vector3>>	GetVertices()												{ return m_vertices; }
		// Получение массива нормалей
		const shared_ptr<vector<Vector3>>	GetNormals()												{ return m_normals; }
		// Получение массива UV координат текстуры
		const shared_ptr<vector<Vector2>>	GetUV()														{ return m_uvs; }
		// Применить все значения (сформировать массив вершин, заполнить его значениями и передать в Видео Память GPU),
		//  по умолчанию формируется массив только из вершин и текстур
		bool								Apply(DataTypesSprite data_types = (DataTypesSprite)(DataTypesSprite::Vertices | DataTypesSprite::UVs_0));

		//friend int							SystemEngine::ResourceManager::GetIndexVerticesSprite(shared_ptr<Sprite> sprite);
		//friend int							SystemEngine::ResourceManager::GetIndexTrianglesSprite(shared_ptr<ScriptEngine::Sprite> sprite);

	protected:
		// Системные переменные
		shared_ptr<SystemEngine::ResourceManager>	m_resource_manadger;	// ссылка на устройство отображения

		// Внутренние переменные
		shared_ptr<Texture2D>						m_texture;				// ссылка на текстуру
		shared_ptr<vector<uint16>>					m_triangles;			// ссылка на массив индексов (обхода треугольников)
		shared_ptr<vector<Vector3>>					m_vertices;				// ссылка на массив вершин
		shared_ptr<vector<Vector3>>					m_normals;				// ссылка на массив нормалей
		shared_ptr<vector<Vector2>>					m_uvs;					// ссылка на массив текстурных координат
		shared_ptr<vector<Color32>>					m_colors;

		int											m_index_triangles;		// индекс в общем списке индексов
		int											m_index_vertices;		// индекс в общем списке вершин

		// Внутренние методы
		// Инициализация значений по умолчанию индексов (обхода треугольников)
		void								TriangleDefault();
		// Инициализация значений по умолчанию вершин
		void								VerticesDefault();
		// Инициализация значений по умолчанию нормалей
		void								NormalsDefault();
		// Инициализация значений по умолчанию текстурных координат
		void								UVsDefault();
		// Инициализация значений по умолчанию цвета вершин
		void								ColorsDefault();
	};
}