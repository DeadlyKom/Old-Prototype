#include "prototype\include\Transform.h"
#include "prototype\include\Material.h"
#include "prototype\include\MeshRender.h"

#include "prototype\engine\include\ObjectRegistry.h"

namespace ScriptEngine
{
	// Конструктор по умолчанию
	Transform::Transform()
	{
		m_translate_vector = new XMFLOAT3(0,0,0);
		m_rotate		= new XMFLOAT3(0,0,0);
	}

	// Создание объекта
	shared_ptr<Transform> Transform::Create(string name)
	{
		shared_ptr<Transform> new_transform(new Transform);

		new_transform->name = NAME_TRANSFORM + name;

		new_transform->type = TObject::Transform;

		new_transform->instance_id = SystemEngine::ObjectRegistry::GetInstance()->RegistrationObject(new_transform);

		return new_transform;
	}

	//
	void Transform::SetTranslate(Vector3 translation)
	{
		m_translate_vector->x = translation.x;
		m_translate_vector->y = translation.y;
		m_translate_vector->z = translation.z;

		RecalculateTransform();
	}
	
	//
	void Transform::SetRotateX(float rotate)
	{
		m_rotate->x = rotate;

		RecalculateTransform();
	}
	//
	void Transform::SetRotateY(float rotate)
	{
		m_rotate->y = rotate;

		RecalculateTransform();
	}
	//
	void Transform::SetRotateZ(float rotate)
	{
		m_rotate->z = rotate;

		RecalculateTransform();
	}

	//
	Matrix4X4 Transform::GetWorldMatrix()
	{
		////const float    radians_x = static_cast<float>(fmod(XMConvertToRadians(m_rotate->x), XM_2PI));
		////const XMMATRIX model_rotation_x = XMMatrixRotationX(radians_x);
		//const float    radians_y = static_cast<float>(fmod(XMConvertToRadians(m_rotate->y), XM_2PI));
		//const XMMATRIX model_rotation_y = XMMatrixRotationY(-radians_y);
		////const float    radians_z = static_cast<float>(fmod(XMConvertToRadians(m_rotate->z), XM_2PI));
		////const XMMATRIX model_rotation_z = XMMatrixRotationZ(radians_z);

		////const XMMATRIX rotate = XMMatrixMultiply(XMMatrixMultiply(model_rotation_x, model_rotation_y), model_rotation_z);

		//const XMMATRIX translate = XMMatrixTranslationFromVector(XMLoadFloat3(m_translate_vector));

		//const XMMATRIX transform = XMMatrixMultiply(model_rotation_y, translate);
		//
		//Matrix4X4 model;

		//XMStoreFloat4x4((XMFLOAT4X4*)&model, XMMatrixTranspose(transform));
		
		return m_transform;
	}

	void Transform::UpdateMatrix()
	{
		// Получим список материалов для сопастовления их с обходом треугольников
		shared_ptr<vector<shared_ptr<Material>>> materials = this->GetComponent<MeshRender>()->GetMaterials();
		//
		for (auto& it_material : *materials)
		{
			it_material->SetMatrix(PB_MATRIX_TRANSFORM, m_transform);
		}
	}

	void Transform::RecalculateTransform()
	{
		const float    radians_x = static_cast<float>(fmod(XMConvertToRadians(m_rotate->x), XM_2PI));
		const XMMATRIX model_rotation_x = XMMatrixRotationX(radians_x);
		const float    radians_y = static_cast<float>(fmod(XMConvertToRadians(m_rotate->y), XM_2PI));
		const XMMATRIX model_rotation_y = XMMatrixRotationY(-radians_y);
		const float    radians_z = static_cast<float>(fmod(XMConvertToRadians(m_rotate->z), XM_2PI));
		const XMMATRIX model_rotation_z = XMMatrixRotationZ(radians_z);
		
		const XMMATRIX translate = XMMatrixTranslationFromVector(XMLoadFloat3(m_translate_vector));

		const XMMATRIX model_rotation = XMMatrixMultiply(XMMatrixMultiply(model_rotation_x, model_rotation_y), model_rotation_z);

		const XMMATRIX transform = XMMatrixMultiply(model_rotation, translate);

		XMStoreFloat4x4((XMFLOAT4X4*)&m_transform, XMMatrixTranspose(transform));

		UpdateMatrix();
	}
}