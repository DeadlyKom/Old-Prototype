#pragma once

#include <memory>
#include <ppltasks.h>
#include <vector>

#include "prototype\include\Type.h"

using namespace std;
using namespace Concurrency;
using namespace Platform;
using namespace Windows::Storage;

namespace SystemEngine
{
	class FileSystem
	{
	public:
		// Конструктор
											FileSystem();
		// Деструктор
		virtual								~FileSystem();
		// Возвращает путь к папке
		StorageFolder^						GetFolder();
		// Устанавливает путь к папке
		void								SetFolder(StorageFolder^ new_folder);
		// Чтение данных (не асинхронно)
		bool								ReadData(Platform::String^ file_name, shared_ptr<vector<byte>>& data_destination);
		// Чтение данных (асинхронно)
		task<shared_ptr<vector<byte>>>		ReadDataAsync(Platform::String^ file_name);
		// Запись данных (не асинхронно)
		bool								WriteData(Platform::String^ file_name, const shared_ptr<vector<byte>>& data_source, uint32& byte_written);
		// Запись данных (асинхронно)
		task<void>							WriteDataAsync(Platform::String^ file_name, const shared_ptr<vector<byte>>& data_source);

	protected:
		StorageFolder^						m_location_folder;
	};
}