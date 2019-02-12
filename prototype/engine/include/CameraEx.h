#pragma once

#include "ResourceManager.h"

using namespace Windows::Graphics::Holographic;

namespace SystemEngine
{
	class CameraEx : public IResource
	{
	public:
		// Конструктор
									CameraEx();
		// Деструктор
		virtual						~CameraEx();
		
		// Перезагрузить
		virtual task<void>			Reload() override;
		// Освободить
		virtual void				Release() override;
		// Установка голографической камеры
		void						SetHolographicCamera(HolographicCamera^ holographic_camera);

	};
}