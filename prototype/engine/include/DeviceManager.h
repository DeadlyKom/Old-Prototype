#pragma once

#include <map>
#include <memory>
#include <string>

#include "prototype\include\Type.h"

#include "DeviceEx.h"

using namespace std;

namespace SystemEngine
{
	class DeviceManager
	{
	private:
		// Конструктор по умолчанию
												DeviceManager();
		// Конструктор копирования
												DeviceManager(const DeviceManager&&)	= delete;
		// Оператор присвоения
		DeviceManager&							operator=(const DeviceManager&&)		= delete;
	public:
		// Деструктор
		virtual									~DeviceManager();
		// Возвращает экземпляр данного класса
		static shared_ptr<DeviceManager>		GetInstance();
		// Создание устройства
		void									CreateDevice(string device_name = MAIN_DEVICE);
		// Возвращает устройство (по имени)
		shared_ptr<DeviceEx>					GetDevice(string device_name = MAIN_DEVICE);

	private:
		// Внутренние переменные
		map<string, shared_ptr<DeviceEx>>		m_map_devices;				// словарь утройств
	};
}