#include "SceneSystem/Scene.h"

namespace Game::SceneSystem
{
	class SceneHelper
	{
	public:
		auto CreateObject(Scene* self, Scene::ObjectPlacementVectorType vectorType) -> AX::ID<Object>;
	};

	static SceneHelper s_Helper;

	/***** CLASS IMPL *****/

	void Scene::Reset()
	{
		m_BackgroundObjects.clear();
		m_ForegroundObjects.clear();
		m_DynamicObjects.clear();
		m_OverlayObjects.clear();
		m_ScreenOverlayObjects.clear();

		m_ObjectIDToPlacement.clear();
	}

	auto Scene::CreateBackgroundObject() -> AX::ID<Object>
	{
		return s_Helper.CreateObject(this, ObjectPlacementVectorType::Background);
	}
	auto Scene::CreateForegroundObject() -> AX::ID<Object>
	{
		return s_Helper.CreateObject(this, ObjectPlacementVectorType::Foreground);
	}
	auto Scene::CreateDynamicObject() -> AX::ID<Object>
	{
		return s_Helper.CreateObject(this, ObjectPlacementVectorType::Dynamic);
	}
	auto Scene::CreateOverlayObject() -> AX::ID<Object>
	{
		return s_Helper.CreateObject(this, ObjectPlacementVectorType::Overlay);
	}
	auto Scene::CreateScreenOverlayObject() -> AX::ID<Object>
	{
		return s_Helper.CreateObject(this, ObjectPlacementVectorType::ScreenOverlay);
	}

	auto Scene::GetObject(const AX::ID<Object>& objectID) -> Object*
	{
		auto it = m_ObjectIDToPlacement.find(objectID);
		if (it == m_ObjectIDToPlacement.end())
		{
			return nullptr;
		}

		auto& placement = it->second;
		switch (placement.VectorType)
		{
			case Game::SceneSystem::Scene::ObjectPlacementVectorType::Background:
			{
				return &m_BackgroundObjects[placement.Index];
			}
			case Game::SceneSystem::Scene::ObjectPlacementVectorType::Foreground:
			{
				return &m_ForegroundObjects[placement.Index];
			}
			case Game::SceneSystem::Scene::ObjectPlacementVectorType::Dynamic:
			{
				return &m_DynamicObjects[placement.Index];
			}
			case Game::SceneSystem::Scene::ObjectPlacementVectorType::Overlay:
			{
				return &m_OverlayObjects[placement.Index];
			}
			case Game::SceneSystem::Scene::ObjectPlacementVectorType::ScreenOverlay:
			{
				return &m_ScreenOverlayObjects[placement.Index];
			}
		}

		return nullptr;
	}

	/***** HELPER IMPL *****/

	auto SceneHelper::CreateObject(Scene* self, Scene::ObjectPlacementVectorType vectorType) -> AX::ID<Object>
	{
		std::vector<Object>* objects = nullptr;
		switch (vectorType)
		{
			case Game::SceneSystem::Scene::ObjectPlacementVectorType::Background:
			{
				objects = &self->m_BackgroundObjects;
				break;
			}
			case Game::SceneSystem::Scene::ObjectPlacementVectorType::Foreground:
			{
				objects = &self->m_ForegroundObjects;
				break;
			}
			case Game::SceneSystem::Scene::ObjectPlacementVectorType::Dynamic:
			{
				objects = &self->m_DynamicObjects;
				break;
			}
			case Game::SceneSystem::Scene::ObjectPlacementVectorType::Overlay:
			{
				objects = &self->m_OverlayObjects;
				break;
			}
			case Game::SceneSystem::Scene::ObjectPlacementVectorType::ScreenOverlay:
			{
				objects = &self->m_ScreenOverlayObjects;
				break;
			}
		}

		auto id = AX::IDGenerator<Object>::GetNextID();
		auto placement = Scene::ObjectPlacement
		{
			.VectorType = vectorType,
			.Index = objects->size()
		};

		objects->emplace_back();
		self->m_ObjectIDToPlacement[id] = placement;

		return id;
	}
}
