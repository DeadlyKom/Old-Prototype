#pragma once

#include <memory>

using namespace std;
using namespace Windows::UI::Input::Spatial;

namespace SystemEngine
{
	class InputHandler
	{
	public:
		// Конструктор
										InputHandler();
		// Деструктор
		virtual							~InputHandler();
		//
		Windows::UI::Input::Spatial::SpatialInteractionSourceState^	CheckForInput();

	protected:
		// Обработчик
		void							OnSourcePressed(Windows::UI::Input::Spatial::SpatialInteractionManager^ sender, Windows::UI::Input::Spatial::SpatialInteractionSourceEventArgs^ args);

		// Внутренние переменные
		Windows::UI::Input::Spatial::SpatialInteractionManager^			m_interaction_manager;			// диспетчер обработки ввода жестов и генерации событий жестов
		Windows::Foundation::EventRegistrationToken	m_source_pressed_event_token;	// флаг регистрации события

		// Used to indicate that a Pressed input event was received this frame.
		Windows::UI::Input::Spatial::SpatialInteractionSourceState^		m_source_state;

	};
}