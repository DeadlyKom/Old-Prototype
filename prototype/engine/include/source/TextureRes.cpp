#include "..\TextureRes.h"

namespace SystemEngine
{
#	define DEBUG_TEXT_CREATE_TEXTURE_FALSE				L"Create texture 2D - flase"
#	define DEBUG_TEXT_CREATE_SHADER_RESOURCE_VIEW_FALSE L"Create shader resource view - false"
#	define DEBUG_TEXT_CREATE_RENDER_TARGET_VIEW_FALSE	L"Create render target view - false"
#	define DEBUG_TEXT_CREATE_DEPTH_STENSIL_VIEW_FALSE	L"Create depth stensil view - false"

	// Конструктор
	TextureRes::TextureRes(string name)
	{
		IResource::name = name;

		IResource::type = TResource::Texture;

		m_resource_manadger = SystemEngine::ResourceManager::GetInstance();

		m_device = m_resource_manadger->GetDevice();

		m_file_manager = FileManager::GetInstance();


		m_index_file_texture = UNKNOW_INDEX_ELEMENTS;

		m_width_texture		=

		m_height_texture	= 0;

		m_mip_map_levels	= 1;

		m_type_usage		= TUsage::Default;

		m_type_bind			= TBind::Default;

		m_type_texture		= TTexture::T2D;

		m_type_format		= TFormat::RGBA;

		m_texture_2D		= nullptr;

		m_texture_srv		= nullptr;

		m_texture_tv		= nullptr;

		m_sampler_state		= nullptr;

		m_texture_dsv		= nullptr;
	}

	// Деструктор
	TextureRes::~TextureRes()
	{
		m_file_manager.reset();

		m_device.reset();

		m_resource_manadger.reset();
	}

	// Перезагрузить
	task<void> TextureRes::Reload()
	{
		task<void> create_texture_task;

		if (m_index_file_texture != UNKNOW_INDEX_ELEMENTS)
		{
			create_texture_task = m_file_manager->LoadFileAsync(m_index_file_texture)
				.then([this](const shared_ptr<vector<byte>> image_data) { Create2DFromData(image_data, m_type_format); });
		}

		return create_texture_task.then([this]()
		{
			IResource::ready = true;
		}, task_continuation_context::use_current());
	}

	// Освободить
	void TextureRes::Release()
	{
		IResource::ready = false;
	}

