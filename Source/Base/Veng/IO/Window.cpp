#include <Veng/IO/Window.h>

#include <AX/EventSystem/EventManager.h>
#include <AX/Tool/Reporter.h>
#include <Veng/IO/Input.h>

#define GLAD_GL_IMPLEMENTATION
#include <glax/gl.h>
#include <GLFW/glfw3.h>

namespace Veng::IO
{
	class WindowHelper
	{
	public:
		glm::uvec2 Size;
		bool IsCursorVisible = true;

	public:
		static void glfw_error_callback(int error_code, const char* description);
		static void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void glfw_mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		static void glfw_cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
		static void glfw_scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
		static void glfw_framebuffer_size_callback(GLFWwindow* window, int width, int height);

		static void OpenGLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam);
	};

	static WindowHelper s_Helper;

	/***** CLASS IMPL *****/

	Window::Window(const std::string& title, size_t width, size_t height) :
		m_Title(title)
	{
		s_Helper.Size = glm::vec2(width, height);

		glfwSetErrorCallback(WindowHelper::glfw_error_callback);
		AX_Assert(glfwInit());

#ifdef AX_CONFIG_SHIPPING
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_FALSE);
#else
		glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE);
#endif // AX_CONFIG_SHIPPING

		glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
		m_Handle = glfwCreateWindow((int)width, (int)height, m_Title.c_str(), nullptr, nullptr);
		AX_Assert(m_Handle, "Failed to create GLFW window!");

		glfwMakeContextCurrent(m_Handle);
		{
			auto version = gladLoadGL(glfwGetProcAddress);
			AX_Assert(version != 0, "Failed to initialize OpenGL context");

			m_RendererVersion = std::format("OpenGL v{}", reinterpret_cast<const char*>(glGetString(GL_VERSION)));

			AX_LogInfo("Loaded {}", m_RendererVersion);
		}

#ifndef AX_CONFIG_SHIPPING
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(WindowHelper::OpenGLDebugMessageCallback, nullptr);
		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
#endif // !AX_CONFIG_SHIPPING

		glfwSetKeyCallback(m_Handle, WindowHelper::glfw_key_callback);
		glfwSetMouseButtonCallback(m_Handle, WindowHelper::glfw_mouse_button_callback);
		glfwSetCursorPosCallback(m_Handle, WindowHelper::glfw_cursor_pos_callback);
		glfwSetScrollCallback(m_Handle, WindowHelper::glfw_scroll_callback);
		glfwSetFramebufferSizeCallback(m_Handle, WindowHelper::glfw_framebuffer_size_callback);
		AX_LogInfo("Window Initialized");

#ifndef AX_PLATFORM_WEB
		{
			auto monitor = glfwGetPrimaryMonitor();
			auto mode = glfwGetVideoMode(monitor);

#ifdef AX_CONFIG_DEBUG
			// Center the window
			int monitorX, monitorY, monitorW, monitorH;
			glfwGetMonitorWorkarea(monitor, &monitorX, &monitorY, &monitorW, &monitorH);

			auto windowX = monitorX + (monitorW - width) / 2;
			auto windowY = monitorY + (monitorH - height) / 2;
			glfwSetWindowPos(m_Handle, (int)windowX, (int)windowY);
#else // !AX_CONFIG_DEBUG
			// Borderless fullscreen
			glfwSetWindowAttrib(m_Handle, GLFW_DECORATED, GLFW_FALSE);
			glfwSetWindowPos(m_Handle, 0, 0);
			glfwSetWindowSize(m_Handle, mode->width, mode->height);

			auto windowX = 0;
			auto windowY = 0;

			s_Helper.Size.x = mode->width;
			s_Helper.Size.y = mode->height;
#endif // AX_CONFIG_DEBUG

			AX_LogInfo(
				"Window mode: [{}, {}, {}, {} : {}Hz]",
				windowX, windowY, s_Helper.Size.x, s_Helper.Size.y, mode->refreshRate
			);
		}
