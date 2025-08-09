#ifndef _VENG_ASSET_ASSETMANAGER
#define _VENG_ASSET_ASSETMANAGER

#include <AX/ID.h>
#include <AX/Tool/Reporter.h>

#include <unordered_map>
#include <string>

namespace Veng::Asset
{
	template<typename T>
	class AssetMeta
	{
		friend class AssetManager;
	public:
		AssetMeta() = default;
		inline AssetMeta(const AX::ID<T>& id, const std::string& name);

		inline auto GetID() const -> const AX::ID<T>&;

		inline auto operator*() const -> T*;
		inline auto operator->() const -> T*;

		inline auto operator==(const AssetMeta& other) const -> bool;
		inline auto operator==(const T* other) const -> bool;
		explicit inline operator bool() const;

	private:
		std::string m_Name;
		AX::ID<T> m_ID;
	};

	class AssetManager
	{
	public:
		template<typename T>
		static inline auto CreateAsset(const std::string& name, T&& assetRef) -> AssetMeta<T>;

		template<typename T>
		static inline void RemoveAsset(AssetMeta<T>& assetMeta);

		template<typename T>
		static inline auto GetAssetPtr(const AssetMeta<T>& assetMeta) -> T*;

	private:
		template<typename T>
		static inline auto GetAssetDataMap() -> std::unordered_map<AX::ID<T>, T>&;

		template<typename T>
		static inline auto GetAssetMetaMap() -> std::unordered_map<AX::ID<T>, AssetMeta<T>>&;
	};

	/***** AssetMeta IMPL *****/

	template<typename T>
	inline AssetMeta<T>::AssetMeta(const AX::ID<T>& id, const std::string& name) :
		m_Name(name),
		m_ID(id)
	{
	}

	template<typename T>
	inline auto AssetMeta<T>::GetID() const -> const AX::ID<T>&
	{
		return m_ID;
	}

	template<typename T>
	inline auto AssetMeta<T>::operator*() const -> T*
	{
		return AssetManager::GetAssetPtr(*this);
	}
	template<typename T>
	inline auto AssetMeta<T>::operator->() const -> T*
	{
		return AssetManager::GetAssetPtr(*this);
	}

	template<typename T>
	inline auto AssetMeta<T>::operator==(const AssetMeta& other) const -> bool
	{
		return (m_ID == other.m_ID);
	}
	template<typename T>
	inline auto AssetMeta<T>::operator==(const T* other) const -> bool
	{
		return (AssetManager::GetAssetPtr(*this) == other);
	}
	template<typename T>
	inline AssetMeta<T>::operator bool() const
	{
		return m_ID.IsValid();
	}

	/***** AssetManager IMPL *****/

	template<typename T>
	inline auto AssetManager::CreateAsset(const std::string& name, T&& assetRef) -> AssetMeta<T>
	{
		auto& assetDataMap = GetAssetDataMap<T>();
		auto& assetMetaMap = GetAssetMetaMap<T>();
		
		auto assetMeta = AssetMeta(AX::IDGenerator<T>::GetNextID(), name);
		assetDataMap.emplace(assetMeta.m_ID, std::forward<T>(assetRef));
		assetMetaMap.emplace(assetMeta.m_ID, assetMeta);

		return assetMeta;
	}

	template<typename T>
	inline void AssetManager::RemoveAsset(AssetMeta<T>& assetMeta)
	{
		auto& assetDataMap = GetAssetDataMap<T>();
		auto& assetMetaMap = GetAssetMetaMap<T>();

		assetDataMap.erase(assetMeta.m_ID);
		assetMetaMap.erase(assetMeta.m_ID);

		assetMeta = {};
	}

	template<typename T>
	inline auto AssetManager::GetAssetPtr(const AssetMeta<T>& assetMeta) -> T*
	{
		auto& assetDataMap = GetAssetDataMap<T>();

		auto assetDataIt = assetDataMap.find(assetMeta.m_ID);
		if (assetDataIt == assetDataMap.end())
		{
			return nullptr;
		}

		return &(assetDataIt->second);
	}

	template<typename T>
	inline auto AssetManager::GetAssetDataMap() -> std::unordered_map<AX::ID<T>, T>&
	{
		static auto assetDataMap = std::unordered_map<AX::ID<T>, T>();
		return assetDataMap;
	}

	template<typename T>
	inline auto AssetManager::GetAssetMetaMap() -> std::unordered_map<AX::ID<T>, AssetMeta<T>>&
	{
		static auto assetMetaMap = std::unordered_map<AX::ID<T>, AssetMeta<T>>();
		return assetMetaMap;
	}
}

// To be able to use the AssetMeta as a key for unordered_map etc.
template<typename T>
struct std::hash<Veng::Asset::AssetMeta<T>>
{
	auto operator()(const Veng::Asset::AssetMeta<T>& assetMeta) const noexcept -> size_t
	{
		return hash<size_t>()(assetMeta.GetID().Value);
	}
};

#endif // !_VENG_ASSET_ASSETMANAGER
