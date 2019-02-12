#include "prototype\engine\include\ShaderRes.h"

namespace SystemEngine
{
	// Конструктор
	ShaderRes::ShaderRes(string name)
	{
		IResource::name		= "Shader " + name;

		IResource::type		= TResource::Shader;

		m_resource_manadger = ResourceManager::GetInstance();

		m_device = m_resource_manadger->GetDevice();

		m_file_manager = FileManager::GetInstance();

		m_index_file_vertex_shader		= \
		m_index_file_geometry_shader	= \
		m_index_file_pixel_shader		= UNKNOW_INDEX_ELEMENTS;
	}

	// Деструктор
	ShaderRes::~ShaderRes()
	{
		m_file_manager.reset();

		m_device.reset();

		m_resource_manadger.reset();	
	}
	
	// Перезагрузить
	task<void> ShaderRes::Reload()
	{
		task<void> create_VS_task;
		task<void> create_GS_task;
		task<void> create_PS_task;

		if (m_index_file_vertex_shader != UNKNOW_INDEX_ELEMENTS)
		{ create_VS_task = m_file_manager->LoadFileAsync(m_index_file_vertex_shader)
			.then([this](const shared_ptr<vector<byte>> shader_code) { CompiledVertex(shader_code); }); }
		
		if (m_index_file_geometry_shader != UNKNOW_INDEX_ELEMENTS)
		{ create_GS_task = m_file_manager->LoadFileAsync(m_index_file_geometry_shader)
			.then([this](const shared_ptr<vector<byte>> shader_code) { CompiledGeometry(shader_code); }); }

		if (m_index_file_pixel_shader != UNKNOW_INDEX_ELEMENTS)
		{ create_PS_task = m_file_manager->LoadFileAsync(m_index_file_pixel_shader)
			.then([this](const shared_ptr<vector<byte>> shader_code) { CompiledPixel(shader_code); }); }

		task<void> shader_task_group = (m_index_file_geometry_shader == UNKNOW_INDEX_ELEMENTS) ? (create_VS_task && create_PS_task) :
																								 (create_VS_task && create_GS_task && create_PS_task);
		return shader_task_group.then([this]()
		{
			IResource::ready = true;
		}, task_continuation_context::use_current());
	}

	// Освободить
	void ShaderRes::Release()
	{
		IResource::ready = false;
	}

	// Установка файла шейдера
	void ShaderRes::SetFile(TShader type_shader, int index_file_shader)
	{
		switch (type_shader)
		{
		case TShader::Vertex	:		m_index_file_vertex_shader		= index_file_shader; break;
		case TShader::Geometry	:		m_index_file_geometry_shader	= index_file_shader; break;
		case TShader::Pixel		:		m_index_file_pixel_shader		= index_file_shader; break;
		}
	}

	// Компиляция шейдера
	bool ShaderRes::Compiled(TShader type_shader, const shared_ptr<vector<byte>> byte_code)
	{
		HRESULT result_compilling;
		HRESULT result_create_input_layout;

		switch (type_shader)
		{
		case TShader::Vertex:
			result_compilling = m_device->GetDevice()->CreateVertexShader(byte_code->data(), byte_code->size(), nullptr, &m_vertex_shader);

			if (FAILED(result_compilling))			{ return true;  }

			result_create_input_layout = m_device->GetDevice()->CreateInputLayout(m_list_input_layout.data(), m_list_input_layout.size(), byte_code->data(), byte_code->size(), &m_input_layout);

			if (FAILED(result_create_input_layout)) { return true;  }
			else									{ return false; }

			break;
		case TShader::Geometry:
			result_compilling = m_device->GetDevice()->CreateGeometryShader(byte_code->data(), byte_code->size(), nullptr, &m_geometry_shader);
			break;
		case TShader::Pixel:
			result_compilling = m_device->GetDevice()->CreatePixelShader(byte_code->data(), byte_code->size(), nullptr, &m_pixel_shader);
			break;
		default:
			return true;
		}

		if (FAILED(result_compilling)) { return true; }

		return false;
	}

