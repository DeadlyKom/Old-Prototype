#include <WindowsNumerics.h>

#include "prototype\engine\include\Camera.h"
#include "prototype\engine\include\Device.h"

namespace SystemEngine
{
	// Конструктор
	Camera::Camera(string name)
	{
		//IResource::name = name;

		//IResource::type = TResource::Camera;
	}

	// Деструктор
	Camera::~Camera()
	{
	}

	//
	void Camera::CreateResourcesForBackBuffer(shared_ptr<Device> device_resources_ptr, HolographicCameraRenderingParameters^ camera_parameters)
	{
		const auto device = device_resources_ptr->GetDevice();

		// Получим поверхность голографической камеры
		IDirect3DSurface^ surface = camera_parameters->Direct3D11BackBuffer;

		ComPtr<ID3D11Resource> resource;

		HRESULT result_get_resource = GetDXGIInterfaceFromObject(surface, IID_PPV_ARGS(&resource));

		if (FAILED(result_get_resource)) {
			int a = 10;
		}

		ComPtr<ID3D11Texture2D> camera_back_buffer;

		HRESULT result_cast_back_buffer = resource.As(&camera_back_buffer);
		
		if (FAILED(result_cast_back_buffer)) {
			int a = 10;
		}

		if (m_d3d_back_buffer.Get() != camera_back_buffer.Get())
		{
			m_d3d_back_buffer = camera_back_buffer;

			HRESULT result_create_render_target = device->CreateRenderTargetView(m_d3d_back_buffer.Get(), nullptr, &m_d3d_render_target_view);
			
			if (FAILED(result_create_render_target)) {
				int a = 10;
			}

			D3D11_TEXTURE2D_DESC back_buffer_desc;

			m_d3d_back_buffer->GetDesc(&back_buffer_desc);

			m_dxgi_format = back_buffer_desc.Format;

			Windows::Foundation::Size current_size = m_holographic_camera->RenderTargetSize;

			if (m_d3d_render_target_size != current_size)
			{
				m_d3d_render_target_size = current_size;

				m_d3d_depth_stencil_view.Reset();
			}
		}

		if (m_d3d_depth_stencil_view == nullptr)
		{
			CD3D11_TEXTURE2D_DESC depth_stencil_desc(DXGI_FORMAT_D16_UNORM, static_cast<uint32>(m_d3d_render_target_size.Width),
				static_cast<uint32>(m_d3d_render_target_size.Height), m_isStereo ? 2 : 1, 1, D3D11_BIND_DEPTH_STENCIL);

			ComPtr<ID3D11Texture2D> depth_stencil;
			
			HRESULT result_create_texture = device->CreateTexture2D(&depth_stencil_desc, nullptr, &depth_stencil);
			
			if (FAILED(result_create_texture)) {
				int a = 10;
			}

			CD3D11_DEPTH_STENCIL_VIEW_DESC depth_stencil_view_desc(m_isStereo ? D3D11_DSV_DIMENSION_TEXTURE2DARRAY : D3D11_DSV_DIMENSION_TEXTURE2D);
			
			HRESULT result_create_DSV = device->CreateDepthStencilView(depth_stencil.Get(), &depth_stencil_view_desc, &m_d3d_depth_stencil_view);
			
			if (FAILED(result_create_DSV)) {
				int a = 10;
			}
		}

		if (m_view_projection_constant_buffer == nullptr)
		{
			CD3D11_BUFFER_DESC constant_buffer_desc(sizeof(DirectX::XMFLOAT4X4) * 2, D3D11_BIND_CONSTANT_BUFFER);

			HRESULT result_create_view_projection_constant_buffer = device->CreateBuffer(&constant_buffer_desc, nullptr, &m_view_projection_constant_buffer);
			
			if (FAILED(result_create_view_projection_constant_buffer)) {
				int a = 10;
			}
		}

	}

	//
	void Camera::ReleaseResourcesForBackBuffer(shared_ptr<Device> device_resources_ptr)
	{
		const auto context = device_resources_ptr->GetDeviceContext();

		m_d3d_back_buffer.Reset();
		m_d3d_render_target_view.Reset();
		m_d3d_depth_stencil_view.Reset();

		ID3D11RenderTargetView* nullViews[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT] = { nullptr };
		context->OMSetRenderTargets(ARRAYSIZE(nullViews), nullViews, nullptr);
		context->Flush();
	}

