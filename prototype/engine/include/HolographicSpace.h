#pragma once

#include <map>
#include <memory>
#include <mutex>
#include <ppltasks.h>
#include <string>
#include <vector>
#include <windows.graphics.directx.direct3d11.interop.h>

#include "..\prototype\include\Type.h"

#include "CameraEx.h"
#include "DeviceEx.h"
#include "DeviceManager.h"
#include "LoaderAssets.h"

using namespace std;
using namespace std::placeholders;
using namespace Concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Perception::Spatial;
using namespace Windows::Graphics::Holographic;
using namespace Windows::UI::Input::Spatial;

namespace SystemEngine
{
	class HoloSpace
	{
	public:
		// Конструктор
									HoloSpace();
		// Деструктор
		virtual						~HoloSpace();

		// Установка загрузчика асетов
		void						SetLoaderAsset(LoaderAssets& loader_assets_ptr);
		// Установка голографического пространства
		void						SetHolographicSpace(HolographicSpace^ holographic_space);
		// Установка списка имён устройств (необходим для сопоставления с камерами)
		//void						SetDeviceNames(vector<string>& list_device_names = vector<string>{ MAIN_DEVICE });

	private:
		// Обработчик асинхронного создания ресурсов для новых голографических камер
		void						OnCameraAdded(HolographicSpace^ sender, HolographicSpaceCameraAddedEventArgs^ args);
		// Обработчик синхронного освобождения ресурсов для устаревших голографических камер
		void						OnCameraRemoved(HolographicSpace^ sender, HolographicSpaceCameraRemovedEventArgs^ args);
		// Обработчик изменения позиции
		void						OnLocatabilityChanged(SpatialLocator^ sender, Object^ args);
		// Освобождение ранее зарегистрированных обработчиков событий
		void						UnregisterHolographicEventHandlers();
		// Последовательный доступ к голографической камере
		template<typename return_type, typename lamda_callback>
		return_type					UseHolographicCamera(const lamda_callback& callback);

	protected:
		// Вернуть ссылку на устройство (с последующим переключением на следующее, если их несколько)
		//shared_ptr<DeviceEx>		GetDeviceNext();

		// Системные переменные
		shared_ptr<DeviceManager>						m_device_manager;					// ссылка на диспетчер устройств

		// Внутренние переменные
		HolographicSpace^								m_holographic_space;				// голографическое пространство вокруг пользователя
		SpatialLocator^									m_locator;							// пространственный указатель движения устройства
		SpatialStationaryFrameOfReference^				m_reference_frame;					// ссылка на фрейм прикреплённый к голографической камере
		
		Windows::Foundation::EventRegistrationToken		m_camera_added_token;				// обработчик добовления камеры
		Windows::Foundation::EventRegistrationToken		m_camera_removed_token;				// обработчик удаления камеры
		Windows::Foundation::EventRegistrationToken		m_locatability_changed_token;		// обработчик изменения пространсва
		
		map<uint32, unique_ptr<CameraEx>>				m_camera_resources;
		mutex											m_camera_resources_lock;

		//vector<string>									m_list_device_names;				// список имён устройств (необходим для сопоставления с устройствоми по имени)
		//vector<string>::iterator						m_current_device;					// итератор на текущее устройство

		LoaderAssets*									m_in_loader_assets;					// ссылка на загрузчик асетов
	};

	// Последовательный доступ к голографической камере
	template<typename return_type, typename lamda_callback>
	inline return_type HoloSpace::UseHolographicCamera(const lamda_callback& callback)
	{
		lock_guard<mutex> guard(m_camera_resources_lock);
		return callback(m_camera_resources);
	}
}