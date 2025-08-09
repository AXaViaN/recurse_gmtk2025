#ifndef _VENG_LAYERSYSTEM_LAYER
#define _VENG_LAYERSYSTEM_LAYER

#include <AX/EventSystem/EventManager.h>
#include <AX/Tool/Macro.h>
#include <Veng/IO/Input.h>
#include <Veng/IO/Window.h>
#include <Veng/LayerSystem/Context.h>

namespace Veng::LayerSystem
{
	// Usage:
	//		class CustomLayer : public Layer
	//		{
	//			VENG_ImplementLayer(CustomLayer)
	//			
	//			...
	//		}
	#define VENG_ImplementLayer(layerType) \
		public: \
		inline layerType(Veng::LayerSystem::LayerManager& layerManager, Veng::LayerSystem::Context* context) : \
			Veng::LayerSystem::Layer(layerManager, context, #layerType) {}

	class LayerManager;

	class Layer
	{
	public:
		inline virtual void OnStart();
		inline virtual void OnDispose();
		inline virtual void OnUpdate(float deltaTime);
		inline virtual void OnFixedUpdate();
		inline virtual void OnRender();

		inline void SetListenerGroupOrder(const size_t& order);

		inline auto GetName() const -> const std::string&;

	public:
		inline virtual ~Layer() noexcept;
		AX_SemanticsDeleteAll(Layer)

	public:
		// TODO_ASC: TMP
		static constexpr float FixedDeltaTime = 1.0f / 50.0f;

	protected:
		inline Layer(LayerManager& layerManager, Context* context, const std::string& name);

		template<typename TContext>
		inline auto GetContext() -> TContext&;

	protected:
		LayerManager& p_LayerManager;
		Context* p_Context;
		AX::EventSystem::EventListenerGroup p_EventListenerGroup;

	private:
		std::string m_Name;
	};

	/***** IMPL *****/

	inline void Layer::OnStart()
	{
	}
	inline void Layer::OnDispose()
	{
	}
	inline void Layer::OnUpdate(float /*deltaTime*/)
	{
	}
	inline void Layer::OnFixedUpdate()
	{
	}
	inline void Layer::OnRender()
	{
	}

	inline void Layer::SetListenerGroupOrder(const size_t& order)
	{
		AX::EventSystem::EventManager::SetListenerGroupOrder(p_EventListenerGroup, order);
	}

	inline auto Layer::GetName() const -> const std::string&
	{
		return m_Name;
	}

	inline Layer::~Layer() noexcept
	{
		AX::EventSystem::EventManager::DestroyListenerGroup(p_EventListenerGroup);
	}

	inline Layer::Layer(LayerManager& layerManager, Context* context, const std::string& name) :
		p_LayerManager(layerManager),
		p_Context(context),
		p_EventListenerGroup(AX::EventSystem::EventManager::CreateListenerGroup()),
		m_Name(name)
	{
	}

	template<typename TContext>
	inline auto Layer::GetContext() -> TContext&
	{
		return *static_cast<TContext*>(p_Context);
	}
}

#endif // !_VENG_LAYERSYSTEM_LAYER
