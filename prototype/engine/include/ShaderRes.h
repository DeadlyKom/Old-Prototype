#pragma once

#include <agile.h>
#include <d3d11_4.h>
#include <memory>
#include <ppltasks.h>
#include <string>
#include <vector>

#include "prototype\include\Type.h"

#include "Device.h"
#include "FileManager.h"
#include "ResourceManager.h"

using namespace std;
using namespace Concurrency;

namespace SystemEngine
{
	class ShaderRes : public IResource
	{
	public:
		enum class TShader
		{
			Vertex = 0, Geometry, Pixel
		};

	public:
									ShaderRes() = delete;
		// Конструктор
									ShaderRes(string name);
		// Деструктор
		virtual						~ShaderRes();
		// Перезагрузить
		virtual task<void>			Reload() override;
		// Освободить
		virtual void				Release() override;
		// Установка файла шейдера
		void						SetFile(TShader type_shader, int index_file_shader);

		// Компиляция шейдера
		bool						Compiled(TShader type_shader, const shared_ptr<vector<byte>> byte_code);
		// Установка формата вершин
		void						SetInputLayout(const shared_ptr<vector<D3D11_INPUT_ELEMENT_DESC>> vertex_layout = nullptr);
		// Добовление формата вершин
		void						AddInputLayout(string semantic_name, int semantic_index, DXGI_FORMAT format, UINT offset = D3D11_APPEND_ALIGNED_ELEMENT);
		// Установка шейдера
		bool						Include(TShader type_shader);
		// Установка привязки формата вершин
		bool						IncludeInputLayout();

	protected:
		// Компиляция вершинного шейдера
		void						CompiledVertex(const shared_ptr<vector<byte>> byte_code_vertex);
		// Компиляция геометрического шейдера
		void						CompiledGeometry(const shared_ptr<vector<byte>> byte_code_geometry);
		// Компиляция пиксельного шейдера
		void						CompiledPixel(const shared_ptr<vector<byte>> byte_code_pixel);

		// Системные переменные
		shared_ptr<Device>								m_device;				// ссылка на интерфейс устройства
		shared_ptr<FileManager>							m_file_manager;			// ссылка на диспетчер файлов
		shared_ptr<ResourceManager>						m_resource_manadger;	// ссылка на диспетчер ресурсов

		// Структура шейдера
		Microsoft::WRL::ComPtr<ID3D11VertexShader>      m_vertex_shader;		// интерфейс вершинного шейдера
		Microsoft::WRL::ComPtr<ID3D11GeometryShader>    m_geometry_shader;		// интерфейс геометрического шейдера
		Microsoft::WRL::ComPtr<ID3D11PixelShader>       m_pixel_shader;			// интерфейс пиксельного шейдера
		Microsoft::WRL::ComPtr<ID3D11InputLayout>       m_input_layout;			// интерфейс порядка и параметров вершин

		// Внутренние переменные
		int												m_index_file_vertex_shader;		// индекс файла вершинного шейдера
		int												m_index_file_geometry_shader;	// индекс файла геометрического шейдера
		int												m_index_file_pixel_shader;		// индекс файла пиксельного шейдера
		vector<D3D11_INPUT_ELEMENT_DESC>				m_list_input_layout;			// список формата
	};
}