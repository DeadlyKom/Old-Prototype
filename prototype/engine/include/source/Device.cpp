#include <Collection.h>

#include "prototype\engine\include\Device.h"

#define DEBUG_TEXT_CREATE_FACTORY L"Error create factory Direct2D"

namespace SystemEngine
{
	// Конструктор
	Device::Device(string name)
	{
		IResource::name = name;

		//IResource::type = TResource::Device;

		m_supports_Vprt = false;

		m_holographic_space = nullptr;

		m_d3d_feature_level = (D3D_FEATURE_LEVEL)0;

		m_resource_manadger = ResourceManager::GetInstance();;

		Create();
	}

	// Деструктор
	Device::~Device()
	{
		m_resource_manadger.reset();
	}

	// Создание дополнительных интерфейсов работы с графикой
	void Device::Create()
	{
		D2D1_FACTORY_OPTIONS options{};

#		if defined(_DEBUG)
		options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
#		endif

		// Инициализация Direct2D
		HRESULT result_direct_2d = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, __uuidof(ID2D1Factory2), &options, &m_d2d_factory);

		if (FAILED(result_direct_2d)) {
			OutputDebugString(DEBUG_TEXT_CREATE_FACTORY);
		}

		// Инициализация DirectWrite
		HRESULT result_direct_write = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory2), &m_direct_write_factory);
		
		if (FAILED(result_direct_write)) {
			int a = 10;
		}

		// Инициализация Windows Imaging Component (WIC)
		HRESULT result_WIC = CoCreateInstance(CLSID_WICImagingFactory2, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&m_WIC_factory));
		
		if (FAILED(result_WIC)) {
			int a = 10;
		}
	}

	// Инициализация голографического устройства 
	void Device::InitializeHolographicSpace()
	{
		// Укажим какой адаптер поддерживает голограммы
		LUID id = { m_holographic_space->PrimaryAdapterId.LowPart, m_holographic_space->PrimaryAdapterId.HighPart };

		//
		if ((id.HighPart != 0) && (id.LowPart != 0))
		{
			UINT createFlags = 0;

#		ifdef _DEBUG
			HRESULT result_device = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_NULL, 0, D3D11_CREATE_DEVICE_DEBUG, nullptr, 0, D3D11_SDK_VERSION, nullptr, nullptr, nullptr);

			if (SUCCEEDED(result_device)) { createFlags |= DXGI_CREATE_FACTORY_DEBUG; }
#		endif
			// Создание DXGI
			ComPtr<IDXGIFactory1> DXGI_factory;

			HRESULT result_DXGI = CreateDXGIFactory2(createFlags, IID_PPV_ARGS(&DXGI_factory));
			
			if (FAILED(result_DXGI)) {
				int a = 10;
			}

			ComPtr<IDXGIFactory4> DXGI_factory4;

			HRESULT result_cast = DXGI_factory.As(&DXGI_factory4);
			
			if (FAILED(result_cast)) {
				int a = 10;
			}

			// Получение необходимого адаптера для указанного голографического устройства
			HRESULT result_retrieve = DXGI_factory4->EnumAdapterByLuid(id, IID_PPV_ARGS(&m_DXGI_adapter));
			
			if (FAILED(result_retrieve)) {
				int a = 10;
			}
		}
		else
		{
			m_DXGI_adapter.Reset();
		}

		CreateDevice();

		m_holographic_space->SetDirect3D11Device(m_d3d_interop_device);
	}

	// Создание интерфейса устройства
	void Device::CreateDevice()
	{
		UINT creation_flags = D3D11_CREATE_DEVICE_BGRA_SUPPORT;

#		if defined(_DEBUG)
		HRESULT result_device = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_NULL, 0, D3D11_CREATE_DEVICE_DEBUG, nullptr, 0, D3D11_SDK_VERSION, nullptr, nullptr, nullptr);

		if (SUCCEEDED(result_device))		{ creation_flags |= D3D11_CREATE_DEVICE_DEBUG; }
