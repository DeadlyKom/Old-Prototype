#pragma once

#include <memory>

#include "prototype\include\Type.h"

#include "G-Buffer.h"
#include "IndexRes.h"
#include "MaterialPropertyBlockRes.h"
#include "ShaderRes.h"
#include "TextureRes.h"
#include "VerticesRes.h"

using namespace std;

namespace SystemEngine
{
	class RendererManager
	{
	private:
		//
		struct sort_material
		{
			struct material_struct
			{
				shared_ptr<ShaderRes>			shader;
			};

			struct primitive_struct
			{
				shared_ptr<VerticesRes>			vertices;
				shared_ptr<IndexRes>			index;
				shared_ptr<PropertyBlockRes>	constants;
				shared_ptr<TextureRes>			texture;
			};

			int							count_uses_material = 0;

			material_struct				material;

			vector<primitive_struct*>	list_primitive;
		};

	private:
		// Конструктор по умолчанию
												RendererManager();
		// Конструктор копирования
												RendererManager(const RendererManager&&)	= delete;
		// Оператор присвоения
		RendererManager&						operator=(const RendererManager&&)			= delete;
	public:
		// Деструктор
		virtual									~RendererManager();
		// Возвращает экземпляр данного класса
		static shared_ptr<RendererManager>		GetInstance();
		// Отрисовка изображения
		void									Render();
		// Обновление облости отображения
		void									UpdateRenderTarget();
		// Добавление материалла в список
		void									AddMaterial(shared_ptr<ShaderRes> shader);
		// Добавление примитива в список
		void									AddPrimitive(shared_ptr<ShaderRes> shader, shared_ptr<PropertyBlockRes> property_shader, shared_ptr<VerticesRes> vertices, shared_ptr<IndexRes> index, shared_ptr<TextureRes> texture);
	
	private:
		// Поиск совпадения шейдера
		int										MatchShaderSearch(shared_ptr<ShaderRes> shader);

		// Внутренние переменные
		vector<sort_material*>					m_sort_materials;
		shared_ptr<GBuffer>						m_Gbuffer;
	};
}