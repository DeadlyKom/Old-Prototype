#pragma once

#include <functional>

#include "Component.h"
#include "Type.h"

using namespace std;

namespace ScriptEngine
{
	class Behavior : public Component
	{
	private:
									Behavior() = delete;
		// Конструктор по умолчанию
									Behavior(string name);
		// Конструктор копирования
									Behavior(const Behavior&&) = delete;
		// Оператор присвоения
		Behavior&					operator=(const Behavior&&) = delete;

	public:
		// Создание объекта
		static shared_ptr<Behavior>	Create(string name);
		// Деструктор
		virtual						~Behavior();
		// Запуск текущей функции
		void						CallCurrentFunction();
		// Установка функции первого запуска
		void						SetAwake(function<void(void)> awake);
		// Установка функции запуска
		void						SetStart(function<void(void)> start);
		// Установка функции обновления
		void						SetUpdate(function<void(void)> update);

	protected:
		int							m_call_current_functions;

		function<void(void)>		m_awake;
		function<void(void)>		m_start;
		function<void(void)>		m_update;
	};
}