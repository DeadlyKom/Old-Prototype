#include "prototype\engine\include\FileManager.h"

namespace SystemEngine
{
	// Конструктор
	FileManager::FileManager()
	{
	}

	// Деструктор
	FileManager::~FileManager()
	{
	}

	shared_ptr<FileManager> FileManager::GetInstance()
	{
		static shared_ptr<FileManager> file_manager_singleton(new FileManager());/* = make_shared<Device>();*/

		return file_manager_singleton;
	}

	// Добавить файл в общий список
	bool FileManager::AddFile(Platform::String^ path_file_name, int& out_index_file, bool add_folder)
	{
		// Заранее укажем об неудачности добовления файла
		out_index_file = UNKNOW_INDEX_ELEMENTS;

		// --------------------------------------------------
		// Проверка нахождения файла по указанному пити
		//volatile bool fine = false;
		//task<void>([this, path_file_name, &fine]()
		//{
		//	return create_task([path_file_name, &fine]()
		//	{
		//		wstring path, name;
		//		bool name_save = true;
		//		wchar_t current;
		//		for (auto it = path_file_name->Length() - 1; it > 0; it--)
		//		{
		//			current = path_file_name->Data()[it];
		//			if ((current == '\\') || (current == '/')) { 
		//				name_save = false; }
		//			if (name_save)
		//			{
		//				name.insert(name.begin(), current);
		//			}
		//			else
		//			{
		//				path.insert(path.begin(), current);
		//			}
		//		}
		//		path.insert(path.begin(), path_file_name->Data()[0]);
		//		Windows::Storage::StorageFolder^ installedLocation = Windows::ApplicationModel::Package::Current->InstalledLocation;
		//		try {
		//			Platform::String^ path_plf = installedLocation->Path + "\\" + %Platform::String(path.data(), path.size());
		//			Platform::String^ name_plf = %Platform::String(name.data(), name.size());
		//			auto load = create_task(StorageFolder::GetFolderFromPathAsync(path_plf))
		//				.then([=, &name_plf, &fine](StorageFolder^ folder)->task <IStorageItem^> {
		//				try {
		//					return create_task(folder->TryGetItemAsync(name_plf));
		//				}
		//				catch (...)
		//				{
		//					int a = 01;
		//				}
		//			})
		//				.then([=, &fine](IStorageItem^ file)
		//				{
		//					if (file != nullptr)
		//					{
		//						int a = 10;
		//						fine = true;
		//					}
		//				});
		//		}
		//		catch (/*Platform::COMException^ e*/...)
		//		{
		//			int a = 10;
		//		}
		//	});
		//})/*, task_continuation_context::use_current()*/;
		//while (!fine) {}
		// --------------------------------------------------

		// Сформируем новую структуру хранения информации о файле
		shared_ptr<file_info> new_info = make_shared<file_info>();
		// Сохраним значения в структуре
		new_info->path_file_name = add_folder ? (m_location_folder->Path + "\\" +  path_file_name) : path_file_name;
		// Проверим на пустоту списка
		if (m_list_file_info.empty())
		{
			// Добавим значение в конец
			m_list_file_info.push_back(new_info);
			// Укажем его индекс в списке
			out_index_file = m_list_file_info.size() - 1;
		}
		// Список не пустой
		else
		{
			// Инициализация позиции
			int current_position = 0;
			// По умолчанию список полный
			bool full_list = true;
			// Пройдёмся по всему списку, с целью найти пустое место
			for (auto& it_find : m_list_file_info)
			{
				// Проверим текущую позицию на пустоту
				if (it_find == nullptr)
				{
					// Укажем что список был не полным
					full_list = false;
					// Укажем пустой индек
					out_index_file = current_position;
					// Сохраним значение в списке
					m_list_file_info[out_index_file] = new_info;
					// Остановка поиска в списке
					break;
				}
				// Увеличим позицию индекса
				current_position++;
			}
			// Провери на полноту списка, если он полный добавим новый элемент в конец
			if (full_list)
			{
				// Добавим значение в конец
				m_list_file_info.push_back(new_info);
				// Укажем индекс добавленного
				out_index_file = m_list_file_info.size() - 1;
			}
		}
		// Вернём успешность добовления файла
		return false;
	}

	// Удаление файла из списка
	void FileManager::DeleteFileIndex(int index_file)
	{
		// Проверим на превышение индекса
		if ((int)m_list_file_info.size() <= index_file) { return; }
		// Освободим файл по указанному индексу
		m_list_file_info[index_file].reset();
	}

	// Загрузка файла заранее установленным имением и пути к нему
	bool FileManager::LoadFile(int index_file, shared_ptr<vector<byte>> data_destination)
	{	
		return ReadData(m_list_file_info[index_file]->path_file_name, data_destination);
	}

	// Загрузка файла заранее установленным имением и пути к нему (асинхронная загрузка)
	task<shared_ptr<vector<byte>>> FileManager::LoadFileAsync(int index_file)
	{
		return ReadDataAsync(m_list_file_info[index_file]->path_file_name);
	}
}