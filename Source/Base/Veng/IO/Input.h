#ifndef _VENG_IO_INPUT
#define _VENG_IO_INPUT

#include <AX/EventSystem/Event.h>

#include <glm/glm.hpp>

#include <vector>
#include <unordered_map>
#include <functional>

namespace Veng::IO
{
	class Input
	{
	public:
		enum class State;
		enum class Key;
		enum class MouseButton;

		class RegisterKeyStateEvent;
		class RegisterMouseButtonStateEvent;
		class RegisterMousePositionEvent;
		class RegisterMouseScrollEvent;

		class KeyStateEvent;
		class MousePositionEvent;
		class MouseScrollEvent;

	public:
		Input();

		void RegisterKeyState(const Key& key, const State& state);
		void RegisterMouseButtonState(const MouseButton& mouseButton, const State& state);
		void RegisterMousePosition(const glm::vec2& position);
		void RegisterMouseScroll(const glm::vec2& scroll);

		void Update();

		inline auto GetKeyDown(const Key& key) -> bool;
		inline auto GetKeyHold(const Key& key) -> bool;
		inline auto GetKeyRelease(const Key& key) -> bool;

		inline auto GetMouseButtonDown(const MouseButton& mouseButton) -> bool;
		inline auto GetMouseButtonHold(const MouseButton& mouseButton) -> bool;
		inline auto GetMouseButtonRelease(const MouseButton& mouseButton) -> bool;

		inline auto GetMousePosition() -> const glm::vec2&;
		inline auto GetMouseScroll() -> const glm::vec2&;

	private:
		std::vector<State> m_KeyStates;
		std::vector<size_t> m_KeysToBeHold;
		std::vector<size_t> m_KeysToBeCleared;
		std::vector<Key> m_HoldingKeys;

		std::vector<State> m_MouseButtonStates;
		std::vector<size_t> m_MouseButtonsToBeHold;
		std::vector<size_t> m_MouseButtonsToBeCleared;

		glm::vec2 m_MousePosition = glm::vec2(0.0f);
		glm::vec2 m_MouseScroll = glm::vec2(0.0f);

		std::vector<std::function<void()>> m_InputRegisterOperations;
	};

	/***** IMPL *****/

	enum class Input::State
	{
		None,
		Down,
		Hold,
		Release
	};

	enum class Input::Key
	{
		/* The unknown key */
		Unknown			=	-1,

		/* Printable keys */
		Space			=	32,
		Apostrophe		=	39,	/* ' */
		Comma			=	44,	/* , */
		Minus			=	45,	/* - */
		Period			=	46,	/* . */
		Slash			=	47,	/* / */
		Alpha0			=	48,
		Alpha1			=	49,
		Alpha2			=	50,
		Alpha3			=	51,
		Alpha4			=	52,
		Alpha5			=	53,
		Alpha6			=	54,
		Alpha7			=	55,
		Alpha8			=	56,
		Alpha9			=	57,
		Semicolon		=	59,  /* ; */
		Equal			=	61,  /* = */
		A				=	65,
		B				=	66,
		C				=	67,
		D				=	68,
		E				=	69,
		F				=	70,
		G				=	71,
		H				=	72,
		I				=	73,
		J				=	74,
		K				=	75,
		L				=	76,
		M				=	77,
		N				=	78,
		O				=	79,
		P				=	80,
		Q				=	81,
		R				=	82,
		S				=	83,
		T				=	84,
		U				=	85,
		V				=	86,
		W				=	87,
		X				=	88,
		Y				=	89,
		Z				=	90,
		LeftBracket		=	91,  /* [ */
		Backslash		=	92,  /* \ */
		RightBracket	=	93,  /* ] */
		GraveAccent		=	96,  /* ` */
		World1			=	161, /* non-US #1 */
		World2			=	162, /* non-US #2 */