#		endif

		D3D_FEATURE_LEVEL feature_levels[] = { D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0, D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_1, D3D_FEATURE_LEVEL_10_0 };
		
		// Create the Direct3D 11 API device object and a corresponding context.
		ComPtr<ID3D11Device> device;
		ComPtr<ID3D11DeviceContext> context;

		const HRESULT result_create_device = D3D11CreateDevice(m_DXGI_adapter.Get(), D3D_DRIVER_TYPE_HARDWARE, 0, creation_flags, 
			feature_levels, ARRAYSIZE(feature_levels), D3D11_SDK_VERSION, &device, &m_d3d_feature_level, &context);
		////---------------------
		//ComPtr<IDXGIFactory4> factory;
		//CreateDXGIFactory1(IID_PPV_ARGS(&factory));
		//ComPtr<IDXGIAdapter1> hardwareAdapter;
		//IDXGIAdapter1** ppAdapter = &hardwareAdapter; *ppAdapter = nullptr;
		//for (UINT adapterIndex = 0; ; ++adapterIndex)
		//{
		//	IDXGIAdapter1* pAdapter = nullptr;
		//	if (DXGI_ERROR_NOT_FOUND == factory->EnumAdapters1(adapterIndex, &pAdapter))
		//	{
		//		// No more adapters to enumerate.
		//		break;
		//	}
		//	// Check to see if the adapter supports Direct3D 12, but don't create the
		//	// actual device yet.
		//	if (SUCCEEDED(D3D12CreateDevice(pAdapter, D3D_FEATURE_LEVEL_12_1, _uuidof(ID3D12Device), nullptr)))
		//	{
		//		*ppAdapter = pAdapter;
		//		break;
		//	}
		//	pAdapter->Release();
		//}
		//DXGI_ADAPTER_DESC1 desc;
		//hardwareAdapter->GetDesc1(&desc);
		//hardwareAdapter = nullptr;
		//ComPtr<ID3D12Device> device_12;
		//HRESULT result_create_device_12 = D3D12CreateDevice(hardwareAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&device_12));
		////----------------
		if (FAILED(result_create_device))
		{
			HRESULT result_create_device = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_WARP, 0, creation_flags, feature_levels, 
				ARRAYSIZE(feature_levels), D3D11_SDK_VERSION, &device, &m_d3d_feature_level, &context);

			if (FAILED(result_create_device)) {
				int a = 10;
			}
		}



		// Store pointers to the Direct3D device and immediate context.
		HRESULT result_cast_device = device.As(&m_d3d_device);
		
		if (FAILED(result_cast_device)) {
			int a = 10;
		}

		HRESULT result_cast_context = context.As(&m_d3d_context);

		if (FAILED(result_cast_context)) {
			int a = 10;
		}

		// Acquire the DXGI interface for the Direct3D device.
		ComPtr<IDXGIDevice3> DXGI_device;

		HRESULT result_cast_DXGI_device = m_d3d_device.As(&DXGI_device);
		
		if (FAILED(result_cast_DXGI_device)) {
			int a = 10;
		}

		// Wrap the native device using a WinRT interop object.
		m_d3d_interop_device = CreateDirect3DDevice(DXGI_device.Get());

		// Cache the DXGI adapter.
		// This is for the case of no preferred DXGI adapter, or fallback to WARP.
		ComPtr<IDXGIAdapter> DXGI_adapter;

		HRESULT result_get_adapter = DXGI_device->GetAdapter(&DXGI_adapter);
		
		if (FAILED(result_get_adapter)) {
			int a = 10;
		}

		HRESULT result_cast_adapter = DXGI_adapter.As(&m_DXGI_adapter);

		//m_DXGI_adapter->GetDesc1(&desc);

		if (FAILED(result_cast_adapter)) {
			int a = 10;
		}

		// Check for device support for the optional feature that allows setting the render target array index from the vertex shader stage.
		D3D11_FEATURE_DATA_D3D11_OPTIONS3 options;

		m_d3d_device->CheckFeatureSupport(D3D11_FEATURE_D3D11_OPTIONS3, &options, sizeof(options));
		
		if (options.VPAndRTArrayIndexFromAnyShaderFeedingRasterizer)		{ m_supports_Vprt = true; }

		//D3D11_FEATURE_DATA_THREADING threading;

		//m_d3d_device->CheckFeatureSupport(D3D11_FEATURE_THREADING, &threading, sizeof(threading));

		//ID3D11DeviceContext* contexts;

		//HRESULT hr_c = m_d3d_device->CreateDeferredContext(0, &contexts);

		//if (threading.DriverConcurrentCreates) 
		//{ 
		//	int a = 10;
		//	m_d3d_device->GetCreationFlags();
		//}
		//if (threading.DriverCommandLists)
		//{
		//	int a = 10;
		//	m_d3d_device->GetCreationFlags();
		//}
		//m_d3d_device->GetCreationFlags();
		//ID3D11DeviceContext* deferred;
		//HRESULT result = m_d3d_device->CreateDeferredContext(0, &deferred);
		//int a = 10;

	}

	// Установка голографического пространства
	void Device::SetHolographicSpace(HolographicSpace^ holographic_space)
	{
		// Сохраним указатель на голографическое устройство
		m_holographic_space = holographic_space;

		InitializeHolographicSpace();
	}

	//
	void Device::EnsureCameraResources(HolographicFrame^ frame, HolographicFramePrediction^ prediction)
	{
		UseHolographicCameraResources<void>([this, frame, prediction](map<uint32, unique_ptr<Camera>>& camera_resource_map)
		{
			for (HolographicCameraPose^ pose : prediction->CameraPoses)
			{
				HolographicCameraRenderingParameters^ rendering_parameters = frame->GetRenderingParameters(pose);

				Camera* camera_resources_ptr = camera_resource_map[pose->HolographicCamera->Id].get();

				camera_resources_ptr->CreateResourcesForBackBuffer(shared_from_this(), rendering_parameters);
			}
		});
	}

	// Добовление голографической камеры к устройству
	void Device::AddHolographicCamera(HolographicCamera^ camera)
	{
		UseHolographicCameraResources<void>([this, camera](map<uint32, unique_ptr<Camera>>& camera_resource_map)
		{
			camera_resource_map[camera->Id] = make_unique<Camera>(MAIN_CAMERA);

			camera_resource_map[camera->Id]->SetHolographic(camera);
		});
	}

	// Удаление голографической камеры
	void Device::RemoveHolographicCamera(HolographicCamera^ camera)
	{
		UseHolographicCameraResources<void>([this, camera](map<uint32, unique_ptr<Camera>>& camera_resource_map)
		{
			Camera* camera_resources_ptr = camera_resource_map[camera->Id].get();

			if (camera_resources_ptr != nullptr)
			{
				camera_resources_ptr->ReleaseResourcesForBackBuffer(shared_from_this());
				camera_resource_map.erase(camera->Id);
			}
		});
	}

	// Регистрация обработчика событий
	void Device::RegisterEvents(shared_ptr<IDeviceEvents> events_handler)
	{
		m_events_handler = events_handler;
	}

	//
	void Device::Trim()
	{
		m_d3d_context->ClearState();

		ComPtr<IDXGIDevice3> dxgi_device;

		HRESULT result_cast_device = m_d3d_device.As(&dxgi_device);
		
		if (FAILED(result_cast_device)) {
			int a = 10;
		}

		dxgi_device->Trim();
	}

	// Переключение каналов отображения
	void Device::Present(HolographicFrame^ frame)
	{
		HolographicFramePresentResult present_result = frame->PresentUsingCurrentPrediction();
		// Проверим на потерю устройства
		if (present_result == HolographicFramePresentResult::DeviceRemoved) { HandleDeviceLost(); }

		HolographicFramePrediction^ prediction = frame->CurrentPrediction;

		UseHolographicCameraResources<void>([this, prediction](map<uint32, unique_ptr<Camera>>& camera_resource_map)
		{
			for (auto camera_pose : prediction->CameraPoses)
			{
				// Получение камеры
				Camera* camera_resources_ptr = camera_resource_map[camera_pose->HolographicCamera->Id].get();
				// Очистка бувера отрисовки
				m_d3d_context->DiscardView(camera_resources_ptr->GetBackBufferRenderTargetView());
				// Очистка буфера трафарета и глубины
				m_d3d_context->DiscardView(camera_resources_ptr->GetDepthStencilView());
			}
		});
		// Проверим на потерю устройства
		//if (present_result == HolographicFramePresentResult::DeviceRemoved)	{ HandleDeviceLost(); }
	}

	// Передача обработки потерянного устройства
	void Device::HandleDeviceLost()
	{
		if (m_events_handler != nullptr)		{ m_events_handler->OnDeviceLost(); }

		UseHolographicCameraResources<void>([this](map<uint32, unique_ptr<Camera>>& camera_resource_map)
		{
			for (auto& pair : camera_resource_map)
			{
				Camera* сamera_resources_ptr = pair.second.get();
				сamera_resources_ptr->ReleaseResourcesForBackBuffer(shared_from_this());
			}
		});

		InitializeHolographicSpace();

		if (m_events_handler != nullptr)		{ m_events_handler->OnDeviceRestored(); }
	}

	// Возвращает интерфейс устройства
	ID3D11Device4* Device::GetDevice() const
	{
		return m_d3d_device.Get();
	}

	// Возвращает интерфейс контекста
	ID3D11DeviceContext3* Device::GetDeviceContext() const
	{
		return m_d3d_context.Get();
	}
}