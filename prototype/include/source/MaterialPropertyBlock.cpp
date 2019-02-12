#include "prototype\include\MaterialPropertyBlock.h"

#include "prototype\engine\include\MaterialPropertyBlockRes.h"
#include "prototype\engine\include\ResourceManager.h"
#include "prototype\engine\include\ShaderRes.h"

namespace ScriptEngine
{
#	define SHADER_VERTEX	"VS"
	// Конструктор
	PropertyBlock::PropertyBlock(string name)
	{
		//
		shared_ptr<SystemEngine::MatPropBlok> property_block = SystemEngine::ResourceManager::GetInstance()->CreateResourcePtr<SystemEngine::MatPropBlok>(NAME_PROPERTY + name, m_index_property_block_res);
		// Позиция
		m_transform = property_block->AddProperty(PB_MATRIX_TRANSFORM, SystemEngine::ShaderRes::TShader::Vertex, SystemEngine::MatPropBlok::TPropertyShader::ChangeEveryFrame, SystemEngine::MatPropBlok::TVariable::Matrix, 1);
		// Матрица вида
		//m_view_projection = property_block->AddProperty(PB_MATRIX_VIEW_PROJECTION, SystemEngine::ShaderRes::TShader::Vertex, SystemEngine::MatPropBlok::TPropertyShader::ChangeEveryFrame, SystemEngine::MatPropBlok::TVariable::Matrix, 2);
	}

	// Деструктор
	PropertyBlock::~PropertyBlock()
	{
		SystemEngine::ResourceManager::GetInstance()->DeleteResource(m_index_property_block_res);
	}

	// Добавление нового имени
	int PropertyBlock::AddKeyWorlds(string name)
	{
		string name_shader, name_variable;
		// Разделим имя от типа шейдера
		for (auto& char_name : name)
		{
			static bool shader_and_variable = false;

			if (char_name == ' ') { shader_and_variable = true; continue; }

			if (shader_and_variable) { name_variable = name.data() + name_shader.size() + 1; }
			else { name_shader += char_name; }
		}

		if (name_shader.compare(SHADER_VERTEX) == 0)
		{
			int variable;

			if (m_variable_vs.find(name_variable) == m_variable_vs.end())
			{
				//property_block->AddProperty(PB_MATRIX_TRANSFORM, SystemEngine::ShaderRes::TShader::Vertex, SystemEngine::MatPropBlok::TPropertyShader::ChangeEveryFrame, SystemEngine::MatPropBlok::TVariable::Matrix, 1);
			}
		}

		return 0;
	}

	// Очистка всех значений у свойств материала
	void PropertyBlock::Clear()
	{
	}

	// Устанавливает матрицу 4х4 в блока свойств (поиск по имени)
	void PropertyBlock::SetMatrix(string name, Matrix4X4 value)
	{
		if (name.compare(PB_MATRIX_TRANSFORM) == 0)
		{
			SystemEngine::ResourceManager::GetInstance()->GetResource<SystemEngine::MatPropBlok>(m_index_property_block_res)->
				SetConstantBufferIndex(SystemEngine::ShaderRes::TShader::Vertex, SystemEngine::PropertyBlockRes::TPropertyShader::ChangeEveryFrame, m_transform, (float*)&value, sizeof(Matrix4X4));
		}

		//
		//if (name_shader.compare(SHADER_VERTEX) == 0)
		//{
		//	auto find = m_variable_vs.find(name_variable);

		//	
		//	//SystemEngine::ResourceManager::GetInstance()->GetResource<SystemEngine::MatPropBlok>(m_index_property_block_res)->
		//	//	SetConstantBufferIndex(SystemEngine::ShaderRes::TShader::Vertex,
		//	//		SystemEngine::PropertyBlockRes::TPropertyShader::ChangeEveryFrame, variable, (float*)&value, sizeof(Matrix4X4));

		//}
	}

	//
	void PropertyBlock::SetMatrixs(string name, vector<Matrix4X4>& value)
	{
		if (name.compare(PB_MATRIX_VIEW_PROJECTION) == 0)
		{
			SystemEngine::ResourceManager::GetInstance()->GetResource<SystemEngine::MatPropBlok>(m_index_property_block_res)->
				SetConstantBufferIndex(SystemEngine::ShaderRes::TShader::Vertex, SystemEngine::PropertyBlockRes::TPropertyShader::ChangeEveryFrame, m_view_projection, (float*)value.data(), sizeof(Matrix4X4)* 2);
		}
	}
}