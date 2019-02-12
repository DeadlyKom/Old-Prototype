#include <Collection.h>
#include <DirectXColors.h>

#include "prototype\include\Behavior.h"
#include "prototype\include\Material.h"
#include "prototype\include\Mesh.h"
#include "prototype\include\MeshFilter.h"
#include "prototype\include\MeshRender.h"
#include "prototype\include\SceneObject.h"
#include "prototype\include\Shader.h"
#include "prototype\include\Sprite.h"
#include "prototype\include\SpriteRender.h"
#include "prototype\include\Transform.h"

#include "prototype\engine\include\IndexRes.h"
#include "prototype\engine\include\MainLoop.h"
#include "prototype\engine\include\MaterialPropertyBlockRes.h"
#include "prototype\engine\include\ShaderRes.h"
#include "prototype\engine\include\VerticesRes.h"

using namespace DirectX;
using namespace Windows::Perception::Spatial;
using namespace Windows::UI::Input::Spatial;

namespace SystemEngine
{
	// Конструктор по умолчанию
	MainLoop::MainLoop()
	{
		m_resource_manadger = ResourceManager::GetInstance();
		
		m_device = m_resource_manadger->GetDevice();

		m_object_registry = ObjectRegistry::GetInstance();
	}

	// Деструктор
	MainLoop::~MainLoop()
	{
		m_object_registry.reset();

		m_device.reset();

		m_resource_manadger.reset();

		UnregisterHolographicEventHandlers();
	}

	// Установка голографического пространства
	void MainLoop::SetHolographicSpace(HolographicSpace^ holographic_space)
	{
		//
		UnregisterHolographicEventHandlers();
		//
		m_holographic_space = holographic_space;
		// Используем значение пространственного указателя устройства по умолчанию
		m_locator = SpatialLocator::GetDefault();
		// Укажем обработчик изменения позиции
		m_locatability_changed_token	= m_locator->LocatabilityChanged		+= ref new Windows::Foundation::TypedEventHandler<SpatialLocator^, Object^>(bind(&MainLoop::OnLocatabilityChanged, this, _1, _2) );
		//
		m_camera_added_token			= m_holographic_space->CameraAdded		+= ref new Windows::Foundation::TypedEventHandler<HolographicSpace^, HolographicSpaceCameraAddedEventArgs^>(bind(&MainLoop::OnCameraAdded, this, _1, _2) );
		//
		m_camera_removed_token			= m_holographic_space->CameraRemoved	+= ref new Windows::Foundation::TypedEventHandler<HolographicSpace^, HolographicSpaceCameraRemovedEventArgs^>(bind(&MainLoop::OnCameraRemoved, this, _1, _2) );
		
		m_reference_frame				= m_locator->CreateStationaryFrameOfReferenceAtCurrentLocation();

		m_device->RegisterEvents(shared_from_this());
		//
		m_input = make_shared<InputHandler>();


		//D3D11_RASTERIZER_DESC desc;
		//ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
		//desc.FillMode = D3D11_FILL_WIREFRAME;
		//desc.CullMode = D3D11_CULL_NONE;
		//m_device->GetDevice()->CreateRasterizerState(&desc, &WireFrame);
	}

	// Обновления элементов из списка объектов сцены
	HolographicFrame^ MainLoop::Update()
	{
		HolographicFrame^ holographic_frame = m_holographic_space->CreateNextFrame();

		HolographicFramePrediction^ prediction = holographic_frame->CurrentPrediction;

		m_device->EnsureCameraResources(holographic_frame, prediction);

		SpatialCoordinateSystem^ current_coordinate_system = m_reference_frame->CoordinateSystem;

		SpatialInteractionSourceState^ pointer_state = m_input->CheckForInput();

		if (pointer_state != nullptr)
		{
			//m_spinningCubeRenderer->PositionHologram(pointer_state->TryGetPointerPose(current_coordinate_system));
		}


		for (auto camera_pose : prediction->CameraPoses)
		{
			HolographicCameraRenderingParameters^ rendering_parameters = holographic_frame->GetRenderingParameters(camera_pose);

		}

		// ----------------------------------------------------------------------------------------------------
	

		// Получим список объектов сцены
		vector<shared_ptr<ScriptEngine::SceneObject>>* list_scene_objects = m_object_registry->GetListSceneObjects();
		// Цикл обхода всех элементов 
		for (auto& it_list_SO : *list_scene_objects)
		{
			// Если текущий элемент пустой, перейдём к следующему
			if (it_list_SO == nullptr) { continue; }
			// Получим список скриптовых компонентов
			shared_ptr<vector<shared_ptr<ScriptEngine::Behavior>>> list_script_components = (*it_list_SO).GetComponents<ScriptEngine::Behavior>();
			// Проверим на успешность формирования данного списка (если он пуст, перейдём к следующему ...)
			if (list_script_components != nullptr)
			{
				// Цикл обхода всех элементов скриптов
				for (auto& it_list_script : *list_script_components)
				{
					it_list_script->CallCurrentFunction();
				}
			}
		}
		// ----------------------------------------------------------------------------------------------------


		// Вернём текущий голографический фрейи для дальнейших расчётов
		return holographic_frame;
	}

