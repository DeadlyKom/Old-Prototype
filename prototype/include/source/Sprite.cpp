#include "prototype\include\Sprite.h"
#include "prototype\engine\include\VerticesRes.h"
#include "prototype\engine\include\IndexRes.h"

namespace ScriptEngine
{
	// Конструктор по умолчанию
	Sprite::Sprite()
	{
		//m_resource_manadger = SystemEngine::ResourceManager::GetInstance();

		//m_index_vertices	= UNKNOW_INDEX_ELEMENTS;
		//m_index_triangles	= UNKNOW_INDEX_ELEMENTS;
	}

	// Деструктор
	Sprite::~Sprite()
	{
		//m_resource_manadger->DeleteVertices(m_index_vertices);

		//m_resource_manadger->DeleteTriangles(m_index_triangles);

		//m_resource_manadger.reset();
	}

	// Применить все значения (сформировать массив вершин, заполнить его значениями и передать в Видео Память GPU),
	//  по умолчанию формируется массив только из вершин и текстур
	bool Sprite::Apply(DataTypesSprite data_types)
	{
		////
		//if (m_index_triangles == UNKNOW_INDEX_ELEMENTS) { m_resource_manadger->DeleteTriangles(m_index_triangles);	m_index_triangles	= UNKNOW_INDEX_ELEMENTS; }
		////
		//if (m_index_vertices == UNKNOW_INDEX_ELEMENTS)	{ m_resource_manadger->DeleteVertices(m_index_vertices);	m_index_vertices	= UNKNOW_INDEX_ELEMENTS; }
		//// Получим реализацию вершинного класса
		//shared_ptr<SystemEngine::VerticesRes> current_vertices = make_shared<SystemEngine::VerticesRes>();
		////
		//if (m_triangles->empty())	{ TriangleDefault(); }
		////
		//shared_ptr<SystemEngine::IndexRes> new_triangles = make_shared<SystemEngine::IndexRes>();
		////
		//new_triangles->CreateIndexBuffer(m_triangles);
		////
		//m_index_triangles = m_resource_manadger->AddTriangles(new_triangles);
		////
		//int count_vertices = 0;
		////
		//if (m_vertices->empty())	{ VerticesDefault(); }
		////
		//current_vertices->AddElementVertexBuffer(sizeof(Vector3), m_vertices->size(), shared_ptr<float>((float*)m_vertices->data()));
		////
		//count_vertices = m_vertices->size();
		////
		//if ((data_types & DataTypesSprite::Normals) == DataTypesSprite::Normals)
		//{
		//	if (m_normals->size() != count_vertices)	{ NormalsDefault(); }
		//	
		//	current_vertices->AddElementVertexBuffer(sizeof(Vector3), m_normals->size(), shared_ptr<float>((float*)m_normals->data()));
		//}
		////
		//if ((data_types & DataTypesSprite::UVs_0) == DataTypesSprite::UVs_0)
		//{
		//	if (m_uvs->size() != count_vertices)		{ UVsDefault(); }

		//	current_vertices->AddElementVertexBuffer(sizeof(Vector2), m_normals->size(), shared_ptr<float>((float*)m_uvs->data()));
		//}
		////
		//if ((data_types & DataTypesSprite::Colors) == DataTypesSprite::Colors)
		//{
		//	if (m_colors->size() != count_vertices)		{ ColorsDefault(); }
		//	
		//	vector<float> float_colors;
		//	//
		//	for (auto& it_color : *m_colors)
		//	{
		//		Vector4 color = it_color.GetFloat();
		//		float_colors.push_back(color.x);
		//		float_colors.push_back(color.y);
		//		float_colors.push_back(color.z);
		//		float_colors.push_back(color.w);
		//	}
		//	//
		//	current_vertices->AddElementVertexBuffer(sizeof(float), float_colors.size(), shared_ptr<float>(float_colors.data()));
		//}
		////
		//current_vertices->CreateVertexBuffer(count_vertices);
		////
		//m_index_vertices = m_resource_manadger->AddVertices(current_vertices);
		//
		return false;
	}

	// Инициализация значений по умолчанию индексов (обхода треугольников)
	void Sprite::TriangleDefault()
	{
	}

	// Инициализация значений по умолчанию вершин
	void Sprite::VerticesDefault()
	{
	}

	// Инициализация значений по умолчанию нормалей
	void Sprite::NormalsDefault()
	{
	}

	// Инициализация значений по умолчанию текстурных координат
	void Sprite::UVsDefault()
	{
	}

	// Инициализация значений по умолчанию цвета вершин
	void Sprite::ColorsDefault()
	{
	}
}