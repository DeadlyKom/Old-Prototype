#include "..\CameraEx.h"

namespace SystemEngine
{
	// Конструктор
	CameraEx::CameraEx()
	{
	}

	// Деструктор
	CameraEx::~CameraEx()
	{
	}

	// Перезагрузить
	task<void> CameraEx::Reload()
	{
		IResource::ready = true;

		return task<void>();
	}

	// Освободить
	void CameraEx::Release()
	{
		IResource::ready = false;
	}

	// Установка голографической камеры
	void CameraEx::SetHolographicCamera(HolographicCamera^ holographic_camera)
	{
	}
}