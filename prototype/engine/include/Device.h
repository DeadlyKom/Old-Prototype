#pragma once

#include <agile.h>
#include <d2d1.h>
#include <d2d1_1.h>
#include <d2d1_2.h>
#include <dwrite_2.h>
#include <d3d11_4.h>
#include <D3D12.h>
#include <wincodec.h>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <windows.graphics.directx.direct3d11.interop.h>

#include "prototype\include\Type.h"
#include "Camera.h"
#include "ResourceManager.h"

using namespace std;
using namespace Microsoft::WRL;
using namespace Windows::Graphics::Holographic;
using namespace Windows::Graphics::DirectX::Direct3D11;

namespace SystemEngine
{
	interface IDeviceEvents
	{
		virtual void						OnDeviceLost() = 0;
		virtual void						OnDeviceRestored() = 0;
	};

	class Device : public IResource, public enable_shared_from_this<Device>
	{
	public:
											Device() = delete;
		// Конструктор
											Device(string name);
		// Деструктор
		virtual								~Device();
		
		// Интерфейсные методы

		// Перезагрузить
		virtual task<void>					Reload() override { return task<void>();};
		// Освободить
		virtual void						Release() override {};

		// Установка голографического пространства
		void								SetHolographicSpace(HolographicSpace^ holographic_space);
		//
		void								EnsureCameraResources(HolographicFrame^ frame, HolographicFramePrediction^ prediction);
		// Добовление голографической камеры к устройству
		void								AddHolographicCamera(HolographicCamera^ camera);
		// Удаление голографической камеры
		void								RemoveHolographicCamera(HolographicCamera^ camera);

		// Регистрация обработчика событий
		void								RegisterEvents(shared_ptr<IDeviceEvents> events_handler);
		//
		void								Trim();
		// Переключение каналов отображения
		void								Present(HolographicFrame^ frame);
		// Передача обработки потерянного устройства
		void								HandleDeviceLost();

		// Возвращает интерфейс устройства
		ID3D11Device4*						GetDevice() const;
		// Возвращает интерфейс контекста
		ID3D11DeviceContext3*				GetDeviceContext() const;

		// Доступ к голографической камере
		template<typename RetType, typename LCallback>
		RetType								UseHolographicCameraResources(const LCallback& callback);


	protected:
		// Создание дополнительных интерфейсов работы с графикой
		void								Create();
		// Инициализация голографического устройства 
		void								InitializeHolographicSpace();
		// Создание интерфейса устройства
		void								CreateDevice();


		// Системные переменные
		shared_ptr<ResourceManager>			m_resource_manadger;	// ссылка на диспетчер ресурсов

		ComPtr<ID3D11Device4>				m_d3d_device;			// устройство 
		ComPtr<ID3D11DeviceContext3>		m_d3d_context;			// контекст
		ComPtr<IDXGIAdapter3>				m_DXGI_adapter;			// адаптер

		IDirect3DDevice^					m_d3d_interop_device;

		ComPtr<ID2D1Factory2>				m_d2d_factory;
		ComPtr<IDWriteFactory2>				m_direct_write_factory;
		ComPtr<IWICImagingFactory2>			m_WIC_factory;

		HolographicSpace^					m_holographic_space;

		D3D_FEATURE_LEVEL					m_d3d_feature_level;

		bool								m_supports_Vprt;
		
		map<uint32, unique_ptr<Camera>>		m_camera_resources;
		mutex								m_camera_resources_lock;

		shared_ptr<IDeviceEvents>			m_events_handler;
	};

	template<typename RetType, typename LCallback>
	inline RetType Device::UseHolographicCameraResources(const LCallback & callback)
	{
		lock_guard<mutex> guard(m_camera_resources_lock);
		return callback(m_camera_resources);
	}
}