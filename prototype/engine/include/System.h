#pragma once

#include <memory>

#include "prototype\include\Type.h"

#include "Editor.h"
#include "DeviceManager.h"
#include "HolographicSpace.h"
#include "LoaderAssets.h"
#include "MainLoop.h"
#include "ObjectRegistry.h"
#include "ResourceManager.h"

using namespace Platform;
using namespace Windows::UI::Core;
using namespace Windows::ApplicationModel;
using namespace Windows::ApplicationModel::Core;
using namespace Windows::ApplicationModel::Activation;
using namespace Windows::Graphics::Holographic;

namespace SystemEngine
{
	ref class System sealed : public Windows::ApplicationModel::Core::IFrameworkView
	{
	public:
		// Конструктор
									System();

		// Унаследованые методы от IFrameworkView

		// Инициализация приложения
		virtual	void				Initialize(CoreApplicationView^ applicationView);
		// Установки текущего окна
		virtual void				SetWindow(CoreWindow^ window);
		// Загрузка внешних ресурсов
		virtual void				Load(String^ entryPoint);
		// Запуск приложения
		virtual void				Run();
		// Особождение ранее загруженых ресурсов
		virtual void				Uninitialize();

	private:
		// Обработчик активации приложения
		void						OnViewActivated(CoreApplicationView^ sender, IActivatedEventArgs^ args);
		// Обработчик приостановки приложения
		void						OnSuspending(Object^ sender, SuspendingEventArgs^ args);
		// Обработчик возабновления приложения
		void						OnResuming(Object^ sender, Platform::Object^ args);
		// Обработчик изменения видимости
		void						OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args);
		// Обработчик закрытия приложения
		void						OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args);

		// Системные переменные
		shared_ptr<Editor>				m_editor;				// ссылка на редактор компонентов (инициализатор)
		shared_ptr<Device>				m_device;				// ссылка на устройство
		shared_ptr<DeviceManager>		m_device_manager;		// ссылка на диспетчер устройств
		shared_ptr<HoloSpace>			m_holo_space;			// ссылка на пространство голограммы
		shared_ptr<MainLoop>			m_main_loop;			// ссылка на главный цикл
		shared_ptr<ObjectRegistry>		m_object_registry;		// ссылка на реестр объектов
		shared_ptr<ResourceManager>		m_resource_manadger;	// ссылка на диспетчер ресурсов

		// Внутренние переменные
		bool							m_window_visible;		// фдаг видимости окна
		bool							m_window_closed;		// флаг закрытия окна
		HolographicSpace^				m_holographic_space;

		LoaderAssets					m_loader_assets;

		int								m_index_device;
		
		bool							m_first_reload_resource;	// флаг первоначальной загрузки ресурсов
	};

	ref class SystemSource sealed : IFrameworkViewSource
	{
	public:
		virtual IFrameworkView^ CreateView();
	};
}