	// Отображение элементов из списка объектов сцене
	bool MainLoop::Renderer(HolographicFrame^ holographic_frame)
	{
		return m_device->UseHolographicCameraResources<bool>(
			[this, holographic_frame](map<uint32, unique_ptr<Camera>>& camera_resource_map)
		{
			static DX::StepTimer timer;

			holographic_frame->UpdateCurrentPrediction();

			HolographicFramePrediction^ prediction = holographic_frame->CurrentPrediction;

			bool atLeastOneCameraRendered = false;

			for (auto camera_pose : prediction->CameraPoses)
			{
				Camera* camera_resources_ptr = camera_resource_map[camera_pose->HolographicCamera->Id].get();

				const auto context = m_device->GetDeviceContext();
				const auto depth_stencil_view = camera_resources_ptr->GetDepthStencilView();

				ID3D11RenderTargetView *const targets[1] = { camera_resources_ptr->GetBackBufferRenderTargetView() };
				context->OMSetRenderTargets(1, targets, depth_stencil_view);

				context->ClearRenderTargetView(targets[0], DirectX::Colors::Transparent);
				context->ClearDepthStencilView(depth_stencil_view, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

				camera_resources_ptr->UpdateViewProjectionBuffer(m_device, camera_pose, m_reference_frame->CoordinateSystem);

				bool cameraActive = camera_resources_ptr->AttachViewProjectionBuffer(m_device);

				if (cameraActive)
				{
					// ----------------------------------------------------------------------------------------------------
					
					//m_device->GetDeviceContext()->RSSetState(WireFrame);
					
					uint64 old = timer.GetTicks();

					RendererManager::GetInstance()->Render();

					uint64 new_ = timer.GetTicks();
			
					uint64 delta = new_ - old;
					wstring data = to_wstring(delta);

					OutputDebugString((data + L"\n").data());
		
					// ----------------------------------------------------------------------------------------------------

					// Visual Studio 2017
					//ComPtr<ID3D11Texture2D> depthBuffer = camera_resource_map.at(camera_pose->HolographicCamera->Id)->GetBackBufferTexture2D();/// GetDepthBufferTexture2D();
					//ComPtr<IDXGIResource1> depthStencilResource;
					//HRESULT hr = depthBuffer.As(&depthStencilResource);
					//ComPtr<IDXGISurface2> depthDxgiSurface;
					//if (SUCCEEDED(hr))
					//{
					//	hr = depthStencilResource->CreateSubresourceSurface(0, &depthDxgiSurface);
					//}
					//if (SUCCEEDED(hr))
					//{
					//	IDirect3DSurface^ depthD3DSurface = CreateDirect3DSurface(depthDxgiSurface.Get());
					//	Windows::Graphics::Holographic::HolographicCameraRenderingParameters^ renderingParameters = holographic_frame->GetRenderingParameters(camera_pose);
					//	try
					//	{
					//		// Provide the depth buffer.
					//		renderingParameters->CommitDirect3D11DepthBuffer(depthD3DSurface);
					//	}
					//	catch (Platform::InvalidArgumentException^ ex)
					//	{
					//		OutputDebugStringA("Unsupported depth buffer format, invalid properties, or incorrect D3D device.\n");
					//	}
					//}

				}
				atLeastOneCameraRendered = true;
			}

			return atLeastOneCameraRendered;
		});
	}

	// Обработчик потерянного устройства
	void MainLoop::OnDeviceLost()
	{
	}

	// Обработчик перезагрузки устройства
	void MainLoop::OnDeviceRestored()
	{
	}

	// Обработчик асинхронного создания ресурсов для новых голографических камер
	void MainLoop::OnCameraAdded(HolographicSpace^ sender, HolographicSpaceCameraAddedEventArgs^ args)
	{
		//
		Deferral^ deferral = args->GetDeferral();
		//
		HolographicCamera^ holographic_camera = args->Camera;
		//
		create_task([this, deferral, holographic_camera]()
		{
			// ----------------------------------------
			// Загрузка ресурсов для новой камеры 
			// ----------------------------------------

			// Добавим голографическую камеру к устройству
			m_device->AddHolographicCamera(holographic_camera);
			// Не производить установку камеры до завершения
			deferral->Complete();
		});

	}

	// Обработчик синхронного освобождения ресурсов для уставевших каммер
	void MainLoop::OnCameraRemoved(HolographicSpace^ sender, HolographicSpaceCameraRemovedEventArgs^ args)
	{
		create_task([this]()
		{
			// ----------------------------------------
			// Асинхроннай выгрузка ресурсов (кроме back buffer)
			// ----------------------------------------
		});
		//
		m_device->RemoveHolographicCamera(args->Camera);
	}

	// Обработчик изменения позиции
	void MainLoop::OnLocatabilityChanged(SpatialLocator^ sender, Object^ args)
	{
		auto a = sender->Locatability;
		switch (sender->Locatability)
		{
		// Голограмма не может быть отображена
		case SpatialLocatability::Unavailable:
		// Подготовка к отслеживанию позиции
		case SpatialLocatability::PositionalTrackingActivating:
		// Отслеживание позиции небыло активированно
		case SpatialLocatability::OrientationOnly:
		// Отслеживание позиции временно запрещено (требуется действия со стороны пользователя)
		case SpatialLocatability::PositionalTrackingInhibited:
		// Отслеживание позиции активно (пространство заблокированно, можно отобразить)
		case SpatialLocatability::PositionalTrackingActive:
			break;
		}

	}

	// Освобождение ранее зарегистрированных обработчиков событий
	void MainLoop::UnregisterHolographicEventHandlers()
	{
		if (m_holographic_space != nullptr)
		{
			// Очистка ранее регестрированных обработчиков
			if (m_camera_added_token.Value != 0)
			{
				m_holographic_space->CameraAdded -= m_camera_added_token;
				m_camera_added_token.Value = 0;
			}
			//
			if (m_camera_removed_token.Value != 0)
			{
				m_holographic_space->CameraRemoved -= m_camera_removed_token;
				m_camera_removed_token.Value = 0;
			}
		}

		if (m_locator != nullptr)
		{
			m_locator->LocatabilityChanged -= m_locatability_changed_token;
		}
	}
}