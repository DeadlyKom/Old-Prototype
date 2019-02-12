#include "prototype\include\Material.h"
#include "prototype\include\Mesh.h"
#include "prototype\include\MaterialPropertyBlock.h"
#include "prototype\include\Renderer.h"
#include "prototype\include\Shader.h"
#include "prototype\include\Sprite.h"

#include "..\Device.h"
#include "..\IndexRes.h"
#include "..\MaterialPropertyBlockRes.h"
#include "..\ResourceManager.h"
#include "..\ShaderRes.h"
#include "..\TextureRes.h"
#include "..\VerticesRes.h"

namespace SystemEngine
{
	// Конструктор
	ResourceManager::ResourceManager()
	{
		m_device = nullptr;
	}

	// Деструктор
	ResourceManager::~ResourceManager()
	{
	}

	// Возвращает реализацию данного класса
	shared_ptr<ResourceManager> ResourceManager::GetInstance()
	{
		static shared_ptr<ResourceManager>	resource_manager_singleton(new ResourceManager());/* = make_shared<Device>();*/

		return resource_manager_singleton;
	}

	// Возвращает ссылку на интерфейс устройства
	shared_ptr<Device> ResourceManager::GetDevice(string name)
	{
		if (m_device == nullptr) { m_device = this->GetResource<Device>(this->FindResource(name)); }

		return m_device;
	}

	//// -------------------- Отображение (рендер) --------------------
	//// Добавление рендер к списку устройству отображения
	//int ResourceManager::AddRenderer(shared_ptr<RendererImp> renderer)
	//{
	//	int index = FindEmpty<vector<shared_ptr<RendererImp>>>(m_list_renderer);

	//	if (index = -1) { return index; }

	//	m_list_renderer[index] = renderer;

	//	return index;
	//}

	//// Удаление рендер из списка устройств отображения
	//void ResourceManager::DeleteRenderer(int index)
	//{
	//	// Проверим на превышение индекса
	//	if ((int)m_list_renderer.size() <= index) { return; }
	//	// Освободим шейдер по указанному индексу
	//	m_list_renderer[index].reset();
	//}
	//
	//// Возвращает рендер из списка по индексу
	//shared_ptr<RendererImp> ResourceManager::GetRenderer(int index)
	//{
	//	// Проверим на превышение индекса, если нет вернём значение
	//	if ((int)m_list_shaders.size() > index) { return m_list_renderer[index]; }
	//	// Вернём пустышку
	//	return nullptr;
	//}

	////// Возвращает индекс рендера из списка (взятие приватной переменной)
	////int ResourceManager::GetIndexRenderer(shared_ptr<ScriptEngine::Renderer> renderer)
	////{
	////	return renderer->m_index_renderer;
	////}

	//// -------------------- Шейдеры --------------------
	//// Добавление шейдера к списку устройству отображения
	//int ResourceManager::AddShader(shared_ptr<ShaderRes> shader)
	//{
	//	int return_index = -1;
	//	// Проверим на пустоту списка шейдеров
	//	if(m_list_shaders.empty())
	//	{
	//		// Добавим шейдер в конец списка
	//		m_list_shaders.push_back(shader);
	//		// Укажем его индекс в списке шейдеров
	//		return_index = m_list_shaders.size() - 1;
	//	}
	//	// Список шейдеров не пустой
	//	else
	//	{
	//		// Инициализация позиции
	//		int current_position = 0;
	//		// По умолчанию список шейдеров полный
	//		bool full_list_shader = true;
	//		// Пройдёмся по всему списку, с целью найти пустое место
	//		for (auto& it_shader : m_list_shaders)
	//		{
	//			// Проверим текущую позицию на пустоту
	//			if (it_shader == nullptr)
	//			{
	//				// В текущую позицию сохраним новый шейдер
	//				m_list_shaders[current_position] = shader;
	//				// Укажем что список шейдеров был не полным
	//				full_list_shader = false;
	//				// Укажем индек добавленного шейдера
	//				return_index = current_position;
	//				// Остановка поиска в списке
	//				break;
	//			}
	//			// Увеличим позицию индекса
	//			current_position++;
	//		}
	//		// Провери на полноту списка шейдеров, если он полный добавим новый шейдер в конец
	//		if (full_list_shader)
	//		{
	//			// Добавление нового шейдера в конец списка
	//			m_list_shaders.push_back(shader);
	//			// Укажем индекс добавленного шейдера
	//			return_index = m_list_shaders.size() - 1;
	//		}
	//	}
	//	// Вернём индекс добавленного шейдера
	//	return return_index;
	//}

