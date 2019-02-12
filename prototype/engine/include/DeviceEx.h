#pragma once

using namespace Windows::Graphics::Holographic;

namespace SystemEngine
{
	class DeviceEx
	{
	public:
		// Конструктор
											DeviceEx();
		// Деструктор
		virtual								~DeviceEx();
		// Установка голографического пространства
		void								SetHolographicSpace(HolographicSpace^ holographic_space);

	protected:

	};
}