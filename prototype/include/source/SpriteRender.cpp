#include "prototype\include\SpriteRender.h"

namespace ScriptEngine
{
	// Конструктор по умолчанию
	SpriteRender::SpriteRender()
	{
	}

	// Деструктор
	SpriteRender::~SpriteRender()
	{
	}
	
	// Возвращение ссылки на спрайт
	shared_ptr<Sprite> SpriteRender::GetSprite()
	{
		return m_sprite;
	}

	// Установка ссылки на спрайт
	void SpriteRender::SetSprite(shared_ptr<Sprite> sprite)
	{
		//// Поучим текущую реализацию класса рендера
		//shared_ptr<SystemEngine::RendererImp> current_renderer = m_resource_manadger->GetRenderer(m_index_renderer);
		//// Установим новый индек вершин, находящийся в указанном спрайте
		//current_renderer->SetVertices(m_resource_manadger->GetIndexVerticesSprite(sprite));
		//// Очистим старые данные о обходе треугольников
		//current_renderer->ClearTriangles();
		//// Установим новые данные об обходе треугольников, находящиеся в указанном спрайте
		//current_renderer->AddTriangles(m_resource_manadger->GetIndexTrianglesSprite(sprite));
		//// Сохраним ссылку нового спрайта
		//m_sprite = sprite;
	}
}