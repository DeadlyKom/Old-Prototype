#include "prototype\engine\include\IndexRes.h"

namespace SystemEngine
{
	// Конструктор
	IndexRes::IndexRes(string name)
	{
		IResource::name = name;

		IResource::type = TResource::Index;

		m_resource_manadger = SystemEngine::ResourceManager::GetInstance();

		m_device = m_resource_manadger->GetDevice();

		m_primitive_topology = (D3D11_PRIMITIVE_TOPOLOGY)typePrimitive::Triangle;
	}

	// Деструктор
	IndexRes::~IndexRes()
	{
		m_device.reset();

		m_resource_manadger.reset();
	}
	
	// Перезагрузить
	task<void> IndexRes::Reload()
	{
		CreateIndexBuffer(m_triangles);
		
		IResource::ready = true;

		return task<void>();
	}

	// Освободить
	void IndexRes::Release()
	{
		IResource::ready = false;
	}

	// Устанавливает массив индексов (обхода треугольников) в указанный индекс
	void IndexRes::SetTriangles(shared_ptr<vector<uint16>> triangles)
	{
		m_triangles = triangles;
	}

	// Создание индексного буфера
	bool IndexRes::CreateIndexBuffer(shared_ptr<vector<uint16>> source_buffer_ptr, bool dynamic)
	{
		//
		D3D11_BUFFER_DESC buffer_desc;
		buffer_desc.ByteWidth = source_buffer_ptr->size() * sizeof(uint16);
		buffer_desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
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
		//
		D3D11_SUBRESOURCE_DATA data;
		data.pSysMem = source_buffer_ptr->data();
		data.SysMemPitch = 0;
		data.SysMemSlicePitch = 0;

		HRESULT result_create_buffer = m_device->GetDevice()->CreateBuffer(&buffer_desc, &data, &m_index_buffer);
		
		if (FAILED(result_create_buffer)) { return true; }

		return false;
	}

	// Установка вида примитива (по умолчанию список из треугольников)
	void IndexRes::SetPrimitive(typePrimitive primitive)
	{
		m_primitive_topology = (D3D11_PRIMITIVE_TOPOLOGY)primitive;
	}

	// Установить данный индексный буфер
	bool IndexRes::IncludeIndexBuffer(uint32 offset)
	{
		if (IResource::ready)
		{
			// Получим контекст устройства
			ID3D11DeviceContext3* device_context = m_device->GetDeviceContext();
			// Установим индексный буфер (обход треугольников)
			device_context->IASetIndexBuffer(m_index_buffer.Get(), DXGI_FORMAT_R16_UINT, offset);
			// Установим тип примитива
			device_context->IASetPrimitiveTopology(m_primitive_topology);

			return false;
		}
		else { return true; }
	}

	//
	void IndexRes::Draw()
	{
	}

	//
	void IndexRes::DrawIndex()
	{
		// Получим контекст устройства
		ID3D11DeviceContext3* device_context = m_device->GetDeviceContext();
		//
		device_context->DrawIndexed(m_triangles->size(), 0, 0);
	}

	//
	bool IndexRes::DrawInstanced()
	{
		if (IResource::ready)
		{
			// Получим контекст устройства
			ID3D11DeviceContext3* device_context = m_device->GetDeviceContext();
			//
			device_context->DrawIndexedInstanced(m_triangles->size(), 2, 0, 0, 0);

			return false;
		}
		else { return true; }
	}
}

