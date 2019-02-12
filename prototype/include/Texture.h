#pragma once

#include <memory>
#include <vector>
#include <string>

#include "Color.h"
#include "Object.h"
#include "Type.h"

#include "prototype\engine\include\ResourceManager.h"

using namespace std;
using namespace AddStruct;

namespace ScriptEngine
{
	class Texture : public IObject
	{
	protected:
		// Конструктор по умолчанию
									Texture();
		// Конструктор копирования
									Texture(const Texture&&)	= delete;
		// Оператор присвоения
		Texture&					operator=(const Texture&&)	= delete;

	public:
		// Деструктор
		virtual						~Texture();

		friend shared_ptr<SystemEngine::TextureRes>	SystemEngine::ResourceManager::GetTextureRes(shared_ptr<Texture> texture);

	protected:
		// Внутренние переменные
		int							m_width;
		int							m_height;
		int							m_index_texture_res;
	};

	class Texture2D : public Texture
	{
	protected:
										Texture2D()						= delete;
		// Конструктор по умолчанию
										Texture2D(string name);
		// Конструктор копирования
										Texture2D(const Texture&&)		= delete;
		// Оператор присвоения
		Texture2D&						operator=(const Texture2D&&)	= delete;

	public:
		// Создание объекта
		static shared_ptr<Texture2D>	Create(string name);
		// Деструктор
		virtual							~Texture2D();
	};
}