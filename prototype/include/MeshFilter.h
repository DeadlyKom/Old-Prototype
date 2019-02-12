#pragma once

#include <memory>

#include "Component.h"
#include "Mesh.h"
#include "Type.h"

using namespace std;

namespace ScriptEngine
{
	class MeshFilter : public Component
	{
	private:
		// Конструктор по умолчанию
									MeshFilter();
		// Конструктор копирования
									MeshFilter(const MeshFilter&&) = delete;
		// Оператор присвоения
		MeshFilter&					operator=(const MeshFilter&&) = delete;

	public:
		// Создание объекта
		static shared_ptr<MeshFilter>	Create(string name);
		// Деструктор
		virtual							~MeshFilter();
		// Возращает ссылку на меш
		shared_ptr<Mesh>				GetMesh();
		// Устанавливает новую ссылку на меш
		void							SetMesh(shared_ptr<Mesh> mesh);

	protected:
		// Внутренние переменные
		shared_ptr<Mesh>				m_mesh;
	};
}