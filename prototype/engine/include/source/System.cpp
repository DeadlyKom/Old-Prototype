#include "prototype\engine\include\ResourceManager.h"
#include "prototype\engine\include\System.h"
#include "prototype\engine\include\ShaderRes.h"

#include "ProjectAssets\InitAsset.h"

namespace SystemEngine
{
	IFrameworkView^ SystemSource::CreateView() { return ref new System(); }

	// Конструктор
	System::System()
	{
		// Окно видно
		m_window_visible	= false;
		// Окно не закрыто
		m_window_closed		= false;
		//
		m_holographic_space = nullptr;


		//
		m_index_device		= UNKNOW_INDEX_ELEMENTS;
		//
		m_first_reload_resource = false;
	}

	// Инициализация приложения
	void System::Initialize(CoreApplicationView^ applicationView)
	{
		// Добавим обработчик активации приложения
		applicationView->Activated	+= ref new Windows::Foundation::TypedEventHandler<CoreApplicationView^, IActivatedEventArgs^>(this, &System::OnViewActivated);
		// Добавим обработчик засыпания приложения
		CoreApplication::Suspending	+= ref new Windows::Foundation::EventHandler<SuspendingEventArgs^>(this, &System::OnSuspending);
		// Добавим обработчик пробуждения приложения
		CoreApplication::Resuming	+= ref new Windows::Foundation::EventHandler<Platform::Object^>(this, &System::OnResuming);
		//
		m_loader_assets.BindingAssetCameraExecute(MAIN_CAMERA, asset_main_camera_execute);
		//
		m_loader_assets.BindingAssetCameraRelease(MAIN_CAMERA, asset_main_camera_release);
		//
		m_resource_manadger = SystemEngine::ResourceManager::GetInstance();
		//
		m_device			= m_resource_manadger->CreateResourcePtr<Device>(MAIN_DEVICE, m_index_device);
		//
		m_editor			= make_shared<Editor>();
		//
		m_main_loop			= make_shared<MainLoop>();
	}

	// Установки текущего окна
	void System::SetWindow(CoreWindow^ window)
	{
		// Регистрация обработчика закрытия окна
		window->Closed			   += ref new TypedEventHandler<CoreWindow^, CoreWindowEventArgs^>(this, &System::OnWindowClosed);
		// Регистрация обработчика изменения фокуса окна
		window->VisibilityChanged  += ref new TypedEventHandler<CoreWindow^, VisibilityChangedEventArgs^>(this, &System::OnVisibilityChanged);
		// Создание голографического окна
		m_holographic_space			= HolographicSpace::CreateForCoreWindow(window);
		// Получим диспетчер устройств
		m_device_manager = DeviceManager::GetInstance();
		// Создадим устройство
		m_device_manager->CreateDevice();
		// Создадим наше пространство
		//m_holo_space = make_shared<HoloSpace>();
		// Установим загрузчик асетов для камер
		//m_holo_space->SetLoaderAsset(m_loader_assets);
		// Укажем с какими устройствами нужно будет сопостовлять камыры
		//m_holo_space->SetDeviceNames();
		// Привяжим текущее пространство с нашим оброботчиком
		//m_holo_space->SetHolographicSpace(m_holographic_space);
		////
		//m_device_manager->GetDevice()->SetHolographicSpace(m_holographic_space);


		// Привязка голографического пространства к устройству
		m_resource_manadger->GetResource<Device>(m_index_device)->SetHolographicSpace(m_holographic_space);
		//
		m_main_loop->SetHolographicSpace(m_holographic_space);
		//
	}

	// Загрузка внешних ресурсов
	void System::Load(String^ entryPoint)
	{
		// Инициализация (мнимого редактора)
		if (!m_editor->Initialization())
		{
			//// Создадим поток инициализации всех ресурсов
			//create_task([this]()
			///*task<void>([this]()*/
			//{
			//	// Получим список всех ресурсов
			//	vector<shared_ptr<IResource>> resources = *m_resource_manadger->GetResources();
			//	// Пройдёмся по каждому и проинициализируем его
			//	for (auto& it_resource : resources)
			//	{
			//		it_resource->Reload();
			//	}
			//}), task_continuation_context::use_current();
		}
	}

	// Запуск приложения
	void System::Run()
	{
		static bool task_run = false;

		while (!m_window_closed)
		{
			if (m_window_visible && (m_holographic_space != nullptr))
			{
				CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessAllIfPresent);

				HolographicFrame^ holographic_frame = m_main_loop->Update();

				if (!task_run)
				{
					// Создадим поток инициализации всех ресурсов
					create_task([this]()
					{
						// Получим список всех ресурсов
						vector<shared_ptr<IResource>> resources = *m_resource_manadger->GetResources();
						// Пройдёмся по каждому и проинициализируем его
						for (auto& it_resource : resources)
						{
							it_resource->Reload();
						}

						m_first_reload_resource = true;

					}), task_continuation_context::use_current();

					task_run = true;
				}

				if (m_first_reload_resource)
				{
					if (m_main_loop->Renderer(holographic_frame))
					{
						m_device->Present(holographic_frame);
					}
				}
			}
			else
			{
				CoreWindow::GetForCurrentThread()->Dispatcher->ProcessEvents(CoreProcessEventsOption::ProcessOneAndAllPending);
			}
		}
	}

	// Особождение ранее загруженых ресурсов
	void System::Uninitialize()
	{
		m_resource_manadger->DeleteResource(m_index_device);

		m_resource_manadger.reset();
	}

	// Обработчик активации приложения
	void System::OnViewActivated(CoreApplicationView^ sender, IActivatedEventArgs^ args)
	{
		// Запуск приложения не начнётся пока CoreWindow не станет активным
		sender->CoreWindow->Activate();
	}

	// Обработчик приостановки приложения
	void System::OnSuspending(Object^ sender, SuspendingEventArgs^ args)
	{
		SuspendingDeferral^ deferral = args->SuspendingOperation->GetDeferral();

		create_task([this, deferral]()
		{
			m_device->Trim();

			if (m_main_loop != nullptr)
			{
				//m_main_loop->SaveAppState();
			}

			deferral->Complete();
		});

	}

	// Обработчик возабновления приложения
	void System::OnResuming(Object^ sender, Object^ args)
	{
	}

	// Обработчик изменения видимости
	void System::OnVisibilityChanged(CoreWindow^ sender, VisibilityChangedEventArgs^ args)
	{
		m_window_visible = args->Visible;
	}

	// Обработчик закрытия приложения
	void System::OnWindowClosed(CoreWindow^ sender, CoreWindowEventArgs^ args)
	{
		m_window_closed = true;
	}
}