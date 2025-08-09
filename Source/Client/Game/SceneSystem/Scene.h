#ifndef _GAME_SCENESYSTEM_SCENE
#define _GAME_SCENESYSTEM_SCENE

#include <AX/ID.h>
#include "SceneSystem/Object.h"

#include <vector>
#include <optional>

namespace Game::SceneSystem
{
	class Scene
	{
		friend class SceneHelper;
	public:
		Scene() = default;

		void Reset();

		auto CreateBackgroundObject() -> AX::ID<Object>;
		auto CreateForegroundObject() -> AX::ID<Object>;
		auto CreateDynamicObject() -> AX::ID<Object>;
		auto CreateOverlayObject() -> AX::ID<Object>;
		auto CreateScreenOverlayObject() -> AX::ID<Object>;

		auto GetObject(const AX::ID<Object>& objectID) -> Object*;

		inline auto GetBackgroundObjects() const -> const std::vector<Object>&;
		inline auto GetForegroundObjects() const -> const std::vector<Object>&;
		inline auto GetDynamicObjects() const -> const std::vector<Object>&;
		inline auto GetOverlayObjects() const -> const std::vector<Object>&;
		inline auto GetScreenOverlayObjects() const -> const std::vector<Object>&;

	public:
		~Scene() noexcept = default;
		AX_SemanticsDeleteAll(Scene)

	private:
		enum class ObjectPlacementVectorType;
		struct ObjectPlacement
		{
			ObjectPlacementVectorType VectorType;
			size_t Index;
		};

	private:
		std::vector<Object> m_BackgroundObjects;
		std::vector<Object> m_ForegroundObjects;
		std::vector<Object> m_DynamicObjects;
		std::vector<Object> m_OverlayObjects;
		std::vector<Object> m_ScreenOverlayObjects;

		std::unordered_map<AX::ID<Object>, ObjectPlacement> m_ObjectIDToPlacement;
	};

	/***** IMPL *****/

	enum class Scene::ObjectPlacementVectorType
	{
		Background,
		Foreground,
		Dynamic,
		Overlay,
		ScreenOverlay
	};

	inline auto Scene::GetBackgroundObjects() const -> const std::vector<Object>&
	{
		return m_BackgroundObjects;
	}
	inline auto Scene::GetForegroundObjects() const -> const std::vector<Object>&
	{
		return m_ForegroundObjects;
	}
	inline auto Scene::GetDynamicObjects() const -> const std::vector<Object>&
	{
		return m_DynamicObjects;
	}
	inline auto Scene::GetOverlayObjects() const -> const std::vector<Object>&
	{
		return m_OverlayObjects;
	}
	inline auto Scene::GetScreenOverlayObjects() const -> const std::vector<Object>&
	{
		return m_ScreenOverlayObjects;
	}
}

#endif // !_GAME_SCENESYSTEM_SCENE
