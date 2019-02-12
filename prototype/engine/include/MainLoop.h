#pragma once

#include <memory>
#include <vector>
#include <ppltasks.h>
#include <string>

#include "Common\StepTimer.h"

#include "prototype\include\Type.h"
//#include "prototype\include\SceneObject.h"

#include "Device.h"
#include "InputHandler.h"
#include "ObjectRegistry.h"
#include "RendererManager.h"
#include "ResourceManager.h"

using namespace std;
using namespace std::placeholders;
using namespace concurrency;
using namespace Platform;
using namespace Windows::Foundation;
using namespace Windows::Perception::Spatial;
using namespace Windows::Graphics::Holographic;
using namespace Windows::UI::Input::Spatial;

namespace SystemEngine
{
	// Основной класс обработки SO
	class MainLoop : public IDeviceEvents, public enable_shared_from_this<MainLoop>
	{
	public:
		// Конструктор по умолчанию
									MainLoop();
		// Деструктор
									~MainLoop();
		// Установка голографического пространства
		void						SetHolographicSpace(HolographicSpace^ holographic_space);
		// Обновления элементов из списка объектов сцены
		HolographicFrame^ 			Update();
		// Отображение элементов из списка объектов сцене
		bool						Renderer(HolographicFrame^ holographic_frame);

		// Обработчик потерянного устройства
		virtual	void				OnDeviceLost()		override;
		// Обработчик перезагрузки устройства
		virtual void				OnDeviceRestored()	override;

	private:
		// Обработчик асинхронного создания ресурсов для новых голографических камер
		void						OnCameraAdded(HolographicSpace^ sender, HolographicSpaceCameraAddedEventArgs^ args);
		// Обработчик синхронного освобождения ресурсов для уставевших каммер
		void						OnCameraRemoved(HolographicSpace^ sender, HolographicSpaceCameraRemovedEventArgs^ args);
		// Обработчик изменения позиции
		void						OnLocatabilityChanged(SpatialLocator^ sender, Object^ args);
		// Освобождение ранее зарегистрированных обработчиков событий
		void						UnregisterHolographicEventHandlers();

		// Системные переменные
		shared_ptr<ResourceManager>						m_resource_manadger;				// ссылка на диспетчер ресурсов
		shared_ptr<Device>								m_device;							// ссылка на интерфейс устройства
		shared_ptr<InputHandler>						m_input;							// ссылка на обработчик ввода

		// Внутренние переменные
		HolographicSpace^								m_holographic_space;				// голографическое пространство вокруг пользователя
		SpatialLocator^									m_locator;							// пространственный указатель движения устройства
		SpatialStationaryFrameOfReference^				m_reference_frame;					// ссылка на фрейм прикреплённый к голографической камере
		shared_ptr<ObjectRegistry>						m_object_registry;					// ссылка реестра объектов
		
		Windows::Foundation::EventRegistrationToken		m_camera_added_token;				//
		Windows::Foundation::EventRegistrationToken		m_camera_removed_token;				//
		Windows::Foundation::EventRegistrationToken		m_locatability_changed_token;		//

		vector<Matrix4X4>								m_view_projection;

		ID3D11RasterizerState*							WireFrame;
	};
}