		/* Function keys */
		Escape			=	256,
		Enter			=	257,
		Tab				=	258,
		Backspace		=	259,
		Insert			=	260,
		Delete			=	261,
		Right			=	262,
		Left			=	263,
		Down			=	264,
		Up				=	265,
		PageUp			=	266,
		PageDown		=	267,
		Home			=	268,
		End				=	269,
		CapsLock		=	280,
		ScrollLock		=	281,
		NumLock			=	282,
		PrintScreen		=	283,
		Pause			=	284,
		F1				=	290,
		F2				=	291,
		F3				=	292,
		F4				=	293,
		F5				=	294,
		F6				=	295,
		F7				=	296,
		F8				=	297,
		F9				=	298,
		F10				=	299,
		F11				=	300,
		F12				=	301,
		F13				=	302,
		F14				=	303,
		F15				=	304,
		F16				=	305,
		F17				=	306,
		F18				=	307,
		F19				=	308,
		F20				=	309,
		F21				=	310,
		F22				=	311,
		F23				=	312,
		F24				=	313,
		F25				=	314,
		Keypad0			=	320,
		Keypad1			=	321,
		Keypad2			=	322,
		Keypad3			=	323,
		Keypad4			=	324,
		Keypad5			=	325,
		Keypad6			=	326,
		Keypad7			=	327,
		Keypad8			=	328,
		Keypad9			=	329,
		KeypadDecimal	=	330,
		KeypadDivide	=	331,
		KeypadMultiply	=	332,
		KeypadSubtract	=	333,
		KeypadAdd		=	334,
		KeypadEnter		=	335,
		KeypadEqual		=	336,
		LeftShift		=	340,
		LeftControl		=	341,
		LeftAlt			=	342,
		LeftSuper		=	343,
		RightShift		=	344,
		RightControl	=	345,
		RightAlt		=	346,
		RightSuper		=	347,
		Menu			=	348,

		COUNT
	};

	enum class Input::MouseButton
	{
		Button0,
		Button1,
		Button2,
		Button3,
		Button4,
		Button5,
		Button6,
		Button7,

		COUNT,

		Left = Button0,
		Right = Button1,
		Middle = Button2
	};

	class Input::RegisterKeyStateEvent : public AX::EventSystem::Event
	{
	public:
		Key Key;
		State State;
	};
	class Input::RegisterMouseButtonStateEvent : public AX::EventSystem::Event
	{
	public:
		MouseButton MouseButton;
		State State;
	};
	class Input::RegisterMousePositionEvent : public AX::EventSystem::Event
	{
	public:
		RegisterMousePositionEvent(const glm::vec2& position) : Position(position) {}

	public:
		glm::vec2 Position;
	};
	class Input::RegisterMouseScrollEvent : public AX::EventSystem::Event
	{
	public:
		RegisterMouseScrollEvent(const glm::vec2& scroll) : Scroll(scroll) {}

	public:
		glm::vec2 Scroll;
	};

	class Input::KeyStateEvent : public AX::EventSystem::Event
	{
	public:
		KeyStateEvent(Key key, State state) : Key(key), State(state) {}

		inline virtual auto GetCustomID() const -> AX::EventSystem::EventCustomID
		{
			return (
				(static_cast<AX::EventSystem::EventCustomID>(Key) << 16) |
				static_cast<AX::EventSystem::EventCustomID>(State)
			);
		}

	public:
		const Key Key;
		const State State;
	};
	class Input::MousePositionEvent : public AX::EventSystem::Event
	{
	public:
		MousePositionEvent(const glm::vec2& position) : Position(position) {}

	public:
		glm::vec2 Position;
	};
	class Input::MouseScrollEvent : public AX::EventSystem::Event
	{
	public:
		MouseScrollEvent(const glm::vec2& scroll) : Scroll(scroll) {}

	public:
		glm::vec2 Scroll;
	};

	inline auto Input::GetKeyDown(const Key& key) -> bool
	{
		auto keyAsIdx = static_cast<size_t>(key);
		return (m_KeyStates[keyAsIdx] == State::Down);
	}
	inline auto Input::GetKeyHold(const Key& key) -> bool
	{
		auto keyAsIdx = static_cast<size_t>(key);
		return (m_KeyStates[keyAsIdx] == State::Hold);
	}
	inline auto Input::GetKeyRelease(const Key& key) -> bool
	{
		auto keyAsIdx = static_cast<size_t>(key);
		return (m_KeyStates[keyAsIdx] == State::Release);
	}

	inline auto Input::GetMouseButtonDown(const MouseButton& mouseButton) -> bool
	{
		auto mouseButtonAsIdx = static_cast<size_t>(mouseButton);
		return (m_MouseButtonStates[mouseButtonAsIdx] == State::Down);
	}
	inline auto Input::GetMouseButtonHold(const MouseButton& mouseButton) -> bool
	{
		auto mouseButtonAsIdx = static_cast<size_t>(mouseButton);
		return (m_MouseButtonStates[mouseButtonAsIdx] == State::Hold);
	}
	inline auto Input::GetMouseButtonRelease(const MouseButton& mouseButton) -> bool
	{
		auto mouseButtonAsIdx = static_cast<size_t>(mouseButton);
		return (m_MouseButtonStates[mouseButtonAsIdx] == State::Release);
	}

	inline auto Input::GetMousePosition() -> const glm::vec2&
	{
		return m_MousePosition;
	}
	inline auto Input::GetMouseScroll() -> const glm::vec2&
	{
		return m_MouseScroll;
	}
}

#endif // !_VENG_IO_INPUT
