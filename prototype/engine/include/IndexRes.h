#pragma once

#include <d3d11_4.h>
#include <memory>
#include <string>
#include <vector>

#include "prototype\include\Type.h"
#include "ResourceManager.h"
#include "Device.h"

using namespace std;

namespace SystemEngine
{
	class IndexRes : public IResource
	{
	public:
		enum class typePrimitive : unsigned int
		{
			Point			= D3D11_PRIMITIVE_TOPOLOGY_POINTLIST,
			Line			= D3D11_PRIMITIVE_TOPOLOGY_LINELIST,
			LineStrip		= D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP,
			Triangle		= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST,
			TriangleStrip	= D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP
		};

	public:
									IndexRes() = delete;
		// Конструктор
									IndexRes(string name);
		// Деструктор
		virtual						~IndexRes();
		// Перезагрузить
		virtual task<void>			Reload() override;
		// Освободить
		virtual void				Release() override;
		// Устанавливает массив индексов (обхода треугольников) в указанный индекс
		void						SetTriangles(shared_ptr<vector<uint16>> triangles);
		// Создание индексного буфера
		bool						CreateIndexBuffer(shared_ptr<vector<uint16>> source_buffer_ptr, bool dynamic = false);
		// Установка вида примитива (по умолчанию список из треугольников)
		void						SetPrimitive(typePrimitive primitive = typePrimitive::Triangle);
		// Установить данный индексный буфер
		bool						IncludeIndexBuffer(uint32 offset = 0);
		//
		void						Draw();
		//
		void						DrawIndex();
		// 
		bool						DrawInstanced();

	protected:
		// Системные переменные
		shared_ptr<SystemEngine::ResourceManager>	m_resource_manadger;	// ссылка на диспетчер ресурсов
		shared_ptr<SystemEngine::Device>			m_device;				// ссылка на интерфейс устройства
		
		// Внутрение переменные
		D3D11_PRIMITIVE_TOPOLOGY					m_primitive_topology;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_index_buffer;			// интерфейс вершинного буфера

		shared_ptr<vector<uint16>>					m_triangles;			// массив индексов (обхода треугольников)
	};
}