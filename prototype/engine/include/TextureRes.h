#pragma once

#include <d3d11_4.h>
#include <memory>
#include <string>
#include <vector>

#include "prototype\include\Color.h"
#include "prototype\include\Type.h"

#include "Device.h"
#include "FileManager.h"
#include "ResourceManager.h"

using namespace std;

// Тип данных
enum class ConverterTypeData
{
	CTD_FILE_UNKNOW		= 0x00000000,			// неизвестный формат изображения
	CTD_FILE_TGA		= 0x00000001,			// формат изображения TGA
	CTD_FILE_JPEG		= 0x00000002,			// формат изображения JPEG
	CTD_FILE_PNG		= 0x00000003			// формат изображения PNG
};

#define HGD_DLLIMPORT_EX	extern "C" __declspec (dllimport)
#define HGD_DLLEXPORT_EX	extern "C" __declspec (dllexport)

// Создание объекта конвертора
HGD_DLLEXPORT_EX size_t Converter_Create();
// Освобождение конвертора
HGD_DLLEXPORT_EX void	Converter_Release(size_t id_converter);
// Установка настроек конвертора
HGD_DLLEXPORT_EX bool	Setup(size_t id_converter, uint32 flags);
// Установка ранее загруженного изображения в память
HGD_DLLEXPORT_EX bool	SetDataFromFileInMemory(size_t id_converter, const byte* source_buffer, ConverterTypeData ctd, uint32 length);
// Установка буфера RGBA
HGD_DLLEXPORT_EX bool	SetDataRGBA(size_t id_converter, const Color32* source_buffer, uint32 width, uint32 height);
// Конвертация в DXT
HGD_DLLEXPORT_EX bool	CompressToDXT(size_t id_converter, byte* destination_buffer);
// Возвращает тип ошибки
HGD_DLLEXPORT_EX uint32	GetError(size_t id_converter);
// Возвращает ширину изображения
HGD_DLLEXPORT_EX uint32	GetWidth(size_t id_converter);
// Возвращает высоту изображения
HGD_DLLEXPORT_EX uint32	GetHeight(size_t id_converter);
// Возвращает массив RGBA
HGD_DLLEXPORT_EX const Color32*	GetRGBA(size_t id_converter);
// Возвращает размер массива необходимый для конвертации RGBA в DXT
HGD_DLLEXPORT_EX uint32	GetOutputBufferSize(size_t id_converter);

enum CompressorSetUp
{
	CSU_NULL			= 0x00000000,
	CSU_NOT_DDS_HADER	= 0x00000001,			// не формировать заголовок DDS формата
	CSU_NEGATIVE_X		= 0x00000002,			// отражение по оси Х
	CSU_NEGATIVE_Y		= 0x00000004,			// отражение по оси Y
	CSU_JPEG_LIMIT		= 0x00000008
};

namespace SystemEngine
{
	class TextureRes : public IResource
	{
	public:
		enum class TTexture
		{
			T1D = 0, T1DA, T2D, T2DA, T3D
		};

		enum class TUsage
		{
			Default					= D3D11_USAGE_DEFAULT,
			Immutable				= D3D11_USAGE_IMMUTABLE,
			Dynamic					= D3D11_USAGE_DYNAMIC,
			Staging					= D3D11_USAGE_STAGING
		};

		enum class TBind
		{
			Default					= D3D11_BIND_SHADER_RESOURCE,
			RenderTarget			= D3D11_BIND_RENDER_TARGET,
			DepthBuffer				= D3D11_BIND_DEPTH_STENCIL,
			UnorderedView			= D3D11_BIND_UNORDERED_ACCESS
		};

		enum class TFormat
		{
			R11G11B10				= DXGI_FORMAT_R11G11B10_FLOAT,
			RGBA					= DXGI_FORMAT_R8G8B8A8_UNORM,
			D16						= DXGI_FORMAT_D16_UNORM,
			BC1						= DXGI_FORMAT_BC1_UNORM,
			BC5						= DXGI_FORMAT_BC5_UNORM
		};

	public:
									TextureRes() = delete;
		// Конструктор
									TextureRes(string name);
		// Деструктор
		virtual						~TextureRes();
		// Перезагрузить
		virtual task<void>			Reload() override;
		// Освободить
		virtual void				Release() override;

		// Создание текстуры
		bool						Create2D(int width, int height, TBind bind, TFormat format, bool stereo = false, int mip_map_levels = 1, bool dynamic = false);
		// Создание облости отрисовки
		bool						CreateRenderTarget();
		// Создание буфера глубины
		bool						CreateDepthBuffer();
		// Создание доступности данной текстуры в шейдере
		bool						CreateShaderResourceThis();

		// Установка файла текстуры
		void						SetFile(TTexture type_texture, int index_file_shader, TFormat format = TFormat::BC1);
		// Создание 1D текструры (без сжатия)
		bool						Create1D(int width, vector<Color32>* rgba = nullptr);
		// Создание 1D текстуры
		bool						Create1DFromData(shared_ptr<vector<byte>> image_data, TFormat format);
		// Создание 2D текструры (без сжатия)
		bool						Create2D(int width, int height, vector<Color32>* rgba = nullptr);
		// Создание 2D текстуры
		bool						Create2DFromData(shared_ptr<vector<byte>> image_data, TFormat format);

		// Установить данный текстурный ресурс
		bool						IncludeShaderResource(uint32 start_slot = 0, uint32 count_buffers = 1);

		// Возвращает ссылка на интерфейс области отрисовки
		ID3D11RenderTargetView*		GetRenderTarget(bool recreate = false);
		// Возвращает ссылку на интерфейс буфера глубины и трафарета
		ID3D11DepthStencilView*		GetDepthBuffer(bool recreate = false);
		// Возвращает ссылку на интерфейс ресурса шейдера
		ID3D11ShaderResourceView*	GetShaderResource(bool recreate = false);

	protected:
		// Системные переменные
		shared_ptr<Device>					m_device;				// ссылка на интерфейс устройства
		shared_ptr<FileManager>				m_file_manager;			// ссылка на диспетчер файлов
		shared_ptr<ResourceManager>			m_resource_manadger;	// ссылка на диспетчер ресурсов

		// Внутренние переменные
		int									m_width_texture;		// ширина текстуры
		int									m_height_texture;		// высота текстуры
		int									m_mip_map_levels;		// номер MipMap'а
		int									m_index_file_texture;	// индекс файла текстуры
		bool								m_stereo;				// флаг создания массива текстур (для стерео)
		TUsage								m_type_usage;			// тип использования
		TBind								m_type_bind;			// способ включения в состав
		TTexture							m_type_texture;			// тип текстуры
		TFormat								m_type_format;			// тип формата

		ComPtr<ID3D11Texture2D>				m_texture_2D;			// ссылка на интерфейс 2D текстуры
		ComPtr<ID3D11ShaderResourceView>	m_texture_srv;			// ссылка на интерфейс ресурса текстуры
		ComPtr<ID3D11RenderTargetView>		m_texture_tv;			// ссылка на интерфейс облости отрисовки
		ComPtr<ID3D11DepthStencilView>		m_texture_dsv;			// ссылка на интерфейс буфера глубины ит рафарета
		ComPtr<ID3D11SamplerState>			m_sampler_state;		// ссылка на самплер(способ наложения тексутры)
	};
}