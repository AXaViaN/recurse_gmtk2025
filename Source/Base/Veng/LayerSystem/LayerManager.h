#ifndef _VENG_LAYERSYSTEM_LAYERMANAGER
#define _VENG_LAYERSYSTEM_LAYERMANAGER

#include <AX/Tool/ProtectedContainer.h>
#include <AX/Tool/Reporter.h>
#include <Veng/LayerSystem/Layer.h>

#include <vector>
#include <memory>

namespace Veng::LayerSystem
{
	class LayerManager : private Layer
	{
	public:
		enum class Placement;

	public:
		LayerManager(Context* context);

	public:
		virtual void OnUpdate(float deltaTime) override;
		virtual void OnFixedUpdate() override;
		virtual void OnRender() override;

		template<typename T>
		inline auto AddLayer(const Placement& layerPlacement) -> std::shared_ptr<T>;
		template<typename T>
		inline auto AddLayer(const Placement& layerPlacement, const Layer& reference) -> std::shared_ptr<T>;
		template<typename T>
		inline auto AddLayer(const Placement& layerPlacement, const std::shared_ptr<Layer>& reference) -> std::shared_ptr<T>;

		void RemoveLayer(const Layer& layer);
		void RemoveLayer(const std::shared_ptr<Layer>& layer);

	private:
		template<typename T>
		inline auto EmplaceLayer(const std::vector<std::shared_ptr<Layer>>::iterator& where) -> std::shared_ptr<T>;

	private:
		AX::Tool::ProtectedContainer<std::vector<std::shared_ptr<Layer>>> m_Layers;
	};

	/***** IMPL *****/

	enum class LayerManager::Placement
	{
		Top,
		Bottom,
		Above,
		Below
	};

	template<typename T>
	inline auto LayerManager::AddLayer(const Placement& layerPlacement) -> std::shared_ptr<T>
	{
		auto placementIt = m_Layers.begin();
		if (layerPlacement == Placement::Bottom)
		{
			placementIt = m_Layers.end();
		}
		else if (layerPlacement != Placement::Top)
		{
			AX_Assert(
				false,
				"Cannot call LayerManager::AddLayer without reference "
				"unless Top or Bottom!"
			);
		}

		return EmplaceLayer<T>(placementIt);
	}
	template<typename T>
	inline auto LayerManager::AddLayer(const Placement& layerPlacement, const Layer& reference) -> std::shared_ptr<T>
	{
		auto placementIt = m_Layers.begin();
		if (layerPlacement == Placement::Bottom)
		{
			placementIt = m_Layers.end();
		}
		else
		{
			placementIt = std::find_if(
				m_Layers.begin(), m_Layers.end(),
				[&reference](const std::shared_ptr<Layer>& layer)
				{
					return (&(*layer) == &reference);
				}
			);
			AX_Assert(
				placementIt != m_Layers.end(),
				"Reference layer is not found in the stack!"
			);

			if (layerPlacement == Placement::Below)
			{
				++placementIt;
			}
		}

		return EmplaceLayer<T>(placementIt);
	}
	template<typename T>
	inline auto LayerManager::AddLayer(const Placement& layerPlacement, const std::shared_ptr<Layer>& reference) -> std::shared_ptr<T>
	{
		return AddLayer<T>(layerPlacement, *reference);
	}

	template<typename T>
	inline auto LayerManager::EmplaceLayer(const std::vector<std::shared_ptr<Layer>>::iterator& where) -> std::shared_ptr<T>
	{
		auto layer = std::make_shared<T>(*this, p_Context);
		m_Layers.emplace(where, layer, [&, layer]()
		{
			for (auto i = 0; i < m_Layers.size(); ++i)
			{
				m_Layers.at(i)->SetListenerGroupOrder(i);
			}

			layer->OnStart();
		});

		return layer;
	}
}

#endif // !_VENG_LAYERSYSTEM_LAYERMANAGER
