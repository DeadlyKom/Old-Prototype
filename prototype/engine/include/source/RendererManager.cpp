#include "..\RendererManager.h"

namespace SystemEngine
{
	// Конструктор по умолчанию
	RendererManager::RendererManager()
	{
		m_Gbuffer = make_shared<GBuffer>();
	}

	// Деструктор
	RendererManager::~RendererManager()
	{
	}

	// Возвращает экземпляр данного класса
	shared_ptr<RendererManager> RendererManager::GetInstance()
	{
		static shared_ptr<RendererManager>	renderer_manager_singleton(new RendererManager());/* = make_shared<Device>();*/

		return renderer_manager_singleton;
	}

	// Отрисовка изображения
	void RendererManager::Render()
	{
		for (auto& it_material : m_sort_materials)
		{
			if (it_material->count_uses_material == 0) { continue; }

			if (it_material->material.shader->IncludeInputLayout()) { continue; }

			if (it_material->material.shader->Include(ShaderRes::TShader::Vertex)) { continue; }

			if (it_material->material.shader->Include(ShaderRes::TShader::Geometry)) { continue; }

			if (it_material->material.shader->Include(ShaderRes::TShader::Pixel)) { continue; }

			for (auto& it_primitive : it_material->list_primitive)
			{
				if (it_primitive->constants->UpdateChangeEveryFrame(ShaderRes::TShader::Vertex)) { continue; }

				if (it_primitive->constants->IncludeConstantBuffer(ShaderRes::TShader::Vertex, PropertyBlockRes::TPropertyShader::ChangeEveryFrame)) { continue; }

				if (it_primitive->vertices->IncludeVerticesBuffer()) { continue; }

				if (it_primitive->index->IncludeIndexBuffer())		{ continue; }

				if (it_primitive->texture->IncludeShaderResource())	{ continue; }

				if (it_primitive->index->DrawInstanced())			{ continue; }
			}
		}
	}

	// Обновление облости отображения
	void RendererManager::UpdateRenderTarget()
	{
	}

	// Добавление материалла в список
	void RendererManager::AddMaterial(shared_ptr<ShaderRes> shader)
	{
		// Проверим на наличие данного материала
		if (MatchShaderSearch(shader) == UNKNOW_INDEX_ELEMENTS)
		// Сформируем новые данные для добавления в список материалов
		{
			sort_material* add_material = new sort_material();

			add_material->material.shader = shader;
			

			m_sort_materials.push_back(add_material);
		}
	}

	// Добавление примитива в список
	void RendererManager::AddPrimitive(shared_ptr<ShaderRes> shader, shared_ptr<PropertyBlockRes> property_shader, shared_ptr<VerticesRes> vertices, shared_ptr<IndexRes> index, shared_ptr<TextureRes> texture)
	{
		int find_shader = MatchShaderSearch(shader);

		if (find_shader == UNKNOW_INDEX_ELEMENTS) { return; }

		sort_material::primitive_struct* primitive = new sort_material::primitive_struct;

		primitive->vertices		= vertices;

		primitive->index		= index;

		primitive->constants	= property_shader;

		primitive->texture		= texture;

		m_sort_materials.at(find_shader)->list_primitive.push_back(primitive);

		m_sort_materials.at(find_shader)->count_uses_material++;
	}

	// Поиск совпадения шейдера
	int RendererManager::MatchShaderSearch(shared_ptr<ShaderRes> shader)
	{
		int position = 0;

		for (auto& it_material : m_sort_materials)
		{
			if (it_material->material.shader == shader) { return position; }

			position++;
		}

		return UNKNOW_INDEX_ELEMENTS;
	}
}