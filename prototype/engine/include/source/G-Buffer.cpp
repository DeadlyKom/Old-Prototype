#include "..\G-Buffer.h"
namespace SystemEngine
{
	// Конструктор
	GBuffer::GBuffer()
	{
		m_resource_manadger = ResourceManager::GetInstance();

		m_device = m_resource_manadger->GetDevice();
	}

	// Деструктор
	GBuffer::~GBuffer()
	{
		m_device.reset();

		m_resource_manadger.reset();
	}

	// Создание буфера геометрии
	bool GBuffer::Create(int width, int height)
	{
		m_textures[Buffer::Diffuse]		= make_shared<TextureRes>(NAME_G_BUFFER_DIFFUSE);
		m_textures[Buffer::Diffuse]->Create2D(width, height, TextureRes::TBind::RenderTarget, TextureRes::TFormat::RGBA, true, -1);
		m_textures[Buffer::Diffuse]->CreateRenderTarget();
		m_textures[Buffer::Diffuse]->CreateShaderResourceThis();

		m_textures[Buffer::Normal]		= make_shared<TextureRes>(NAME_G_BUFFER_NORMAL);
		m_textures[Buffer::Normal]->Create2D(width, height, TextureRes::TBind::RenderTarget, TextureRes::TFormat::R11G11B10, true);
		m_textures[Buffer::Normal]->CreateRenderTarget();
		m_textures[Buffer::Normal]->CreateShaderResourceThis();

		m_textures[Buffer::Specular]	= make_shared<TextureRes>(NAME_G_BUFFER_SPECULAR);
		m_textures[Buffer::Specular]->Create2D(width, height, TextureRes::TBind::RenderTarget, TextureRes::TFormat::RGBA, true);
		m_textures[Buffer::Specular]->CreateRenderTarget();
		m_textures[Buffer::Specular]->CreateShaderResourceThis();

		m_textures[Buffer::Depth]		= make_shared<TextureRes>(NAME_G_BUFFER_DEPTH);
		m_textures[Buffer::Depth]->Create2D(width, height, TextureRes::TBind::DepthBuffer, TextureRes::TFormat::D16, true);
		m_textures[Buffer::Depth]->CreateDepthBuffer();
		m_textures[Buffer::Depth]->CreateShaderResourceThis();

		return false;
	}

	// Очистка буферов цвета и глубины
	void GBuffer::ClearDiffuseDepth(ColorFloat diffuse)
	{
		m_device->GetDeviceContext()->ClearRenderTargetView(m_textures[Buffer::Diffuse]->GetRenderTarget(), (float*)&diffuse);

		m_device->GetDeviceContext()->ClearDepthStencilView(m_textures[Buffer::Depth]->GetDepthBuffer(), D3D11_CLEAR_DEPTH, 1.0f, 0);
	}

	// Связывание буферов с областью отображения
	void GBuffer::IncludeBuffersRenderTargets()
	{
		ID3D11RenderTargetView* render_target_views[3]	{ m_textures[Buffer::Diffuse]->GetRenderTarget(),
														  m_textures[Buffer::Normal]->GetRenderTarget(),
														  m_textures[Buffer::Specular]->GetRenderTarget() };
		m_device->GetDeviceContext()->OMSetRenderTargets(3, render_target_views, m_textures[Buffer::Depth]->GetDepthBuffer());
	}

	// Связывание текстур с пиксельным шейдером
	void GBuffer::IncludePixelShader()
	{
		ID3D11ShaderResourceView* shader_resource_views[4]	{ m_textures[Buffer::Diffuse]->GetShaderResource(),
															  m_textures[Buffer::Normal]->GetShaderResource(),
															  m_textures[Buffer::Specular]->GetShaderResource(),
															  m_textures[Buffer::Depth]->GetShaderResource()		};

		m_device->GetDeviceContext()->PSSetShaderResources(0, 4, shader_resource_views);
	}

	// Убрать связывание с пиксельным шейдером
	void GBuffer::UnincludedPixelShader()
	{
		ID3D11ShaderResourceView* shader_resource_views_null[4] { nullptr,nullptr,nullptr,nullptr };
		m_device->GetDeviceContext()->PSSetShaderResources(0, 4, shader_resource_views_null);
	}
}