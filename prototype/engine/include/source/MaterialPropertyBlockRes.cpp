#include "..\MaterialPropertyBlockRes.h"
#include "..\ResourceManager.h"

namespace SystemEngine
{
	// Конструктор
	PropertyBlockRes::PropertyBlockRes(string name)
	{
		IResource::name = name;

		IResource::type = TResource::PropertyShader;

		m_device = ResourceManager::GetInstance()->GetDevice();

		for (int shader = 0; shader < 3; shader++)
		{ for (int constant = 0; constant < 4; constant++) { m_array_buffer_shaders[shader][constant] = nullptr; } }
	}

	// Деструктор
	PropertyBlockRes::~PropertyBlockRes()
	{
		m_device.reset();
	}

	// Перезагрузить
	task<void> PropertyBlockRes::Reload()
	{
		for (int count_shader = 0; count_shader < 3; count_shader++)
		{
			for (int count_constant = 0; count_constant < 4; count_constant++)
			{
				if (m_list_constants_buffer[count_shader][count_constant].size() == 0) { continue; }
				CreateConstantBuffer(m_array_buffer_shaders[count_shader][count_constant], m_list_constants_buffer[count_shader][count_constant].size());
			}
		}

		IResource::ready = true;

		return task<void>();
	}

	// Освободить
	void PropertyBlockRes::Release()
	{
		IResource::ready = false;
	}

	// Добавить свойство
	int PropertyBlockRes::AddProperty(string name, ShaderRes::TShader type_shader, TPropertyShader property_shader, TVariable type_variable, int count_variable)
	{
		int index_property = m_list_constants_buffer[(int)type_shader][(int)property_shader].size();

		int max_count_float = (int)type_variable * count_variable;

		for(int count_float = 0; count_float < max_count_float; count_float++)
		{ m_list_constants_buffer[(int)type_shader][(int)property_shader].push_back(0.0f); }
		
		pair<map<string, int>::iterator, bool> return_insert = m_list_property_name[(int)type_shader].insert(pair<string, int>(name, index_property));

		if (!return_insert.second) { return UNKNOW_INDEX_ELEMENTS; }

		return index_property;
	}

	// Обновление константных значений раз во фрейм
	bool PropertyBlockRes::UpdateChangeEveryFrame(ShaderRes::TShader type_shader)
	{
		if (IResource::ready)
		{
			switch (type_shader)
			{
			case ShaderRes::TShader::Vertex:
				if (m_array_buffer_shaders[(int)ShaderRes::TShader::Vertex][(int)TPropertyShader::ChangeEveryFrame] == nullptr) { return true; }
				m_device->GetDeviceContext()->UpdateSubresource(m_array_buffer_shaders[(int)ShaderRes::TShader::Vertex][(int)TPropertyShader::ChangeEveryFrame], 0, nullptr,
					m_list_constants_buffer[(int)ShaderRes::TShader::Vertex][(int)TPropertyShader::ChangeEveryFrame].data(), 0, 0);
				break;
			}
			return false;
		}
		else { return true; }
	}

	// Установка константных значений в массив по имени
	void PropertyBlockRes::SetConstantBufferName(ShaderRes::TShader type_shader, TPropertyShader property_shader, string name, float* value, int size)
	{
		int index = m_list_property_name[(int)type_shader][name];
		float* dsc = m_list_constants_buffer[(int)type_shader][(int)property_shader].data();
		memcpy(dsc + index, value, size);
	}

	// Установка константных значений в массив по индексу
	void PropertyBlockRes::SetConstantBufferIndex(ShaderRes::TShader type_shader, TPropertyShader property_shader, int index, float * value, int size)
	{
		float* dsc = m_list_constants_buffer[(int)type_shader][(int)property_shader].data();
		memcpy(dsc + index, value, size);
	}

	// Создание константного буфера
	bool PropertyBlockRes::CreateConstantBuffer(ID3D11Buffer*& buffer, int size, bool dynamic)
	{
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = size * sizeof(float);
		buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		buffer_desc.MiscFlags = 0;
		buffer_desc.StructureByteStride = 0;
		if (dynamic)
		{
			buffer_desc.Usage = D3D11_USAGE_DYNAMIC;
			buffer_desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		else
		{
			buffer_desc.Usage = D3D11_USAGE_DEFAULT;
			buffer_desc.CPUAccessFlags = 0;
		}

		HRESULT hr = m_device->GetDevice()->CreateBuffer(&buffer_desc, NULL, &buffer);

		if (FAILED(hr)) { return true; }

		return false;
	}

	// Установить текущий константный буфер
	bool PropertyBlockRes::IncludeConstantBuffer(ShaderRes::TShader type_shader, TPropertyShader property_shader, uint32 start_slot, uint32 count_buffers)
	{
		if (IResource::ready)
		{
			switch (type_shader)
			{
			case ShaderRes::TShader::Vertex:
				m_device->GetDeviceContext()->VSSetConstantBuffers(start_slot, count_buffers, &m_array_buffer_shaders[(int)type_shader][(int)property_shader]);
				break;
			}

			return false;
		}
		else { return true; }
	}
}