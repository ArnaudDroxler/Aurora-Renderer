#pragma once

#include "Aurora/Events/Event.h"

namespace Aurora
{

	class Layer
	{
	public:
		Layer(const std::string& debugName);
		virtual ~Layer() = default;


		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate() {};
		virtual void OnEvent(Event& event) {};
		virtual void OnImGUIRender() {};

		inline const std::string& GetName() const { return debugName; }

	private:
		std::string debugName = "";
	};


}
