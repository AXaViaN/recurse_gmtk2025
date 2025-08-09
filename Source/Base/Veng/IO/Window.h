#ifndef _VENG_IO_WINDOW
#define _VENG_IO_WINDOW

#include <AX/EventSystem/Event.h>
#include <AX/Tool/Macro.h>

#include <glm/glm.hpp>

#include <string>

struct GLFWwindow;

namespace Veng::IO
{
	class Window
	{
	public:
		class WindowResizeEvent;
		
	public:
		Window(const std::string& title, size_t width, size_t height);

		auto IsValid() -> bool;

		void Close();
		auto ShouldClose() -> bool;

		void SetVisible(bool isVisible);
		void SetCursorVisible(bool isVisible);
		void SetVsync(bool isVsync);
		void SetAsCurrentContext();

		void ResetFrame();
		void PrepareFrame();
		void RenderFrame();

		inline auto GetTime() -> float;
		inline auto GetDeltaTime() -> float;

		auto GetSize() const -> const glm::uvec2&;
		auto GetAspectRatio() const -> float;

		inline auto GetHandle() -> GLFWwindow*;
		inline auto GetRendererVersion() -> const std::string&;

	public:
		~Window() noexcept;
		AX_SemanticsDeleteAll(Window)

	private:
		GLFWwindow* m_Handle = nullptr;
		std::string m_Title;
		std::string m_RendererVersion;

		double m_FrameStartTime = 0.0f;
		double m_DeltaTime = 0.0f;

		double m_TitleUpdateTimer = 0.0f;
		unsigned int m_TitleUpdateFrameCounter = 0;
	};

	/***** IMPL *****/

	class Window::WindowResizeEvent : public AX::EventSystem::Event
	{
	public:
		size_t Width;
		size_t Height;
	};

	inline auto Window::GetTime() -> float
	{
		return static_cast<float>(m_FrameStartTime);
	}
	inline auto Window::GetDeltaTime() -> float
	{
		return static_cast<float>(m_DeltaTime);
	}

	inline auto Window::GetHandle() -> GLFWwindow*
	{
		return m_Handle;
	}
	inline auto Window::GetRendererVersion() -> const std::string&
	{
		return m_RendererVersion;
	}
}

#endif // !_VENG_IO_WINDOW
