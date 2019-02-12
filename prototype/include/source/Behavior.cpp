#include "prototype\include\Behavior.h"

#include "prototype\engine\include\ObjectRegistry.h"

namespace ScriptEngine
{
	// Конструктор
	Behavior::Behavior(string name)
	{
		IObject::name = NAME_BEHAVIOR + name;

		IObject::type = TObject::Behavior;

		m_call_current_functions = 0;

		m_awake = m_start = m_update = nullptr;
	}

	// Создание объекта
	shared_ptr<Behavior> Behavior::Create(string name)
	{
		shared_ptr<Behavior> new_behavior(new Behavior(name));

		new_behavior->instance_id = SystemEngine::ObjectRegistry::GetInstance()->RegistrationObject(new_behavior);

		return new_behavior;
	}

	// Деструктор
	Behavior::~Behavior()
	{
	}

	// Запуск текущей функции
	void Behavior::CallCurrentFunction()
	{
		switch (m_call_current_functions)
		{
		case 0:	if (m_awake != nullptr)		{ m_awake();  }	m_call_current_functions++; break;
		case 1:	if (m_start != nullptr)		{ m_start();  }	m_call_current_functions++; break;
		case 2:	if (m_update != nullptr)	{ m_update(); }								break;
		}
	}

	// Установка функции первого запуска
	void Behavior::SetAwake(function<void(void)> awake)
	{
		m_awake = awake;
	}

	// Установка функции запуска
	void Behavior::SetStart(function<void(void)> start)
	{
		m_start = start;
	}

	// Установка функции обновления
	void Behavior::SetUpdate(function<void(void)> update)
	{
		m_update = update;
	}
}