	//
	void Camera::UpdateViewProjectionBuffer(shared_ptr<Device> device_resources_ptr, HolographicCameraPose^ camera_pose, SpatialCoordinateSystem^ coordinate_system)
	{
		//
		m_d3d_viewport = CD3D11_VIEWPORT(camera_pose->Viewport.Left, camera_pose->Viewport.Top, camera_pose->Viewport.Width, camera_pose->Viewport.Height);
		//
		HolographicStereoTransform camera_projection_transform = camera_pose->ProjectionTransform;
		//
		Platform::IBox<HolographicStereoTransform>^ view_transform_container = camera_pose->TryGetViewTransform(coordinate_system);
		//
		bool view_transform_acquired = view_transform_container != nullptr;
		//
		if (view_transform_acquired)
		{
			//
			HolographicStereoTransform view_coordinate_system_transform = view_transform_container->Value;
			//
			XMStoreFloat4x4((XMFLOAT4X4*)&m_view_projection[0], XMMatrixTranspose(DirectX::XMLoadFloat4x4(&view_coordinate_system_transform.Left) * DirectX::XMLoadFloat4x4(&camera_projection_transform.Left)));
			//
			XMStoreFloat4x4((XMFLOAT4X4*)&m_view_projection[1], XMMatrixTranspose(DirectX::XMLoadFloat4x4(&view_coordinate_system_transform.Right) * DirectX::XMLoadFloat4x4(&camera_projection_transform.Right)));
		}
		//
		const auto context = device_resources_ptr->GetDeviceContext();


		if ((context == nullptr) || (m_view_projection_constant_buffer == nullptr) || !view_transform_acquired)		{ m_frame_pending = false; }
		else
		{
			context->UpdateSubresource(m_view_projection_constant_buffer.Get(), 0, nullptr, &m_view_projection, 0, 0);

			m_frame_pending = true;
		}
	}

	//
	bool Camera::AttachViewProjectionBuffer(shared_ptr<Device> device_resources_ptr)
	{
		const auto context = device_resources_ptr->GetDeviceContext();

		if (context == nullptr || m_view_projection_constant_buffer == nullptr || m_frame_pending == false) { return false; }

		context->RSSetViewports(1, &m_d3d_viewport);

		context->VSSetConstantBuffers(1, 1, m_view_projection_constant_buffer.GetAddressOf());

		m_frame_pending = false;

		return true;
	}

	// Вернуть интерфейс буфера отрисовки
	ID3D11RenderTargetView* Camera::GetBackBufferRenderTargetView() const
	{
		return m_d3d_render_target_view.Get();
	}

	// Вернуть интерфейс буфера трафарета и глубины
	ID3D11DepthStencilView* Camera::GetDepthStencilView() const
	{
		return m_d3d_depth_stencil_view.Get();
	}

	// Вернуть интерфейс текстуры заднего буфера
	ID3D11Texture2D* Camera::GetBackBufferTexture2D() const
	{
		return m_d3d_back_buffer.Get();
	}

	// Вернуть стуктуру вида
	D3D11_VIEWPORT Camera::GetViewport() const
	{
		return m_d3d_viewport;
	}

	// Вернуть формат буфера
	DXGI_FORMAT Camera::GetBackBufferDXGIFormat() const
	{
		return m_dxgi_format;
	}

	// Установка голографического пространства
	void Camera::SetHolographic(HolographicCamera^ holographic_camera)
	{
		m_frame_pending = false;

		m_isStereo = holographic_camera->IsStereo;

		m_holographic_camera = holographic_camera;

		m_d3d_render_target_size = holographic_camera->RenderTargetSize;

		m_d3d_viewport = CD3D11_VIEWPORT(0.f, 0.f, m_d3d_render_target_size.Width, m_d3d_render_target_size.Height);
	}

	// Возвращает матрицы проекции вида
	void Camera::GetMatrixViewProjection(vector<Matrix4X4>& matrixs)
	{
		matrixs.clear();
		matrixs.push_back(m_view_projection[0]);
		matrixs.push_back(m_view_projection[1]);
	}
}