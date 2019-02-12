#include "prototype\include\Material.h"

#include "prototype\engine\include\ShaderRes.h"

namespace ScriptEngine
{
	// Конструктор
	Material::Material(string name)
	{
		IObject::name = NAME_MATERIAL + name;

		type = TObject::Material;

		m_property_block = make_shared<PropertyBlock>(IObject::name);
	}

	// Создание объекта
	shared_ptr<Material> Material::Create(string name)
	{
		shared_ptr<Material> new_material(new Material(name));

		new_material->instance_id = SystemEngine::ObjectRegistry::GetInstance()->RegistrationObject(new_material);

		return new_material;
	}

	// Деструктор
	Material::~Material()
	{
	}

	// Возвращает ссылку на шейдер
	shared_ptr<Shader> Material::GetShader()
	{
		return m_shader;
	}

	// Возвращает ссылку на текстуру
	shared_ptr<Texture> Material::GetTexture()
	{
		return m_texture;
	}

	// Возвращает ссылку на блок свойств материала
	shared_ptr<PropertyBlock> Material::GetPropertyBlock()
	{
		return m_property_block;
	}

	// Устанавливает ссылку на шейдер
	void Material::SetShader(shared_ptr<Shader> shader)
	{
		m_shader = shader;
	}

	// Установливает ссылку на текстуру
	void Material::SetTexture(shared_ptr<Texture> texture)
	{
		m_texture = texture;
	}

	// Устанавливает ссылку на блок свойств материала
	void Material::SetPropertyBlock(shared_ptr<PropertyBlock> property_block)
	{
		m_property_block = property_block;
	}

	//
	void Material::SetMatrix(string name, Matrix4X4& matrix)
	{
		m_property_block->SetMatrix(name, matrix);
	}

	//
	void Material::SetMatrixs(string name, vector<Matrix4X4>& matrixs)
	{
		m_property_block->SetMatrixs(name, matrixs);
	}
}