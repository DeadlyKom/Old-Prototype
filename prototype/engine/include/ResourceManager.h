#pragma once

#include <d3d11_4.h>
#include <memory>
#include <ppltasks.h>
#include <string>
#include <vector>

#include "prototype\include\Type.h"

#include "DeviceEx.h"

using namespace std;
using namespace Concurrency;

namespace ScriptEngine
{
	class Material;
	class Mesh;
	class MeshFilter;
	class MeshRender;
	class PropertyBlock;
	class Renderer;
	class Shader;
	class Sprite;
	class Texture;
}

namespace SystemEngine
{
	class Device;
	class IndexRes;
	class MaterialImp;
	class PropertyBlockRes;
	class MeshImp;
	class RendererImp;
	class ShaderRes;
	class TextureRes;
	class VerticesRes;

	// Перечесление типов ресурсов
	enum class TResource : unsigned int
	{
		Unknown = 0,
		All,
		Camera,
		//Device,
		Index,
		PropertyShader,
		Shared,
		Shader,
		Texture,
		Vertices
	};

	// Интерфейс ресурса
	interface IResource
	{
		virtual task<void>						Reload()	= 0;	// перестроить ресурс
		virtual void							Release()	= 0;	// освободить ресурс

		string									name;								// имя ресурса
		TResource								type		= TResource::Unknown;	// тип ресурса
		volatile bool							ready		= false;				// готовность ресурса

		shared_ptr<DeviceEx>					this_device = nullptr;				// ссылка на устройство
	};

	class ResourceManager
	{
	private:
		// Конструктор по умолчанию
												ResourceManager();
		// Конструктор копирования
												ResourceManager(const ResourceManager&&)	= delete;
		// Оператор присвоения
		ResourceManager&						operator=(const ResourceManager&&)			= delete;
	public:
		// Деструктор
		virtual									~ResourceManager();
		// Возвращает экземпляр данного класса
		static shared_ptr<ResourceManager>		GetInstance();

		// Возвращает ссылку на интерфейс устройства
		shared_ptr<Device>						GetDevice(string name = MAIN_DEVICE);


		// Создание ресурса и возвращение его индекса
		template <class create_resource>
		int										CreateResource(const string& name, TResource type_resource = TResource::Shared);
		// Создание ресурса и возвращение его индекса и ссылки
		template <class create_resource>
		shared_ptr<create_resource>				CreateResourcePtr(const string& name, int& index_resource, TResource type_resource = TResource::Shared);
		// Удаление ресурса из списка
		void									DeleteResource(int index_resource);
		// Поиск ресурса по имени
		int										FindResource(string& name, TResource type_resource = TResource::All);
		// Возвращает ресурс по указанному индексу
		template <class get_resource>
		shared_ptr<get_resource>				GetResource(int index_resource);
		// Возвращает ссылку на список рессурсов
		vector<shared_ptr<IResource>>*			GetResources();

		// -------------------- Материалы (свойства) --------------------

		// Возвращает ресурс свойства материал из списка
		shared_ptr<PropertyBlockRes>			GetMaterialPropertyBlockRes(shared_ptr<ScriptEngine::PropertyBlock> material_property_block);
		// -------------------- Шейдеры --------------------

		// Возвращает ресурс шейдера из списка
		shared_ptr<ShaderRes>					GetShaderRes(shared_ptr<ScriptEngine::Shader> shader);
		// Поиск шейдера из списка по имени
		int										FindShader(string name);

		// -------------------- Меши --------------------
		
		// Возвращает ресурс вершин из списка
		shared_ptr<VerticesRes>					GetVerticesRes(shared_ptr<ScriptEngine::Mesh> mesh);
		// Возвращает список ресурсов индексов(обхода треугольников) из списка
		void									GetIndexesRes(shared_ptr<ScriptEngine::Mesh> mesh, vector<shared_ptr<IndexRes>>& out_list_indexes_resource);

		// -------------------- Текстуры --------------------

		// Возвращает ресурс текстуры
		shared_ptr<TextureRes>					GetTextureRes(shared_ptr<ScriptEngine::Texture> texture);

	private:
		// Поиск свободного элемента в списке
		template <class find>
		int										FindEmpty(find& array_find);

		// Системные переменные
		shared_ptr<Device>						m_device;					// ссылка на интерфейс устройства

		// Внутренние переменный
		vector<shared_ptr<IResource>>			m_list_resources;
	};

	// Создание ресурса и возвращение его индекса
	template<class create_resource>
	inline int ResourceManager::CreateResource(const string& name, TResource type_resource)
	{
		shared_ptr<create_resource> new_resource = make_shared<create_resource>(name);

		if(((shared_ptr<IResource>)new_resource)->type == TResource::Unknown)
		{ ((shared_ptr<IResource>)new_resource)->type = type_resource; }

		int index = FindEmpty<vector<shared_ptr<IResource>>>(m_list_resources);

		m_list_resources[index] = new_resource;

		return index;
	}
	// Создание ресурса и возвращение его индекса и ссылки
	template<class create_resource>
	inline shared_ptr<create_resource> ResourceManager::CreateResourcePtr(const string& name, int& index_resource, TResource type_resource)
	{
		shared_ptr<create_resource> new_resource = make_shared<create_resource>(name);

		if(((shared_ptr<IResource>)new_resource)->type == TResource::Unknown)
		{ ((shared_ptr<IResource>)new_resource)->type = type_resource; }

		index_resource = FindEmpty<vector<shared_ptr<IResource>>>(m_list_resources);

		m_list_resources[index_resource] = new_resource;

		return new_resource;
	}
	// Возвращает ресурс по указанному индексу
	template<class get_resource>
	inline shared_ptr<get_resource> ResourceManager::GetResource(int index_resource)
	{
		return static_pointer_cast<get_resource>(m_list_resources[index_resource]);
	}
	// Поиск свободного элемента в списке
	template<class find>
	inline int ResourceManager::FindEmpty(find& array_find)
	{
		int return_index = UNKNOW_INDEX_ELEMENTS;
		// Проверим на пустоту списка
		if (array_find.empty())
		{
			// Изменим размер массива
			array_find.push_back(nullptr);
			// Укажем его индекс в списке
			return_index = array_find.size() - 1;
		}
		// Список не пустой
		else
		{
			// Инициализация позиции
			int current_position = 0;
			// По умолчанию список полный
			bool full_list = true;
			// Пройдёмся по всему списку, с целью найти пустое место
			for (auto& it_find : array_find)
			{
				// Проверим текущую позицию на пустоту
				if (it_find == nullptr)
				{
					// Укажем что список был не полным
					full_list = false;
					// Укажем пустой индек
					return_index = current_position;
					// Остановка поиска в списке
					break;
				}
				// Увеличим позицию индекса
				current_position++;
			}
			// Провери на полноту списка, если он полный добавим новый элемент в конец
			if (full_list)
			{
				// Изменим размер
				array_find.push_back(nullptr);
				// Укажем индекс добавленного
				return_index = array_find.size() - 1;
			}
		}
		// Вернём индекс добавленного
		return return_index;
	}
}