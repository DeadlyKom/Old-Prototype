#include "..\InputHandler.h"

#include <functional>

using namespace std::placeholders;
using namespace Windows::Foundation;
using namespace Windows::UI::Input::Spatial;
using namespace std::placeholders;

namespace SystemEngine
{
	// Конструктор
	InputHandler::InputHandler()
	{
		m_source_state = nullptr;

		// The interaction manager provides an event that informs the app when
		// spatial interactions are detected.
		m_interaction_manager = SpatialInteractionManager::GetForCurrentView();

		// Bind a handler to the SourcePressed event.
		m_source_pressed_event_token = m_interaction_manager->SourcePressed +=
			ref new TypedEventHandler<SpatialInteractionManager^, SpatialInteractionSourceEventArgs^>(bind(&InputHandler::OnSourcePressed, this, _1, _2));
	}

	// Деструктор
	InputHandler::~InputHandler()
	{
		// Unregister our handler for the OnSourcePressed event.
		m_interaction_manager->SourcePressed -= m_source_pressed_event_token;
	}

	//
	Windows::UI::Input::Spatial::SpatialInteractionSourceState^ InputHandler::CheckForInput()
	{
		SpatialInteractionSourceState^ source_state = m_source_state;

		m_source_state = nullptr;

		return source_state;
	}

	// Обработчик
	void InputHandler::OnSourcePressed(Windows::UI::Input::Spatial::SpatialInteractionManager^ sender, Windows::UI::Input::Spatial::SpatialInteractionSourceEventArgs^ args)
	{
		m_source_state = args->State;
	}
}