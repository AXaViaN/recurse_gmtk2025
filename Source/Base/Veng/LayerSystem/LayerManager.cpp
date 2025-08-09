#include <Veng/LayerSystem/LayerManager.h>

namespace Veng::LayerSystem
{
	LayerManager::LayerManager(Context* context) : Layer(*this, context, "LayerManager")
	{
	}

	void LayerManager::OnUpdate(float deltaTime)
	{
		m_Layers.StartProtection();
		for (auto& layer : m_Layers)
		{
			layer->OnUpdate(deltaTime);
		}
		m_Layers.EndProtection();
	}
	void LayerManager::OnFixedUpdate()
	{
		m_Layers.StartProtection();
		for (auto& layer : m_Layers)
		{
			layer->OnFixedUpdate();
		}
		m_Layers.EndProtection();
	}
	void LayerManager::OnRender()
	{
		m_Layers.StartProtection();
		for (auto& layer : m_Layers)
		{
			layer->OnRender();
		}
		m_Layers.EndProtection();
	}

	void LayerManager::RemoveLayer(const Layer& layer)
	{
		for (auto i = 0; i < m_Layers.size(); ++i)
		{
			auto& currentLayer = m_Layers[i];
			if (&(*currentLayer) == &layer)
			{
				currentLayer->OnDispose();
				m_Layers.erase(m_Layers.begin() + i);
				break;
			}
		}
	}
	void LayerManager::RemoveLayer(const std::shared_ptr<Layer>& layer)
	{
		RemoveLayer(*layer);
	}
}
