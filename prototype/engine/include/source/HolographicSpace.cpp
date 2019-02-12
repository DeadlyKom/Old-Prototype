#include "..\HolographicSpace.h"

namespace SystemEngine
{
	// Конструктор
	HoloSpace::HoloSpace()
	{
		m_device_manager = DeviceManager::GetInstance();
	}

	// Деструктор
	HoloSpace::~HoloSpace()
	{
		m_device_manager.reset();
	}

	// Установка загрузчика асетов
	void HoloSpace::SetLoaderAsset(LoaderAssets& loader_assets_ptr)
	{
		m_in_loader_assets = &loader_assets_ptr;
	}

	// Установка голографического пространства
	void HoloSpace::SetHolographicSpace(HolographicSpace^ holographic_space)
	{
		// Освободим ранее зарегестрированные обработчики событий
		UnregisterHolographicEventHandlers();
		// Сохраним ссылку интерфейса работы с пространством
		m_holographic_space = holographic_space;
		// Получим интефейс позволяющий определять положение HoloLens в пространстве
		m_locator = SpatialLocator::GetDefault();
		// Укажем обработчик изменения пространства
		m_locatability_changed_token	= m_locator->LocatabilityChanged		+= ref new Windows::Foundation::TypedEventHandler<SpatialLocator^, 
			Object^>(bind(&HoloSpace::OnLocatabilityChanged, this, _1, _2));
		// Укажем обработчик добавления камеры
		m_camera_added_token			= m_holographic_space->CameraAdded		+= ref new Windows::Foundation::TypedEventHandler<HolographicSpace^, 
			HolographicSpaceCameraAddedEventArgs^>(bind(&HoloSpace::OnCameraAdded, this, _1, _2));
		// Укажем обработчик удаления камеры
		m_camera_removed_token			= m_holographic_space->CameraRemoved	+= ref new Windows::Foundation::TypedEventHandler<HolographicSpace^, 
			HolographicSpaceCameraRemovedEventArgs^>(bind(&HoloSpace::OnCameraRemoved, this, _1, _2));
		// Создаёт фрейм являющийся начальной системой координат
		m_reference_frame				= m_locator->CreateStationaryFrameOfReferenceAtCurrentLocation();
	}

	// Установка списка имён устройств (необходим для сопоставления с камерами)
	//void HoloSpace::SetDeviceNames(vector<string>& list_device_names)
	//{
	//	m_list_device_names.resize(list_device_names.size());
	//	copy(list_device_names.begin(), list_device_names.end(), m_list_device_names.begin());
	//	m_current_device = m_list_device_names.begin();
	//}

	// Обработчик асинхронного создания ресурсов для новых голографических камер
	void HoloSpace::OnCameraAdded(HolographicSpace^ sender, HolographicSpaceCameraAddedEventArgs^ args)
	{
		// Запросим не добовлять камеру, до того как мы её проинициализируем
		Deferral^ deferral = args->GetDeferral();
		// Получим добовляемую камеру
		HolographicCamera^ holographic_camera = args->Camera;
		// Создадим поток для асинхронной загрузки ресурсов текущей камеры
		create_task([this, deferral, holographic_camera]()
		{
			// ----------------------------------------
			// Загрузка ресурсов для новой камеры 
			// ----------------------------------------
			m_in_loader_assets->ExecuteAssetCamera(MAIN_CAMERA);

			// Добавим устройство к голографической камере
			UseHolographicCamera<void>([this, holographic_camera](map<uint32, unique_ptr<CameraEx>>& camera_resource_map)
			{
				camera_resource_map[holographic_camera->Id] = make_unique<CameraEx>();
				
				//camera_resource_map[holographic_camera->Id]->this_device = GetDeviceNext();

				camera_resource_map[holographic_camera->Id]->this_device->SetHolographicSpace(m_holographic_space);

				camera_resource_map[holographic_camera->Id]->SetHolographicCamera(holographic_camera);
			});

			// Сообщим что данную камеру можно добовлять
			deferral->Complete();
		});
	}

	// Обработчик синхронного освобождения ресурсов для устаревших голографических камер
	void HoloSpace::OnCameraRemoved(HolographicSpace^ sender, HolographicSpaceCameraRemovedEventArgs^ args)
	{
		create_task([this]()
		{
			// ----------------------------------------
			// Асинхроннай выгрузка ресурсов (кроме back buffer)
			// ----------------------------------------
			m_in_loader_assets->ReleaseAssetCamera(MAIN_CAMERA);
		});
		//
		//m_device->RemoveHolographicCamera(args->Camera);
	}

	// Обработчик изменения позиции
	void HoloSpace::OnLocatabilityChanged(SpatialLocator^ sender, Object^ args)
	{
	}

	// Освобождение ранее зарегистрированных обработчиков событий
	void HoloSpace::UnregisterHolographicEventHandlers()
	{
		if (m_holographic_space != nullptr)
		{
			// Очистка ранее регестрированных обработчиков
			if (m_camera_added_token.Value != 0)	{ m_holographic_space->CameraAdded		-= m_camera_added_token;			m_camera_added_token.Value		= 0; }

			if (m_camera_removed_token.Value != 0)	{ m_holographic_space->CameraRemoved	-= m_camera_removed_token;			m_camera_removed_token.Value	= 0; }
		}

		if (m_locator != nullptr)					{ m_locator->LocatabilityChanged		-= m_locatability_changed_token;										 }
	}

	// Вернуть ссылку на устройство (с последующим переключением на следующее, если их несколько)
	//shared_ptr<DeviceEx> HoloSpace::GetDeviceNext()
	//{
	//	if (m_current_device == m_list_device_names.end()) { m_current_device = m_list_device_names.begin(); }

	//	shared_ptr<DeviceEx> output_device = m_device_manager->GetDevice(*m_current_device);

	//	m_current_device++;

	//	return output_device;
	//}
}