#pragma once

#include <memory>

#include "prototype\include\Renderer.h"
#include "prototype\include\Sprite.h"


namespace ScriptEngine
{
	class SpriteRender : public Renderer
	{
	public:
		// Конструктор по умолчанию
									SpriteRender();
		// Деструктор
		virtual						~SpriteRender();
		// Возвращение ссылки на спрайт
		shared_ptr<Sprite>			GetSprite();
		// Установка ссылки на спрайт
		void						SetSprite(shared_ptr<Sprite> sprite);

	protected:
		shared_ptr<Sprite>			m_sprite;				// ссылка на спрайт
	};
}