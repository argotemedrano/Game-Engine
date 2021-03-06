#include "pch.h"
#include "Application.h"

#include "Events/ApplicationEvent.h"
#include "Log.h"
#include <functional>
#include "Anacardia/ImGui/ImGuiLayer.h"

namespace Anacardia
{
	Application* Application::s_Instance = nullptr;
	Application::Application()
	{
		//Anacardia::Log::Init();
		m_Window = std::unique_ptr<Window>(Window::Create());
		m_Window->SetEventCallback(std::bind(&Application::OnEvent, this, std::placeholders::_1));
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		ANACARDIA_CORE_INFO("Started Anacardia Core Logger: ", Log::GetCoreLogger()->name());
		ANACARDIA_INFO("Started Anacardia Client Logger: ", Log::GetClientLogger()->name());
		while (m_Running)
		{
			m_Window->OnUpdate();
			ImGuiLayer::Begin();
			/*
			* No onupdate since all events that modify the objects
			* work immediately
			*/
			for (Layer* layers : m_LayerStack)
			{
				layers->OnImGuiRender();
			}

			ImGuiLayer::End();
		}
	}

	void Application::OnEvent(Event& event)
	{
		EventDispatcher dispatcher(event);

		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Application::OnWindowClose, this, std::placeholders::_1));
		ANACARDIA_CORE_TRACE("Dispatching event {0} in Application", event.ToString());
	}

	bool Application::OnWindowClose(WindowCloseEvent& event)
	{
		m_Running = false;
		return true;
	}

}
