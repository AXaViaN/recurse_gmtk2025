#include <Veng/IO/Input.h>

#include <AX/EventSystem/EventManager.h>

namespace Veng::IO
{
	Input::Input() :
		m_KeyStates(static_cast<int>(Key::COUNT), State::None),
		m_MouseButtonStates(static_cast<int>(MouseButton::COUNT), State::None)
	{
		AX::EventSystem::EventManager::StartListener<RegisterKeyStateEvent>([&](const RegisterKeyStateEvent& event)
		{
			m_InputRegisterOperations.emplace_back([this, event]()
			{
				RegisterKeyState(event.Key, event.State);
			});
		});
		AX::EventSystem::EventManager::StartListener<RegisterMouseButtonStateEvent>([&](const RegisterMouseButtonStateEvent& event)
		{
			m_InputRegisterOperations.emplace_back([this, event]()
			{
				RegisterMouseButtonState(event.MouseButton, event.State);
			});
		});
		AX::EventSystem::EventManager::StartListener<RegisterMousePositionEvent>([&](const RegisterMousePositionEvent& event)
		{
			m_InputRegisterOperations.emplace_back([this, event]()
			{
				RegisterMousePosition(event.Position);
			});
		});
		AX::EventSystem::EventManager::StartListener<RegisterMouseScrollEvent>([&](const RegisterMouseScrollEvent& event)
		{
			m_InputRegisterOperations.emplace_back([this, event]()
			{
				RegisterMouseScroll(event.Scroll);
			});
		});
	}

	void Input::RegisterKeyState(const Key& key, const State& state)
	{
		auto keyAsIdx = static_cast<size_t>(key);
		m_KeyStates[keyAsIdx] = state;

		AX::EventSystem::EventManager::SendEvent(KeyStateEvent(key, state));

		if (state == State::Down)
		{
			m_KeysToBeHold.emplace_back(keyAsIdx);
		}
		else if (state == State::Release)
		{
			m_KeysToBeCleared.emplace_back(keyAsIdx);
		}
	}
	void Input::RegisterMouseButtonState(const MouseButton& mouseButton, const State& state)
	{
		auto mouseButtonAsIdx = static_cast<size_t>(mouseButton);
		m_MouseButtonStates[mouseButtonAsIdx] = state;

		if (state == State::Down)
		{
			m_MouseButtonsToBeHold.emplace_back(mouseButtonAsIdx);
		}
		else if (state == State::Release)
		{
			m_MouseButtonsToBeCleared.emplace_back(mouseButtonAsIdx);
		}
	}
	void Input::RegisterMousePosition(const glm::vec2& position)
	{
		m_MousePosition = position;

		AX::EventSystem::EventManager::SendEvent(MousePositionEvent(m_MousePosition));
	}
	void Input::RegisterMouseScroll(const glm::vec2& scroll)
	{
		m_MouseScroll = scroll;

		AX::EventSystem::EventManager::SendEvent(MouseScrollEvent(m_MouseScroll));
	}

	void Input::Update()
	{
		for (auto& keyToBeHold : m_KeysToBeHold)
		{
			m_KeyStates[keyToBeHold] = State::Hold;

			auto keyAsInputKey = static_cast<Input::Key>(keyToBeHold);
			m_HoldingKeys.emplace_back(keyAsInputKey);
		}
		m_KeysToBeHold.clear();
		for (auto& keyToBeCleared : m_KeysToBeCleared)
		{
			m_KeyStates[keyToBeCleared] = State::None;

			auto keyAsInputKey = static_cast<Input::Key>(keyToBeCleared);
			for (auto i = 0; i < m_HoldingKeys.size(); ++i)
			{
				if (m_HoldingKeys[i] == keyAsInputKey)
				{
					m_HoldingKeys.erase(m_HoldingKeys.begin() + i);
					break;
				}
			}
		}
		m_KeysToBeCleared.clear();

		for (auto& mouseButtonToBeHold : m_MouseButtonsToBeHold)
		{
			m_MouseButtonStates[mouseButtonToBeHold] = State::Hold;
		}
		m_MouseButtonsToBeHold.clear();
		for (auto& mouseButtonToBeCleared : m_MouseButtonsToBeCleared)
		{
			m_MouseButtonStates[mouseButtonToBeCleared] = State::None;
		}
		m_MouseButtonsToBeCleared.clear();

		m_MouseScroll = glm::vec2(0.0f);

		for (auto& holdingKey : m_HoldingKeys)
		{
			AX::EventSystem::EventManager::SendEvent(KeyStateEvent(holdingKey, State::Hold));
		}

		for (auto& inputRegisterOperation : m_InputRegisterOperations)
		{
			inputRegisterOperation();
		}
		m_InputRegisterOperations.clear();
	}
}