	// Создание текстуры
	bool TextureRes::Create2D(int width, int height, TBind bind, TFormat format, bool stereo, int mip_map_levels, bool dynamic)
	{
		D3D11_TEXTURE2D_DESC texture_desc;
		
		if (mip_map_levels == -1)
		{
			int num_levels = 1;
			int w = width;
			int h = height;

			while (w > 1 && h > 1)
			{ w = max(w >> 1, 1); h = max(h >> 1, 1); num_levels++; }

			mip_map_levels = num_levels;
		}

		m_width_texture		= width;
		m_height_texture	= height;
		m_mip_map_levels	= mip_map_levels;
		m_type_format		= format;
		m_stereo			= stereo;

		texture_desc.Width				= width;
		texture_desc.Height				= height;
		texture_desc.MipLevels			= mip_map_levels;
		texture_desc.ArraySize			= 1;
		texture_desc.Format				= (DXGI_FORMAT)format;
		texture_desc.SampleDesc.Count	= 1;
		texture_desc.SampleDesc.Quality = 0;

		if (dynamic)
		{
			m_type_usage				= TUsage::Dynamic;
			texture_desc.Usage			= D3D11_USAGE_DYNAMIC;
			texture_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else
		{
			m_type_usage				= TUsage::Default;
			texture_desc.Usage			= D3D11_USAGE_DEFAULT;
			texture_desc.CPUAccessFlags = 0;
		}

		m_type_bind						= bind;
		texture_desc.BindFlags			= (D3D11_USAGE)bind;

		if(mip_map_levels != 1)
		{ texture_desc.MiscFlags		= D3D11_RESOURCE_MISC_GENERATE_MIPS; }
		else 
		{ texture_desc.MiscFlags		= 0; }

		HRESULT result_create_texture = m_device->GetDevice()->CreateTexture2D(&texture_desc, nullptr, &m_texture_2D);

		if (FAILED(result_create_texture)) { OutputDebugString(DEBUG_TEXT_CREATE_TEXTURE_FALSE); return true; }

		return false;
	}

	// Создание доступности данной текстуры в шейдере
	bool TextureRes::CreateShaderResourceThis()
	{
		HRESULT result__creat_shader_view = m_device->GetDevice()->CreateShaderResourceView(m_texture_2D.Get(), nullptr, &m_texture_srv);
		
		if (FAILED(result__creat_shader_view)) { OutputDebugString(DEBUG_TEXT_CREATE_SHADER_RESOURCE_VIEW_FALSE); return true; }

		return false;
	}

	// Создание облости отрисовки
	bool TextureRes::CreateRenderTarget()
	{
		D3D11_RENDER_TARGET_VIEW_DESC render_tv_desc;
      
		render_tv_desc.Format			= (DXGI_FORMAT)m_type_format;
		if (m_stereo)
		{
			render_tv_desc.ViewDimension					= D3D11_RTV_DIMENSION_TEXTURE2DARRAY;
			render_tv_desc.Texture2DArray.MipSlice			= 0;
			render_tv_desc.Texture2DArray.FirstArraySlice	= 0;
			render_tv_desc.Texture2DArray.ArraySize			= -1;
		}
		else
		{
			render_tv_desc.ViewDimension					= D3D11_RTV_DIMENSION_TEXTURE2D;
			render_tv_desc.Texture2D.MipSlice				= 0;
		}

		HRESULT result_create_render_tv = m_device->GetDevice()->CreateRenderTargetView(m_texture_2D.Get(), &render_tv_desc, &m_texture_tv);
		
		if (FAILED(result_create_render_tv)) { OutputDebugString(DEBUG_TEXT_CREATE_RENDER_TARGET_VIEW_FALSE); return true; }

		return false;
	}

	// Создание буфера глубины
	bool TextureRes::CreateDepthBuffer()
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC dsv_desc;

		dsv_desc.Format = (DXGI_FORMAT)m_type_format;
		
		if (m_stereo)
		{
			dsv_desc.ViewDimension						= D3D11_DSV_DIMENSION_TEXTURE2DARRAY;
			dsv_desc.Texture2DArray.MipSlice			= 0;
			dsv_desc.Texture2DArray.FirstArraySlice		= 0;
			dsv_desc.Texture2DArray.ArraySize			= -1;
		}
		else
		{
			dsv_desc.ViewDimension						= D3D11_DSV_DIMENSION_TEXTURE2D;
			dsv_desc.Texture2D.MipSlice					= 0;
		}

		dsv_desc.Flags	= 0;

		HRESULT result_create_dsv = m_device->GetDevice()->CreateDepthStencilView(m_texture_2D.Get(), &dsv_desc, &m_texture_dsv);

		if (FAILED(result_create_dsv)) { OutputDebugString(DEBUG_TEXT_CREATE_DEPTH_STENSIL_VIEW_FALSE); return true; }

		return false;
	}

	// Установка файла текстуры
	void TextureRes::SetFile(TTexture type_texture, int index_file_shader, TFormat format)
	{
		m_type_texture = type_texture;

		m_index_file_texture = index_file_shader;

		m_type_format = format;
	}

	// Создание 1D текструры
	bool TextureRes::Create1D(int width, vector<Color32>* rgba)
	{
		return false;
	}

	// Создание 1D текстуры
	bool TextureRes::Create1DFromData(shared_ptr<vector<byte>> image_data, TFormat format)
	{
		return false;
	}

	// Создание 2D текструры
	bool TextureRes::Create2D(int width, int height, vector<Color32>* rgba)
	{
		return false;
	}

