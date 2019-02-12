#include "prototype\engine\include\VerticesRes.h"

namespace SystemEngine
{
	// Конструктор
	VerticesRes::VerticesRes(string name)
	{
		IResource::name = name;

		IResource::type = TResource::Vertices;

		m_resource_manadger = ResourceManager::GetInstance();

		m_device = m_resource_manadger->GetDevice();

		m_strides_in_floated = 0;

		m_uvs = make_shared<array<shared_ptr<vector<Vector2>>, 4>>();
	}

	// Деструктор
	VerticesRes::~VerticesRes()
	{
		m_device.reset();
		
		m_resource_manadger.reset();
	}

	// Перезагрузить
	task<void> VerticesRes::Reload()
	{
		AddElementVertexBuffer(sizeof(Vector3) / sizeof(float), m_vertices->size(), shared_ptr<float>((float*)m_vertices->data()));

		if (m_normals != nullptr)
		{ AddElementVertexBuffer(sizeof(Vector3) / sizeof(float), m_normals->size(), shared_ptr<float>((float*)m_normals->data())); }
	
		if (m_uvs != nullptr)
		{ for (auto& it_uvs : *m_uvs) { if(it_uvs != nullptr) { AddElementVertexBuffer(sizeof(Vector2) / sizeof(float), it_uvs->size(), shared_ptr<float>((float*)it_uvs->data())); } } }

		if (m_colors != nullptr)
		{
			vector<float> float_colors;
			//
			for (auto& it_color : *m_colors)
			{
				Vector4 color = it_color.GetFloat();
				float_colors.push_back(color.x);
				float_colors.push_back(color.y);
				float_colors.push_back(color.z);
				float_colors.push_back(color.w);
			}
			//
			AddElementVertexBuffer(sizeof(float), float_colors.size(), shared_ptr<float>(float_colors.data()));
		}
		
		CreateVertexBuffer(m_vertices->size());

		IResource::ready = true;

		return task<void>();
	}

	// Освободить
	void VerticesRes::Release()
	{
		IResource::ready = false;
	}

	// Устанавливает массив вершин
	void VerticesRes::SetVertices(shared_ptr<vector<Vector3>> vertices)
	{
		m_vertices = vertices;
	}

	// Устанавливает массив нормалей
	void VerticesRes::SetNormals(shared_ptr<vector<Vector3>> normals)
	{
		m_normals = normals;
	}

	// Устанавливает массив UV координат в указанный индекс
	void VerticesRes::SetUVs(int channel, shared_ptr<vector<Vector2>> uvs)
	{
		// Проверим на превышение размерности массива
		if (channel > 3) { return; }
		// Установим массив в указанный индекс
		(*m_uvs)[channel] = uvs;
	}

	// Устанавливает массив 32 битных цветов
	void VerticesRes::SetColor(shared_ptr<vector<Color32>> colors)
	{
		m_colors = colors;
	}

	// Создание вершинного буфера
	bool VerticesRes::CreateVertexBuffer(int count_elements, bool dynamic, shared_ptr<vector<float>> source_buffer)
	{
		if (source_buffer == nullptr)
		{
			//
			vector<float> out_buffer;
			//
			m_strides_in_floated = 0;
			//
			for (auto& it_elements : m_elements) { m_strides_in_floated += it_elements->size_in_floated; }
			//
			out_buffer.reserve(count_elements * m_strides_in_floated);
			//
			for (int element = 0; element < count_elements; element++)
			{
				//
				for (auto& it_elements : m_elements)
				{
					//
					int count_replay = it_elements->size_in_floated;
					//
					for (int current = 0; current < count_replay; current++)
					//
					{ out_buffer.push_back(*(it_elements->source_ptr.get() + element * count_replay + current)); }
				}
			}
			//
			D3D11_BUFFER_DESC buffer_desc;
			buffer_desc.ByteWidth			= count_elements * m_strides_in_floated * sizeof(float);
			buffer_desc.BindFlags			= D3D11_BIND_VERTEX_BUFFER;
			buffer_desc.MiscFlags			= 0;
			buffer_desc.StructureByteStride	= 0;
			if (dynamic)
			{
				buffer_desc.Usage			= D3D11_USAGE_DYNAMIC;
				buffer_desc.CPUAccessFlags	= D3D11_CPU_ACCESS_WRITE;
			}
			else
			{
				buffer_desc.Usage			= D3D11_USAGE_DEFAULT;
				buffer_desc.CPUAccessFlags	= 0;
			}
			//
			D3D11_SUBRESOURCE_DATA data;
			data.pSysMem			= out_buffer.data();
			data.SysMemPitch		= 0;
			data.SysMemSlicePitch	= 0;

			HRESULT result_create_buffer = m_device->GetDevice()->CreateBuffer(&buffer_desc, &data, &m_vertex_buffer);

			if (FAILED(result_create_buffer)) { return true; }		
		}

		return false;
	}

	// Добавление одного элемента в список вершинного буфера
	void VerticesRes::AddElementVertexBuffer(int size_of_one_element_in_floated, int count_elements, shared_ptr<float> source_buffer)
	{
		shared_ptr<DataElement> new_element = make_shared<DataElement>();

		new_element->size_in_floated = size_of_one_element_in_floated;

		new_element->count_elements = count_elements;

		new_element->offset = 0;

		new_element->source_ptr = source_buffer;

		m_elements.push_back(new_element);
	}

	// Очистка массива элементов
	void VerticesRes::ClearElements()
	{
		m_elements.clear();
	}

	// Установить данный вершинный буфер
	bool VerticesRes::IncludeVerticesBuffer(uint32 start_slot, uint32 count_buffers, uint32 stride, uint32 offset)
	{
		if (IResource::ready)
		{
			if (stride == 0) { stride = m_strides_in_floated * sizeof(float); }

			m_device->GetDeviceContext()->IASetVertexBuffers(start_slot, count_buffers, m_vertex_buffer.GetAddressOf(), &stride, &offset);

			return false;
		}
		else { return true; }
	}
}