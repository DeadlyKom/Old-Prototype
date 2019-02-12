#pragma once

#include <memory>
#include <ppltasks.h>
#include <string>
#include <vector>

#include "prototype\include\Type.h"

#include "FileSystem.h"
#include "ResourceManager.h"

using namespace std;
using namespace concurrency;

namespace SystemEngine
{
	class FileManager : public FileSystem
	{
	private:
		struct file_info
		{
			Platform::String^		path_file_name;
		};

	private:
		// Конструктор по умолчанию
										FileManager();
		// Конструктор копирования
										FileManager(const FileSystem&&) = delete;
		// Оператор присвоения
		FileManager&					operator=(const FileManager&&) = delete;

	public:
		// Деструктор
		virtual							~FileManager();
		// Возвращает экземпляр данного класса
		static shared_ptr<FileManager>	GetInstance();
		// Добавление файла в общий список
		bool							AddFile(Platform::String^ path_file_name, int& out_index_file, bool add_folder = true);
		// Удаление файла из списка
		void							DeleteFileIndex(int index_file);
		// Загрузка файла
		bool							LoadFile(int index_file, shared_ptr<vector<byte>> data_destination);
		// Загрузка файла (асинхронная загрузка)
		task<shared_ptr<vector<byte>>>	LoadFileAsync(int index_file);

	protected:
		// Внутренние
		vector<shared_ptr<file_info>>	m_list_file_info;		// список с информацией о файлах
	};
}