#pragma once

#include <map>
#include <memory>
#include <vector>
#include <string>

#include "Type.h"
#include "Vector.h"

#include "Type.h"

#include "prototype\engine\include\ResourceManager.h"

using namespace std;
using namespace AddStruct;

namespace ScriptEngine
{
	class PropertyBlock
	{
	public:
									PropertyBlock() = delete;
		// Конструктор
									PropertyBlock(string name);
		// Деструктор
		virtual						~PropertyBlock();

		// Добавление нового имени
		int							AddKeyWorlds(string name);
		// Очистка всех значений у свойств материала
		void						Clear();
		// Возвращает float из блока свойств (поиск по имени)
		float						GetFloat(string name);
		// Возвращает float из блока свойств (поиск по идентификационному номеру)
		float						GetFloat(int id);
		// Возвращает вектор из блока свойств (поиск по имени)
		Vector3						GetVector(string name);
		// Возвращает вектор из блока свойств (поиск по идентификационному номеру)
		Vector3						GetVector(int id);
		// Возвращает матрицу 4х4 из блока свойств (поиск по имени)
		Matrix4X4					GetMatrix(string name);
		// Возвращает матрицу 4х4 из блока свойств (поиск по идентификационному номеру)
		Matrix4X4					GetMatrix(int id);
		// Устанавливает float в блока свойств (поиск по имени)
		void						SetFloat(string name, float value);
		// Устанавливает float в блока свойств (поиск по идентификационному номеру)
		void						SetFloat(int id, float value);
		// Устанавливает вектор в блока свойств (поиск по имени)
		void						SetVector(string name, Vector3 value);
		// Устанавливает вектор в блока свойств (поиск по идентификационному номеру)
		void						SetVector(int id, Vector3 value);
		// Устанавливает матрицу 4х4 в блока свойств (поиск по имени)
		void						SetMatrix(string name, Matrix4X4 value);
		//
		void						SetMatrixs(string name, vector<Matrix4X4>& value);

		// Устанавливает матрицу 4х4 в блока свойств (поиск по идентификационному номеру)
		void						SetMatrix(int id, Matrix4X4 value);

		friend shared_ptr<SystemEngine::PropertyBlockRes>	SystemEngine::ResourceManager::GetMaterialPropertyBlockRes(shared_ptr<ScriptEngine::PropertyBlock> material_property_block);

	protected:
		// Внутрение переменные
		int							m_index_property_block_res;		//

		int							m_transform;
		int							m_view_projection;

		map<string, int>			m_variable_vs;					// список переменных вершинного шейдера
	};
}