#include <Veng/Core/Application.h>

#include <AX/EventSystem/EventManager.h>
#include <AX/Tool/Reporter.h>
#include <Veng/Gfx/Renderer.h>

#if AX_PLATFORM_WEB
	#include <emscripten.h>
#endif

namespace Veng::Core
{
	class ApplicationHelper
	{
	public:
		void MainLoopStep(Application* self);

	public:
#if AX_PLATFORM_WEB
		static constexpr auto WINDOW_WIDTH = 960;
		static constexpr auto WINDOW_HEIGHT = 540;
#else
		static constexpr auto WINDOW_WIDTH = 1600;
		static constexpr auto WINDOW_HEIGHT = 900;
#endif
	};

	static ApplicationHelper s_Helper;

	/***** CLASS IMPL *****/

	Application::Application(const std::string& name) :
		m_Window(name, s_Helper.WINDOW_WIDTH, s_Helper.WINDOW_HEIGHT)
	{
		AX_Assert(m_Window.IsValid());
	}

	void Application::Run()
	{
		if (m_LayerManager == nullptr)
		{
			InitializeLayerSystem<LayerSystem::Context>();
		}

		Gfx::Renderer::Init();
		Gfx::Renderer::SetViewport({}, m_Window.GetSize());

		m_Window.SetVisible(true);
		m_Window.SetVsync(true);
		m_Window.ResetFrame();

#if AX_PLATFORM_WEB
		static auto application = this;
		emscripten_set_main_loop([]()
		{
			s_Helper.MainLoopStep(application);
		}, 0, true);
#endif

		while (m_Window.ShouldClose() == false)
		{
			s_Helper.MainLoopStep(this);
		}

		AX::EventSystem::EventManager::SendEvent(ShutdownEvent());
	}

	/***** HELPER IMPL *****/

	void ApplicationHelper::MainLoopStep(Application* self)
	{
		// TODO_ASC: TMP
		static auto fixedDeltaTimeAccumulator = 0.0f;

		self->m_Window.PrepareFrame();
		self->m_Input.Update();

		// Update
		self->m_LayerManager->OnUpdate(self->m_Window.GetDeltaTime());

		// TODO_ASC: TMP
		constexpr auto SIMULATION_STEPS_MAX = 5ull;
		fixedDeltaTimeAccumulator += self->m_Window.GetDeltaTime();
		auto simulationSteps = glm::min<size_t>(
			static_cast<size_t>(fixedDeltaTimeAccumulator / LayerSystem::Layer::FixedDeltaTime),
			SIMULATION_STEPS_MAX
		);
		while (simulationSteps > 0)
		{
			self->m_LayerManager->OnFixedUpdate();

			fixedDeltaTimeAccumulator -= LayerSystem::Layer::FixedDeltaTime;
			--simulationSteps;
		}

		// Render
		self->m_LayerManager->OnRender();

		self->m_Window.RenderFrame();
	}
}
