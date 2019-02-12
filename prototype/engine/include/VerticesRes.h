#pragma once

#include <agile.h>
#include <array>
#include <d3d11_4.h>
#include <memory>
#include <string>
#include <vector>

#include "prototype\include\Color.h"
#include "prototype\include\Type.h"
#include "prototype\include\Vector.h"

#include "Device.h"
#include "ResourceManager.h"

using namespace std;
using namespace AddStruct;

namespace SystemEngine
{
	class VerticesRes : public IResource
	{
	private:
		//
		struct DataElement
		{
			int					size_in_floated;
			int					count_elements;
			int					offset;
			shared_ptr<float>	source_ptr;
		};

	public:
									VerticesRes() = delete;
		// Конструктор
									VerticesRes(string name);
		// Деструктор
		virtual						~VerticesRes();
		// Перезагрузить
		virtual task<void>			Reload() override;
		// Освободить
		virtual void				Release() override;

		// Устанавливает массив вершин
		void						SetVertices(shared_ptr<vector<Vector3>> vertices);
		// Устанавливает массив нормалей
		void						SetNormals(shared_ptr<vector<Vector3>> normals);
		// Устанавливает массив UV координат в указанный индекс
		void						SetUVs(int channel, shared_ptr<vector<Vector2>> uvs);
		// Устанавливает массив 32 битных цветов
		void						SetColor(shared_ptr<vector<Color32>> colors);

		// Создание вершинного буфера
		bool						CreateVertexBuffer(int count_elements, bool dynamic = false, shared_ptr<vector<float>> source_buffer = nullptr);
		// Добавление одного элемента в список вершинного буфера
		void						AddElementVertexBuffer(int size_of_one_element_in_floated, int count_elements, shared_ptr<float> source_buffer);
		// Очистка массива элементов
		void						ClearElements();
		// Установить данный вершинный буфер
		bool						IncludeVerticesBuffer(uint32 start_slot = 0, uint32 count_buffers = 1, uint32 stride = 0, uint32 offset = 0);

	protected:
		// Системные переменные
		shared_ptr<SystemEngine::ResourceManager>	m_resource_manadger;	// ссылка на диспетчер ресурсов
		shared_ptr<SystemEngine::Device>			m_device;				// ссылка на интерфейс устройства

		// Внутрение переменные
		vector<shared_ptr<DataElement>>				m_elements;				// массив элементов данных
		Microsoft::WRL::ComPtr<ID3D11Buffer>		m_vertex_buffer;		// интерфейс вершинного буфера

		uint32										m_strides_in_floated;	// размерность одного элемента массива во флоатах

		shared_ptr<vector<Vector3>>							m_vertices;				// массив вершин
		shared_ptr<vector<Vector3>>							m_normals;				// массив нормалей
		shared_ptr<array<shared_ptr<vector<Vector2>>, 4>>	m_uvs;					// массив UV координат
		shared_ptr<vector<Color32>>							m_colors;				// массив 32 битных цветов

	};
}