#include <wrl\wrappers\corewrappers.h>

#include "prototype\engine\include\FileSystem.h"

using namespace Microsoft::WRL;
using namespace Windows::ApplicationModel;
using namespace Windows::Storage::Streams;

namespace SystemEngine
{
	// Конструктор
	FileSystem::FileSystem()
	{
		m_location_folder = Package::Current->InstalledLocation;
	}

	// Деструктор
	FileSystem::~FileSystem()
	{
	}

	// Возвращает путь к папке
	StorageFolder^ FileSystem::GetFolder()
	{
		return m_location_folder;
	}

	// Устанавливает путь к папке
	void FileSystem::SetFolder(StorageFolder^ new_folder)
	{
		if (new_folder->Path->Length() != 0)		{ m_location_folder = new_folder; }
	}

	// Чтение данных (не асинхронно)
	bool FileSystem::ReadData(Platform::String^ file_name, shared_ptr<vector<byte>>& data_destination)
	{
		CREATEFILE2_EXTENDED_PARAMETERS extended_params = { 0 };
		extended_params.dwSize					= sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
		extended_params.dwFileAttributes		= FILE_ATTRIBUTE_NORMAL;
		extended_params.dwFileFlags				= FILE_FLAG_SEQUENTIAL_SCAN;
		extended_params.dwSecurityQosFlags		= SECURITY_ANONYMOUS;
		extended_params.lpSecurityAttributes	= nullptr;
		extended_params.hTemplateFile			= nullptr;

		Wrappers::FileHandle file(CreateFile2(file_name->Data(), GENERIC_READ, FILE_SHARE_READ, OPEN_EXISTING, &extended_params));

		if (file.Get() == INVALID_HANDLE_VALUE)																{ return true; }

		FILE_STANDARD_INFO file_info = { 0 };

		if (!GetFileInformationByHandleEx(file.Get(), FileStandardInfo, &file_info, sizeof(file_info)))		{ return true; }

		if (file_info.EndOfFile.HighPart != 0)																{ return true; }

		data_destination = make_shared<vector<byte>>(file_info.EndOfFile.LowPart);

		if (!ReadFile(file.Get(), data_destination->data(), data_destination->size(), nullptr, nullptr))	{ return true; }

		return false;
	}
	
	// Чтение данных (асинхронно)
	task<shared_ptr<vector<byte>>> FileSystem::ReadDataAsync(Platform::String^ file_name)
	{
		return create_task(PathIO::ReadBufferAsync(file_name)).then(
			[](Streams::IBuffer^ file_buffer) -> shared_ptr<vector<byte>>
		{
			shared_ptr<vector<byte>> return_buffer = make_shared<vector<byte>>();
			return_buffer->resize(file_buffer->Length);
			Streams::DataReader::FromBuffer(file_buffer)->ReadBytes(Platform::ArrayReference<byte>(return_buffer->data(), return_buffer->size()));
			return return_buffer;
		});
	}

	// Запись данных (не асинхронно)
	bool FileSystem::WriteData(Platform::String^ file_name, const shared_ptr<vector<byte>>& data_source, uint32& byte_written)
	{
		CREATEFILE2_EXTENDED_PARAMETERS extended_params = { 0 };
		extended_params.dwSize					= sizeof(CREATEFILE2_EXTENDED_PARAMETERS);
		extended_params.dwFileAttributes		= FILE_ATTRIBUTE_NORMAL;
		extended_params.dwFileFlags				= FILE_FLAG_SEQUENTIAL_SCAN;
		extended_params.dwSecurityQosFlags		= SECURITY_ANONYMOUS;
		extended_params.lpSecurityAttributes	= nullptr;
		extended_params.hTemplateFile			= nullptr;

		Wrappers::FileHandle file(CreateFile2(file_name->Data(), GENERIC_WRITE, 0, CREATE_ALWAYS, &extended_params));
		
		if (file.Get() == INVALID_HANDLE_VALUE)																											{ return true; }

		if (!WriteFile(file.Get(), data_source->data(), data_source->size(), (LPDWORD)byte_written, nullptr) || byte_written != data_source->size())	{ return true; }

		return false;
	}

	// Запись данных (асинхронно)
	task<void> FileSystem::WriteDataAsync(Platform::String^ file_name, const shared_ptr<vector<byte>>& data_source)
	{
		return task<StorageFile^>(m_location_folder->CreateFileAsync(file_name, CreationCollisionOption::ReplaceExisting)).then([&data_source](StorageFile^ file)
		{
			FileIO::WriteBytesAsync(file, Platform::ArrayReference<byte>(data_source->data(), data_source->size()));
		});
	}
}