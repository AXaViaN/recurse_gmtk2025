#ifndef _AX_TOOL_PROTECTEDCONTAINER
#define _AX_TOOL_PROTECTEDCONTAINER

#include <vector>
#include <algorithm>
#include <functional>

namespace AX::Tool
{
	// TODO_ASC: Specialize at, emplace, erase and operator[] for maps

	template<typename T>
	class ProtectedContainer
	{
	public:
		inline void StartProtection();
		inline void EndProtection();

	public:
		inline auto begin() -> T::iterator;
		inline auto end() -> T::iterator;
		inline auto begin() const -> T::const_iterator;
		inline auto end() const -> T::const_iterator;

		inline auto size() const -> T::size_type;

		inline auto at(size_t idx) -> T::value_type&;
		inline auto at(size_t idx) const -> const T::value_type&;

		inline void emplace(T::const_iterator where, T::value_type&& value, const std::function<void()>& postOperation);
		inline void emplace(T::const_iterator where, T::value_type&& value);
		inline void emplace_back(T::value_type&& value);

		inline void erase(T::const_iterator where);

		inline void clear();

	public:
		inline auto operator[](size_t idx) -> T::value_type&;
		inline auto operator[](size_t idx) const -> const T::value_type&;

	private:
		struct Operation;

	private:
		T m_Container;

		size_t m_UsageCounter = 0ull;
		std::vector<Operation> m_Operations;
	};

	/***** IMPL *****/

	namespace ProtectedContainerHelper
	{
		enum class OperationType
		{
			Add,
			Remove
		};
	}
	template<typename T>
	struct ProtectedContainer<T>::Operation
	{
		ProtectedContainerHelper::OperationType Type;
		size_t Index;
		typename T::value_type Value;
		std::function<void()> PostOperation;
	};

	template<typename T>
	inline void ProtectedContainer<T>::StartProtection()
	{
		++m_UsageCounter;
	}
	template<typename T>
	inline void ProtectedContainer<T>::EndProtection()
	{
		--m_UsageCounter;
		if (m_UsageCounter > 0)
		{
			return;
		}

		std::sort(m_Operations.begin(), m_Operations.end(), [](auto& lhs, auto& rhs)
		{
			return (lhs.Index < rhs.Index);
		});

		// TODO_ASC: This idx adjusting is problematic. Refactor this whole system
		auto idxAdjustment = 0;
		auto postOperations = std::vector<std::function<void()>>();
		for (auto& operation : m_Operations)
		{
			auto idx = static_cast<int>(operation.Index) + idxAdjustment;
			if (idx < 0)
			{
				idx = 0;
			}

			if (operation.Type == ProtectedContainerHelper::OperationType::Add)
			{
				emplace(begin() + idx, std::move(operation.Value));
				++idxAdjustment;
			}
			else
			{
				erase(begin() + idx);
				--idxAdjustment;
			}

			if (operation.PostOperation)
			{
				postOperations.emplace_back(operation.PostOperation);
			}
		}
		m_Operations.clear();

		if (postOperations.size() > 0)
		{
			StartProtection();
			for (auto& postOperation : postOperations)
			{
				postOperation();
			}
			EndProtection();
		}
	}

	template<typename T>
	inline auto ProtectedContainer<T>::begin() const -> T::const_iterator
	{
		return m_Container.begin();
	}
	template<typename T>
	inline auto ProtectedContainer<T>::end() const -> T::const_iterator
	{
		return m_Container.end();
	}
	template<typename T>
	inline auto ProtectedContainer<T>::begin() -> T::iterator
	{
		return m_Container.begin();
	}
	template<typename T>
	inline auto ProtectedContainer<T>::end() -> T::iterator
	{
		return m_Container.end();
	}

	template<typename T>
	inline auto ProtectedContainer<T>::size() const -> T::size_type
	{
		return m_Container.size();
	}

	template<typename T>
	inline auto ProtectedContainer<T>::at(size_t i) -> T::value_type&
	{
		return m_Container.at(i);
	}
	template<typename T>
	inline auto ProtectedContainer<T>::at(size_t i) const -> const T::value_type&
	{
		return m_Container.at(i);
	}

	template<typename T>
	inline void ProtectedContainer<T>::emplace(T::const_iterator where, T::value_type&& value, const std::function<void()>& postOperation)
	{
		if (m_UsageCounter == 0)
		{
			m_Container.emplace(where, value);

			if (postOperation)
			{
				postOperation();
			}

			return;
		}

		auto idx = static_cast<size_t>(where - m_Container.begin());
		m_Operations.emplace_back(Operation{
			.Type = ProtectedContainerHelper::OperationType::Add,
			.Index = static_cast<size_t>(idx),
			.Value = std::move(value),
			.PostOperation = postOperation
		});
	}
	template<typename T>
	inline void ProtectedContainer<T>::emplace(T::const_iterator where, T::value_type&& value)
	{
		emplace(where, std::move(value), {});
	}
	template<typename T>
	inline void ProtectedContainer<T>::emplace_back(T::value_type&& value)
	{
		emplace(m_Container.end(), std::move(value));
	}

	template<typename T>
	inline void ProtectedContainer<T>::erase(T::const_iterator where)
	{
		if (m_UsageCounter == 0)
		{
			m_Container.erase(where);
			return;
		}

		auto idx = static_cast<size_t>(where - m_Container.begin());
		m_Operations.emplace_back(Operation{
			.Type = ProtectedContainerHelper::OperationType::Remove,
			.Index = static_cast<size_t>(idx)
		});
	}

	template<typename T>
	inline void ProtectedContainer<T>::clear()
	{
		m_Container.clear();
	}

	template<typename T>
	inline auto ProtectedContainer<T>::operator[](size_t i) -> T::value_type&
	{
		return m_Container[i];
	}
	template<typename T>
	inline auto ProtectedContainer<T>::operator[](size_t i) const -> const T::value_type&
	{
		return m_Container[i];
	}
}

#endif // !_AX_TOOL_PROTECTEDCONTAINER
