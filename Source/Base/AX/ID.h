#ifndef _AX_ID
#define _AX_ID

#include <functional>

namespace AX
{
	using IDBase = size_t;

	template<typename T>
	struct ID
	{
		IDBase Value = 0ull;

	public:
		inline auto IsValid() const -> bool;

		inline auto operator==(const ID& other) const -> bool;
		explicit inline operator bool() const;
	};

	template<typename T>
	class IDGenerator
	{
	public:
		static inline auto GetNextID() -> ID<T>;

	private:
		static inline ID<T> m_NextID = { .Value = 1ull };
	};

	/***** IMPL *****/

	template<typename T>
	inline auto ID<T>::IsValid() const -> bool
	{
		return (Value != 0ull);
	}

	template<typename T>
	inline auto ID<T>::operator==(const ID& other) const -> bool
	{
		return (this->Value == other.Value);
	}
	template<typename T>
	inline ID<T>::operator bool() const
	{
		return IsValid();
	}

	template<typename T>
	inline auto IDGenerator<T>::GetNextID() -> ID<T>
	{
		auto id = m_NextID;
		++m_NextID.Value;
		return id;
	}
}

// To be able to use the ID as a key for unordered_map etc.
template<typename T>
struct std::hash<AX::ID<T>>
{
	auto operator()(const AX::ID<T>& id) const noexcept -> size_t
	{
		return hash<size_t>()(id.Value);
	}
};

#endif // !_AX_ID
