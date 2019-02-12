#pragma once

#include <memory>
#include <DirectXMath.h>

#include "Component.h"
#include "Type.h"
#include "Vector.h"

using namespace std;
using namespace AddStruct;
using namespace DirectX;

namespace ScriptEngine
{
//#pragma pack(push, 16)
//	__declspec(align(16)) struct matrix_transform
//	{
//		XMMATRIX	world_matrix;
//		XMMATRIX	translate_matrix;
//		XMMATRIX	rotate;
//	};

	class Transform : public Component
	{
	private:
		// Конструктор по умолчанию
											Transform();
		// Конструктор копирования
											Transform(const Transform&&) = delete;
		// Оператор присвоения
		Transform&							operator=(const Transform&&) = delete;

	public:
		// Создание объекта
		static shared_ptr<Transform>		Create(string name);
		// Деструктор
		virtual								~Transform() {};
		//
		void								SetTranslate(Vector3 translation);
		//
		void								SetRotateX(float rotate);
		//
		void								SetRotateY(float rotate);
		//
		void								SetRotateZ(float rotate);
		//
		Matrix4X4							GetWorldMatrix();

	private:
		void								UpdateMatrix();
		void								RecalculateTransform();

		XMFLOAT3*							m_translate_vector;
		XMFLOAT3*							m_rotate;
		Matrix4X4							m_transform;
	};
//#pragma pack(pop)
}