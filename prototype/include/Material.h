#pragma once

#include <memory>
#include <string>

#include "MaterialPropertyBlock.h"
#include "Object.h"
#include "Type.h"
#include "Shader.h"
#include "Texture.h"

#include "Type.h"

#include "..\engine\include\ObjectRegistry.h"
#include "..\engine\include\ResourceManager.h"

using namespace std;

namespace ScriptEngine
{
	class Material : public IObject
	{
	private:
											Material() = delete;
		// Конструктор по умолчанию
											Material(string name);
		// Конструктор копирования
											Material(const Material&&) = delete;
		// Оператор присвоения
		Material&							operator=(const Material&&) = delete;

	public:
		// Создание объекта
		static shared_ptr<Material>			Create(string name);
		// Деструктор
		virtual								~Material();
		// Возвращает ссылку на блок свойств материала
		shared_ptr<PropertyBlock>			GetPropertyBlock();
		// Возвращает ссылку на шейдер
		shared_ptr<Shader>					GetShader();
		// Возвращает ссылку на текстуру
		shared_ptr<Texture>					GetTexture();
		//
		void								SetMatrix(string name, Matrix4X4& matrixs);
		//
		void								SetMatrixs(string name, vector<Matrix4X4>& matrixs);
		// Устанавливает ссылку на блок свойств материала
		void								SetPropertyBlock(shared_ptr<PropertyBlock> property_block);		
		// Устанавливает ссылку на шейдер
		void								SetShader(shared_ptr<Shader> shader);
		// Установливает ссылку на текстуру
		void								SetTexture(shared_ptr<Texture> texture);

	protected:		
		// Внутрение переменные
		shared_ptr<Texture>					m_texture;				// ссылка на текстуру
		shared_ptr<PropertyBlock>			m_property_block;		// ссылка на блок сойств материала
		shared_ptr<Shader>					m_shader;				// ссылка на шейдер
	};
}