#endif // !AX_PLATFORM_WEB

		WindowHelper::glfw_framebuffer_size_callback(m_Handle, (int)s_Helper.Size.x, (int)s_Helper.Size.y);
	}

	auto Window::IsValid() -> bool
	{
		return (m_Handle != nullptr);
	}

	void Window::Close()
	{
		glfwSetWindowShouldClose(m_Handle, GLFW_TRUE);
	}
	auto Window::ShouldClose() -> bool
	{
		return (glfwWindowShouldClose(m_Handle) == GLFW_TRUE);
	}

	void Window::SetVisible(bool isVisible)
	{
		if (isVisible)
		{
			glfwShowWindow(m_Handle);
		}
		else
		{
			glfwHideWindow(m_Handle);
		}
	}
	void Window::SetCursorVisible(bool isVisible)
	{
		auto mode = GLFW_CURSOR_NORMAL;
		if (isVisible == false)
		{
			mode = GLFW_CURSOR_DISABLED;
		}

		glfwSetInputMode(m_Handle, GLFW_CURSOR, mode);
		s_Helper.IsCursorVisible = isVisible;
	}
	void Window::SetVsync(bool isVsync)
	{
		glfwSwapInterval(isVsync);
	}
	void Window::SetAsCurrentContext()
	{
		glfwMakeContextCurrent(m_Handle);
	}

	void Window::ResetFrame()
	{
		m_DeltaTime = 0.0f;
		m_FrameStartTime = glfwGetTime();
	}
	void Window::PrepareFrame()
	{
		auto frameEndTime = glfwGetTime();
		m_DeltaTime = frameEndTime - m_FrameStartTime;
		m_FrameStartTime = frameEndTime;

		++m_TitleUpdateFrameCounter;
		m_TitleUpdateTimer += m_DeltaTime;
		if (m_TitleUpdateTimer > 1.0)
		{
			auto avgFPS = m_TitleUpdateFrameCounter / m_TitleUpdateTimer;
			glfwSetWindowTitle(m_Handle, std::format("{} - {:.0f} FPS", m_Title, avgFPS).c_str());

			m_TitleUpdateTimer = 0.0;
			m_TitleUpdateFrameCounter = 0;
		}

		glfwPollEvents();
	}
	void Window::RenderFrame()
	{
		glfwSwapBuffers(m_Handle);
	}

	auto Window::GetSize() const -> const glm::uvec2&
	{
		return s_Helper.Size;
	}
	auto Window::GetAspectRatio() const -> float
	{
		return static_cast<float>(s_Helper.Size.x) / static_cast<float>(s_Helper.Size.y);
	}

	Window::~Window() noexcept
	{
		if (m_Handle)
		{
			glfwDestroyWindow(m_Handle);
			m_Handle = nullptr;
		}
		glfwTerminate();

		AX_LogInfo("Window destroyed");
	}

	/***** HELPER IMPL *****/

	void WindowHelper::glfw_error_callback(int error_code, const char* description)
	{
		AX_LogError("GLFW Error ({}):\n\t{}", error_code, description);
	}

	void WindowHelper::glfw_key_callback(GLFWwindow* /*window*/, int key, int scancode, int action, int /*mods*/)
	{
		if (key < 0 || key >= static_cast<int>(Input::Key::COUNT))
		{
			AX_LogError("Unknown key:{}, scancode:{}, action:{}", key, scancode, action);
			return;
		}

		if (action == GLFW_REPEAT)
		{
			return;
		}

		auto event = Input::RegisterKeyStateEvent();
		event.Key = static_cast<Input::Key>(key);
		event.State = Input::State::Down;
		if (action == GLFW_RELEASE)
		{
			event.State = Input::State::Release;
		}

		AX::EventSystem::EventManager::SendEvent(event);
	}

	void WindowHelper::glfw_mouse_button_callback(GLFWwindow* /*window*/, int button, int action, int /*mods*/)
	{
		if (button < 0 || button >= static_cast<int>(Input::MouseButton::COUNT))
		{
			AX_LogError("Unknown mouse button:{}, action:{}", button, action);
			return;
		}

		auto event = Input::RegisterMouseButtonStateEvent();
		event.MouseButton = static_cast<Input::MouseButton>(button);
		event.State = Input::State::Down;
		if (action == GLFW_RELEASE)
		{
			event.State = Input::State::Release;
		}

		AX::EventSystem::EventManager::SendEvent(event);
	}

	void WindowHelper::glfw_cursor_pos_callback(GLFWwindow* /*window*/, double xpos, double ypos)
	{
		AX::EventSystem::EventManager::SendEvent(Input::RegisterMousePositionEvent(
			glm::vec2(xpos, ypos)
		));
	}

	void WindowHelper::glfw_scroll_callback(GLFWwindow* /*window*/, double xoffset, double yoffset)
	{
		AX::EventSystem::EventManager::SendEvent(Input::RegisterMouseScrollEvent(
			glm::vec2(xoffset, yoffset)
		));
	}

	void WindowHelper::glfw_framebuffer_size_callback(GLFWwindow* /*window*/, int width, int height)
	{
		s_Helper.Size.x = width;
		s_Helper.Size.y = height;

		auto event = Window::WindowResizeEvent();
		event.Width = width;
		event.Height = height;
		AX::EventSystem::EventManager::SendEvent(event);
	}

	void WindowHelper::OpenGLDebugMessageCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei /*length*/, const GLchar* message, const void* /*userParam*/)
	{
		if (id == 131185) // VBO usage notification
		{
			return;
		}

		auto errorMsg = std::string("OpenGL Debug Message:\n");

		errorMsg += "Source: ";
		switch(source)
		{
			case GL_DEBUG_SOURCE_API:				{ errorMsg += "API"; break; }
			case GL_DEBUG_SOURCE_WINDOW_SYSTEM:		{ errorMsg += "WINDOW_SYSTEM"; break; }
			case GL_DEBUG_SOURCE_SHADER_COMPILER:	{ errorMsg += "SHADER_COMPILER"; break; }
			case GL_DEBUG_SOURCE_THIRD_PARTY:		{ errorMsg += "THIRD_PARTY"; break; }
			case GL_DEBUG_SOURCE_APPLICATION:		{ errorMsg += "APPLICATION"; break; }
			case GL_DEBUG_SOURCE_OTHER:				{ errorMsg += "OTHER"; break; }
		}
		errorMsg += "\n";

		errorMsg += "Type: ";
		switch(type)
		{
			case GL_DEBUG_TYPE_ERROR:				{ errorMsg += "ERROR"; break; }
			case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:	{ errorMsg += "DEPRECATED_BEHAVIOR"; break; }
			case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:	{ errorMsg += "UNDEFINED_BEHAVIOR"; break; }
			case GL_DEBUG_TYPE_PORTABILITY:			{ errorMsg += "PORTABILITY"; break; }
			case GL_DEBUG_TYPE_PERFORMANCE:			{ errorMsg += "PERFORMANCE"; break; }
			case GL_DEBUG_TYPE_MARKER:				{ errorMsg += "MARKER"; break; }
			case GL_DEBUG_TYPE_PUSH_GROUP:			{ errorMsg += "PUSH_GROUP"; break; }
			case GL_DEBUG_TYPE_POP_GROUP:			{ errorMsg += "POP_GROUP"; break; }
			case GL_DEBUG_TYPE_OTHER:				{ errorMsg += "OTHER"; break; }
		}
		errorMsg += "\n";

		errorMsg += "Severity: ";
		switch(severity)
		{
			case GL_DEBUG_SEVERITY_LOW:				{ errorMsg += "LOW"; break; }
			case GL_DEBUG_SEVERITY_MEDIUM:			{ errorMsg += "MEDIUM"; break; }
			case GL_DEBUG_SEVERITY_HIGH:			{ errorMsg += "HIGH"; break; }
			case GL_DEBUG_SEVERITY_NOTIFICATION:	{ errorMsg += "NOTIFICATION"; break; }
		}
		errorMsg += "\n";

		errorMsg += "ID: " + std::to_string(id) + "\n";
		errorMsg += "Message: " + std::string(message) + "\n";

		AX_SilentAssert(false, errorMsg);
	}
}
