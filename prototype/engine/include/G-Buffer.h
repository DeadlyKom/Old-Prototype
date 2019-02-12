#pragma once

#include <array>
#include <memory>

#include "prototype\include\Type.h"

#include "Device.h"
#include "ResourceManager.h"
#include "TextureRes.h"

using namespace std;
//using namespace AddStruct;

namespace SystemEngine
{
	class GBuffer
	{
	private:
		enum Buffer : unsigned int
		{
			Diffuse				= 0,
			Normal,
			Specular,
			Depth
		};

	public:
		// Конструктор
									GBuffer();
		// Деструктор
		virtual						~GBuffer();
		// Создание буфера геометрии
		bool						Create(int width, int height);
		// Очистка буферов цвета и глубины
		void						ClearDiffuseDepth(ColorFloat diffuse = AddStruct::Colors::Transparent);
		// Связывание буферов с областью отображения
		void						IncludeBuffersRenderTargets();
		// Связывание текстур с пиксельным шейдером
		void						IncludePixelShader();
		// Убрать связывание с пиксельным шейдером
		void						UnincludedPixelShader();
		
	protected:
		// Системные переменные
		shared_ptr<SystemEngine::ResourceManager>	m_resource_manadger;	// ссылка на диспетчер ресурсов
		shared_ptr<SystemEngine::Device>			m_device;				// ссылка на интерфейс устройства

		// Внутрение переменные
		array<shared_ptr<TextureRes>, 4>			m_textures;
	};
}