	//// Удаление шейдера из списка устройства отображения
	//void ResourceManager::DeleteShader(int index)
	//{
	//	// Проверим на превышение индекса
	//	if ((int)m_list_shaders.size() <= index) { return; }
	//	// Освободим шейдер по указанному индексу
	//	m_list_shaders[index].reset();
	//}

	// Поиск шейдера из списка по имени
	int ResourceManager::FindShader(string name)
	{
		// Успешность поиска, не удачна
		int return_find = UNKNOW_INDEX_ELEMENTS;
		// Инициализация позиции поиска
		int current_position = 0;
		// Пройдём по всем шейдерам в списке
		for (auto& it_resource : m_list_resources)
		{
			// Увеличим позицию индекса
			current_position++;
			//
			if (it_resource == nullptr) { continue; }
			//
			if (it_resource->type != TResource::Shader) { continue; }
			// Сравним имена наших шедёров, если они равны укажим позицию и окончим поиск
			if (it_resource->name.compare(name) == 0) { return_find = current_position - 1; break; }
		}
		// Вернём результат поиска
		return return_find;
	}

	//// Возвращает шейдер из списка по индексу
	//shared_ptr<ShaderRes> ResourceManager::GetShader(int index)
	//{
	//	// Проверим на превышение индекса, если нет вернём значение
	//	if ((int)(m_list_shaders.size()) > index) { return m_list_shaders[index]; }
	//	// Вернём пустышку
	//	return nullptr;
	//}

	////// Возвращает индекс шейдера из списка (взятие приватной переменной)
	////int ResourceManager::GetIndexShader(shared_ptr<ScriptEngine::Shader> shader)
	////{
	////	return shader->m_index_res;
	////}
	//
	//// -------------------- Материалы --------------------
	//// Добавление материала к списку устройству отображения
	//int ResourceManager::AddMaterial(shared_ptr<MaterialImp> material)
	//{
	//	return 0;
	//}

	//// Удаление материала из списка устройств отображения
	//void ResourceManager::DeleteMaterial(int index)
	//{
	//}

	//// Возвращает материал из списка по индексу
	//shared_ptr<MaterialImp> ResourceManager::GetMaterial(int index)
	//{
	//	return shared_ptr<MaterialImp>();
	//}
	//
	//// -------------------- Материалы (свойства) --------------------
	//// Добавление свойства материала к списку устройству отображения
	//int ResourceManager::AddMaterialPropertyBlock(shared_ptr<PropertyBlockRes> material_property)
	//{
	//	return 0;
	//}

	//// Удаление свойства материала из списка устройств отображения
	//void ResourceManager::DeleteMaterialPropertyBlock(int index)
	//{
	//	// Проверим на превышение индекса
	//	if ((int)m_list_material_propertys.size() <= index) { return; }
	//	// Освободим шейдер по указанному индексу
	//	m_list_material_propertys[index].reset();
	//}

	//// Возвращает свойства материал из списка по индексу
	//shared_ptr<PropertyBlockRes> ResourceManager::GetMaterialPropertyBlock(int index)
	//{
	//	return shared_ptr<PropertyBlockRes>();
	//}

	////// Возвращает индекс свойства материала из списка (взятие приватной переменной)
	////int ResourceManager::GetIndexMaterialPropertyBlock(shared_ptr<ScriptEngine::PropertyBlock> material_property)
	////{
	////	return material_property->m_index_property_block;
	////}

	//// -------------------- Текстуры --------------------
	//// Добавление текстуры к списку устройству отображения
	//int ResourceManager::AddTexture(shared_ptr<TextureImp> material)
	//{
	//	return 0;
	//}

	//// Удаление текстуры из списка устройств отображения
	//void ResourceManager::DeleteTexture(int index)
	//{
	//}

	//// Возвращает текстуру из списка по индексу
	//shared_ptr<TextureImp> ResourceManager::GetTexture(int index)
	//{
	//	return shared_ptr<TextureImp>();
	//}

	//// -------------------- Меши --------------------
	//// Добавление меша к списку устройству отображения
	//int ResourceManager::AddMesh(shared_ptr<MeshImp> material)
	//{
	//	return 0;
	//}

	//// Удаление меша из списка устройств отображения
	//void ResourceManager::DeleteMesh(int index)
	//{
	//}

	//// Возвращает ссылку меша из списка по индексу
	//shared_ptr<MeshImp> ResourceManager::GetMesh(int index)
	//{
	//	return shared_ptr<MeshImp>();
	//}

	//// -------------------- Массивы вершин --------------------
	//// Добавление вершин к списку устройству отображения
	//int ResourceManager::AddVertices(shared_ptr<VerticesRes> vertices)
	//{
	//	return 0;
	//}

