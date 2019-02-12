#pragma once

#include <agile.h>
#include <d3d11.h>
#include <ppltasks.h>
#include <memory>
#include <string>

#include <DirectXMath.h>

#include "prototype\include\Vector.h"

#include "ResourceManager.h"

using namespace AddStruct;

using namespace std;
using namespace DirectX;
using namespace Concurrency;
using namespace Microsoft::WRL;
using namespace Windows::Perception::Spatial;
using namespace Windows::Graphics::Holographic;

namespace SystemEngine
{
	class Device;

	class Camera
	{
	public:
											Camera() = delete;
		// Конструктор
											Camera(string name);
		// Деструктор
		virtual								~Camera();

		//
		void								CreateResourcesForBackBuffer(shared_ptr<Device> device_resources_ptr, HolographicCameraRenderingParameters^ camera_parameters);
		//
		void								ReleaseResourcesForBackBuffer(shared_ptr<Device> device_resources_ptr);
		//
		void								UpdateViewProjectionBuffer(shared_ptr<Device> device_resources_ptr, HolographicCameraPose^ camera_pose, SpatialCoordinateSystem^ coordinate_system);
		//
		bool								AttachViewProjectionBuffer(shared_ptr<Device> device_resources_ptr);

		// Вернуть интерфейс буфера отрисовки
		ID3D11RenderTargetView*				GetBackBufferRenderTargetView() const;
		// Вернуть интерфейс буфера трафарета и глубины
		ID3D11DepthStencilView*				GetDepthStencilView()			const;
		// Вернуть интерфейс текстуры заднего буфера
		ID3D11Texture2D*					GetBackBufferTexture2D()		const;
		// Вернуть стуктуру вида
		D3D11_VIEWPORT						GetViewport()					const;
		// Вернуть формат буфера
		DXGI_FORMAT							GetBackBufferDXGIFormat()		const;
		// Установка голографического пространства
		void								SetHolographic(HolographicCamera^ holographic_camera);
		// Возвращает матрицы проекции вида
		void								GetMatrixViewProjection(vector<Matrix4X4>& matrixs);

	private:
		// Внутренние переменные
		ComPtr<ID3D11RenderTargetView>		m_d3d_render_target_view;			//
		ComPtr<ID3D11DepthStencilView>		m_d3d_depth_stencil_view;			//
		ComPtr<ID3D11Texture2D>				m_d3d_back_buffer;					//

		ComPtr<ID3D11Buffer>				m_view_projection_constant_buffer;


		Windows::Foundation::Size			m_d3d_render_target_size;
		DXGI_FORMAT							m_dxgi_format;
		D3D11_VIEWPORT						m_d3d_viewport;

		bool								m_isStereo;

		bool								m_frame_pending;

		HolographicCamera^					m_holographic_camera;

		Matrix4X4							m_view_projection[2];
	};
}