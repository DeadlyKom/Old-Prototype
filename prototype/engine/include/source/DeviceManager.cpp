#include "..\DeviceManager.h"
namespace SystemEngine
{
	// Конструктор по умолчанию
	DeviceManager::DeviceManager()
	{
	}

	// Деструктор
	DeviceManager::~DeviceManager()
	{
	}

	// Возвращает экземпляр данного класса
	shared_ptr<DeviceManager> DeviceManager::GetInstance()
	{
		static shared_ptr<DeviceManager> device_manager_singleton(new DeviceManager());

		return device_manager_singleton;
	}

	// Создание устройства
	void DeviceManager::CreateDevice(string device_name)
	{
		m_map_devices[device_name] = make_shared<DeviceEx>();
	}

	// Возвращает устройство (по имени)
	shared_ptr<DeviceEx> DeviceManager::GetDevice(string device_name)
	{
		return m_map_devices[device_name];
	}
}