	//// Удаление вершин из списка устройств отображения
	//void ResourceManager::DeleteVertices(int index)
	//{
	//}

	//// Возвращает ссылку вершин из списка по индексу
	//shared_ptr<VerticesRes> ResourceManager::GetVertices(int index)
	//{
	//	return shared_ptr<VerticesRes>();
	//}

	//// Возвращает индекс списка вершин из общего списка у SpriteRender (взятие приватной переменной)
	//int ResourceManager::GetIndexVerticesSprite(shared_ptr<ScriptEngine::Sprite> sprite)
	//{
	//	return sprite->m_index_vertices;
	//}

	//// -------------------- Массивы индексов (обхода треугольников) --------------------
	//// Добавление индексов к списку устройству отображения
	//int ResourceManager::AddTriangles(shared_ptr<IndexRes> vertices)
	//{
	//	return 0;
	//}

	//// Удаление индексов из списка устройств отображения
	//void ResourceManager::DeleteTriangles(int index)
	//{
	//}

	//// Возвращает ссылку индексов из списка по индексу
	//shared_ptr<IndexRes> ResourceManager::GetTriangles(int index)
	//{
	//	return shared_ptr<IndexRes>();
	//}

	//// Возвращает индекс обхода треугольников из общего списка у Sprite (взятие приватной переменной)
	//int ResourceManager::GetIndexTrianglesSprite(shared_ptr<ScriptEngine::Sprite> sprite)
	//{
	//	return sprite->m_index_triangles;
	//}

	// Удаление ресурса из списка
	void ResourceManager::DeleteResource(int index_resource)
	{
		// Проверим на превышение индекса
		if ((int)m_list_resources.size() <= index_resource) { return; }
		// Освободим шейдер по указанному индексу
		m_list_resources[index_resource].reset();
	}

	// Поиск ресурса по имени
	int ResourceManager::FindResource(string& name, TResource type_resource)
	{
		// 
		int find_index = UNKNOW_INDEX_ELEMENTS;
		//
		int positin_index = 0;
		//
		for (auto& it_resource : m_list_resources)
		{
			TResource current_type_resource = (*it_resource).type;

			if((current_type_resource == TResource::All) || (current_type_resource == TResource::Shader))
			{
				if (current_type_resource == type_resource)
				{
					if (name.compare((*it_resource).name) == 0) { find_index = positin_index; break; }
				}
			}
			else
			{
				if (name.compare((*it_resource).name) == 0) { find_index = positin_index; break; }
			}

			positin_index++;
		}
		//
		return find_index;
	}

	// Возвращает ссылку на список рессурсов
	vector<shared_ptr<IResource>>* ResourceManager::GetResources()
	{
		return &m_list_resources;
	}

	// -------------------- Материалы (свойства) --------------------

	// Возвращает ресурс свойства материал из списка
	shared_ptr<PropertyBlockRes> ResourceManager::GetMaterialPropertyBlockRes(shared_ptr<ScriptEngine::PropertyBlock> material_property_block)
	{
		return static_pointer_cast<MatPropBlok>(m_list_resources[material_property_block->m_index_property_block_res]);
	}

	// -------------------- Шейдеры --------------------

	// Возвращает ресурс шейдера из списка
	shared_ptr<ShaderRes> ResourceManager::GetShaderRes(shared_ptr<ScriptEngine::Shader> shader)
	{
		return static_pointer_cast<ShaderRes>(m_list_resources[shader->m_index_res]);
	}

	// -------------------- Меши --------------------

	// Возвращает ссылку ресурса вершин
	shared_ptr<VerticesRes> ResourceManager::GetVerticesRes(shared_ptr<ScriptEngine::Mesh> mesh)
	{
		//if (mesh == nullptr) { return nullptr; }

		return static_pointer_cast<VerticesRes>(m_list_resources[mesh->m_index_vertices_res]);
	}

	// Возвращает список ресурсов индексов(обхода треугольников) из списка
	void ResourceManager::GetIndexesRes(shared_ptr<ScriptEngine::Mesh> mesh, vector<shared_ptr<IndexRes>>& out_list_indexes_resource)
	{
		out_list_indexes_resource.clear();

		for(auto& it_triangles : mesh->m_list_index_triangles_res)
		{ out_list_indexes_resource.push_back(static_pointer_cast<IndexRes>(m_list_resources[it_triangles])); }
	}

	// -------------------- Текстуры --------------------

	// Возвращает ресурс текстуры
	shared_ptr<TextureRes> ResourceManager::GetTextureRes(shared_ptr<ScriptEngine::Texture> texture)
	{
		if (texture == nullptr) { return nullptr; }

		return static_pointer_cast<TextureRes>(m_list_resources[texture->m_index_texture_res]);
	}
}