	// Создание 2D текстуры
	bool TextureRes::Create2DFromData(shared_ptr<vector<byte>> image_data, TFormat format)
	{
		size_t converter = Converter_Create();

		Setup(converter, CSU_NEGATIVE_Y | CSU_NOT_DDS_HADER);

		bool result_dicompress_image = SetDataFromFileInMemory(converter, image_data->data(), ConverterTypeData::CTD_FILE_UNKNOW, image_data->size());

		if (!result_dicompress_image) { Converter_Release(converter); return true; }

		int	size_buffer = GetOutputBufferSize(converter);

		byte* output_buffer = new byte[size_buffer];

		bool result_compres_to_DXT = CompressToDXT(converter, output_buffer);

		if (!result_compres_to_DXT) { delete[] output_buffer; Converter_Release(converter); return true; }

		int width_texture = GetWidth(converter);

		int height_texture = GetHeight(converter);

		Converter_Release(converter);

		D3D11_SUBRESOURCE_DATA resource_data;
		memset(&resource_data, 0, sizeof(resource_data));

		resource_data.pSysMem			= (const void*)output_buffer;
		switch (format)
		{
		case TFormat::BC1:
			size_t numBlocksWide = 0;
			if (width_texture > 0)		{ numBlocksWide = max<size_t>(1, (width_texture  + 3) / 4); }
			size_t numBlocksHigh = 0;
			if (height_texture > 0)		{ numBlocksHigh = max<size_t>(1, (height_texture + 3) / 4); }
			resource_data.SysMemPitch		= numBlocksWide * 8;
			resource_data.SysMemSlicePitch	= numBlocksHigh;
			break;
		}

		D3D11_TEXTURE2D_DESC texture_desc;
		memset(&texture_desc, 0, sizeof(texture_desc));

		texture_desc.Width				= width_texture;
		texture_desc.Height				= height_texture;
		texture_desc.MipLevels			= 1;
		texture_desc.ArraySize			= 1;
		switch (format)
		{
		case TFormat::RGBA: texture_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	break;
		case TFormat::BC1:	texture_desc.Format = DXGI_FORMAT_BC1_UNORM;		break;
		case TFormat::BC5:	texture_desc.Format = DXGI_FORMAT_BC5_UNORM;		break;
		}
		texture_desc.SampleDesc.Count	= 1;
		texture_desc.SampleDesc.Quality = 0;
		texture_desc.Usage				= D3D11_USAGE_DEFAULT;
		texture_desc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
		texture_desc.CPUAccessFlags		= 0;
		
		HRESULT result_create_texture = m_device->GetDevice()->CreateTexture2D(&texture_desc, &resource_data, &m_texture_2D);

		if (FAILED(result_create_texture)) { OutputDebugString(DEBUG_TEXT_CREATE_TEXTURE_FALSE); delete[] output_buffer; return false; }

		delete[] output_buffer;

		D3D11_SHADER_RESOURCE_VIEW_DESC shader_resource_view_desc;
		memset(&shader_resource_view_desc, 0, sizeof(shader_resource_view_desc));

		shader_resource_view_desc.Format				= texture_desc.Format;
		shader_resource_view_desc.ViewDimension			= D3D_SRV_DIMENSION_TEXTURE2D;
		shader_resource_view_desc.Texture2D.MipLevels	= 1;

		HRESULT result_create_recource = m_device->GetDevice()->CreateShaderResourceView(m_texture_2D.Get(), &shader_resource_view_desc, &m_texture_srv);

		if (FAILED(result_create_recource)) { OutputDebugString(DEBUG_TEXT_CREATE_SHADER_RESOURCE_VIEW_FALSE); return true; }

		m_width_texture = width_texture;

		m_height_texture = height_texture;

		return false;
	}

	// Установить данный текстурный ресурс
	bool TextureRes::IncludeShaderResource(uint32 start_slot, uint32 count_buffers)
	{
		if (IResource::ready)
		{
			m_device->GetDeviceContext()->PSSetShaderResources(start_slot, count_buffers, m_texture_srv.GetAddressOf());

			return false;
		}
		else { return true; }
	}

	// Возвращает ссылка на интерфейс области отрисовки
	ID3D11RenderTargetView* TextureRes::GetRenderTarget(bool recreate)
	{
		if (recreate && m_texture_tv == nullptr) { CreateRenderTarget(); }

		return m_texture_tv.Get();
	}

	// Возвращает ссылку на интерфейс буфера глубины и трафарета
	ID3D11DepthStencilView * TextureRes::GetDepthBuffer(bool recreate)
	{
		if (recreate && m_texture_dsv == nullptr) { CreateDepthBuffer(); }

		return m_texture_dsv.Get();
	}

	// Возвращает ссылку на интерфейс ресурса шейдера
	ID3D11ShaderResourceView * TextureRes::GetShaderResource(bool recreate)
	{
		if (recreate && m_texture_srv == nullptr) { CreateShaderResourceThis(); }

		return m_texture_srv.Get();
	}
}