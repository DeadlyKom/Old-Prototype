#pragma once

#include <memory>
#include <vector>

#include "prototype\engine\include\ResourceManager.h"

#include "Component.h"
#include "Material.h"
#include "MaterialPropertyBlock.h"
#include "Type.h"

namespace ScriptEngine
{
	class Renderer : public Component
	{
	public:
		// Конструктор
													Renderer();
		// Деструктор
		virtual										~Renderer();
		// Возвращает ссылку на материал (первый из массива, если их несколько)
		shared_ptr<Material>						GetMaterial();
		// Возвращает ссылку на массив материалов (всех)
		shared_ptr<vector<shared_ptr<Material>>>	GetMaterials();
		// Устанавливает ссылку материала
		bool										SetMaterial(shared_ptr<Material> material);
		// Устанавливает ссылку массива материалов
		void										SetMaterials(shared_ptr<vector<shared_ptr<Material>>> materials);

	protected:
		// Внутренние переменные
		shared_ptr<vector<shared_ptr<Material>>>	m_materials;			// список материалов
	};
}