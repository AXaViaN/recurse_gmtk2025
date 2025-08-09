#ifndef _VENG_CORE_APPLICATION
#define _VENG_CORE_APPLICATION

#include <AX/EventSystem/Event.h>
#include <Veng/IO/Input.h>
#include <Veng/IO/Window.h>
#include <Veng/LayerSystem/LayerManager.h>

#include <string>
#include <memory>

namespace Veng::Core
{
	class Application
	{
		friend class ApplicationHelper;
	public:
		class ShutdownEvent;

	public:
		Application(const std::string& name);

		template<typename TLayerSystemContext>
		inline void InitializeLayerSystem();
		template<typename TLayer>
		inline void AddLayer();

		void Run();

	private:
		IO::Input m_Input;
		IO::Window m_Window;

		std::unique_ptr<LayerSystem::LayerManager> m_LayerManager;
		std::unique_ptr<LayerSystem::Context> m_LayerSystemContext;
	};

	/***** IMPL *****/

	class Application::ShutdownEvent : public AX::EventSystem::Event
	{
	};

	template<typename TLayerSystemContext>
	inline void Application::InitializeLayerSystem()
	{
		m_LayerSystemContext = std::make_unique<TLayerSystemContext>(m_Window, m_Input);
		m_LayerManager = std::make_unique<LayerSystem::LayerManager>(m_LayerSystemContext.get());
	}
	template<typename TLayer>
	inline void Application::AddLayer()
	{
		if (m_LayerManager == nullptr)
		{
			InitializeLayerSystem<LayerSystem::Context>();
		}

		m_LayerManager->AddLayer<TLayer>(LayerSystem::LayerManager::Placement::Top);
	}
}

#endif // !_VENG_CORE_APPLICATION