	// Установка формата вершин
	void ShaderRes::SetInputLayout(const shared_ptr<vector<D3D11_INPUT_ELEMENT_DESC>> vertex_layout)
	{
		if (vertex_layout == nullptr)
		{
			AddInputLayout("POSITION",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	 0);
			AddInputLayout("NORMAL",	0,	DXGI_FORMAT_R32G32B32_FLOAT,	12);
			AddInputLayout("TEXCOORD",	0,	DXGI_FORMAT_R32G32_FLOAT,		24);
		}
		else
		{
			m_list_input_layout = vector<D3D11_INPUT_ELEMENT_DESC>(*vertex_layout);
		}
	}
	
	// Добовление дормата вершин
	void ShaderRes::AddInputLayout(string semantic_name, int semantic_index, DXGI_FORMAT format, UINT offset)
	{
		D3D11_INPUT_ELEMENT_DESC layout_format;
		
		layout_format.SemanticName			= (new string(semantic_name.data()))->data(); // УДАЛИТЬ!!!!!!
		layout_format.SemanticIndex			= semantic_index;
		layout_format.Format				= format;
		layout_format.InputSlot				= 0;
		layout_format.AlignedByteOffset		= m_list_input_layout.empty() ? 0 : offset;
		layout_format.InputSlotClass		= D3D11_INPUT_PER_VERTEX_DATA;
		layout_format.InstanceDataStepRate	= 0;

		m_list_input_layout.push_back(layout_format);
	}

	// Установка шейдера
	bool ShaderRes::Include(TShader type_shader)
	{
		if (IResource::ready)
		{
			switch (type_shader)
			{
			case TShader::Vertex:	m_device->GetDeviceContext()->VSSetShader(m_vertex_shader.Get(), nullptr, 0);	break;
			case TShader::Geometry:	m_device->GetDeviceContext()->GSSetShader(m_geometry_shader.Get(), nullptr, 0);	break;
			case TShader::Pixel:	m_device->GetDeviceContext()->PSSetShader(m_pixel_shader.Get(), nullptr, 0);	break;
			}

			return false;
		}
		else { return true; }
	}

	// Установка привязки формата вершин
	bool ShaderRes::IncludeInputLayout()
	{
		if (IResource::ready) { m_device->GetDeviceContext()->IASetInputLayout(m_input_layout.Get());  return false; } else { return true; }
	}

	// Компиляция вершинного шейдера
	void ShaderRes::CompiledVertex(const shared_ptr<vector<byte>> byte_code_vertex)
	{
		HRESULT result_compilling = m_device->GetDevice()->CreateVertexShader(byte_code_vertex->data(), byte_code_vertex->size(), nullptr, &m_vertex_shader);
		HRESULT result_layout;

		if (SUCCEEDED(result_compilling))
		{
			result_layout = m_device->GetDevice()->CreateInputLayout(m_list_input_layout.data(), m_list_input_layout.size(), byte_code_vertex->data(), byte_code_vertex->size(), &m_input_layout);
		}
	}

	// Компиляция геометрического шейдера
	void ShaderRes::CompiledGeometry(const shared_ptr<vector<byte>> byte_code_geometry)
	{
		HRESULT result_compilling = m_device->GetDevice()->CreateGeometryShader(byte_code_geometry->data(), byte_code_geometry->size(), nullptr, &m_geometry_shader);
	}

	// Компиляция пиксельного шейдера
	void ShaderRes::CompiledPixel(const shared_ptr<vector<byte>> byte_code_pixel)
	{
		HRESULT result_compilling = m_device->GetDevice()->CreatePixelShader(byte_code_pixel->data(), byte_code_pixel->size(), nullptr, &m_pixel_shader);
	}
}