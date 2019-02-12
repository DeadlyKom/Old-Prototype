#include "prototype\include\Texture.h"

#include "prototype\engine\include\ObjectRegistry.h"
#include "prototype\engine\include\ResourceManager.h"
#include "prototype\engine\include\TextureRes.h"

namespace ScriptEngine
{
	// Конструктор
	Texture::Texture()
	{
	}

	// Деструктор
	Texture::~Texture()
	{
	}

	// -----------------------------------------------------

	// Конструктор по умолчанию
	Texture2D::Texture2D(string name)
	{
		IObject::name = NAME_TEXTURE_2D + name;

		type = TObject::Texture2D;

		m_index_texture_res = SystemEngine::ResourceManager::GetInstance()->FindResource(name);
		//
		//m_index_texture_res = SystemEngine::ResourceManager::GetInstance()->CreateResource<SystemEngine::TextureRes>(IObject::name, SystemEngine::TResource::Texture);
	}

	// Создание объекта
	shared_ptr<Texture2D> Texture2D::Create(string name)
	{
		shared_ptr<Texture2D> new_texture(new Texture2D(name));

		new_texture->instance_id = SystemEngine::ObjectRegistry::GetInstance()->RegistrationObject(new_texture);

		return new_texture;
	}

	// Деструктор
	Texture2D::~Texture2D()
	{
	}

}