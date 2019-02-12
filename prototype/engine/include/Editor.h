#pragma once

#include <memory>

using namespace std;

#include "Assets\Script\Cube.h"

#include "FileManager.h"
#include "ObjectRegistry.h"

namespace SystemEngine
{
	class Editor
	{
	public:
		// Конструктор
									Editor();
		// Деструктор
		virtual						~Editor();
		// Инициализация редактора
		bool						Initialization();

	protected:
		// Системные переменные
		shared_ptr<FileManager>			m_file_manager;			// ссылка на диспетчер файлов
		shared_ptr<ObjectRegistry>		m_object_registry;		//
		shared_ptr<ResourceManager>		m_resource_manadger;	// ссылка на диспетчер ресурсов

		// Внутренние переменный
		vector<int>						m_list_index_resource;	// список индексов ресурсов (инициализированных в редакторе)

		// Скрипты
		shared_ptr<Cube>				m_cube;
	};
}
