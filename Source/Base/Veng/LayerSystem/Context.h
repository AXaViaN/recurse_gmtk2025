#ifndef _VENG_LAYERSYSTEM_CONTEXT
#define _VENG_LAYERSYSTEM_CONTEXT

#include <Veng/IO/Input.h>
#include <Veng/IO/Window.h>

namespace Veng::LayerSystem
{
	// Usage:
	//		class CustomContext : public Context
	//		{
	//			VENG_ImplementContext(CustomContext)
	//			
	//			...
	//		}
	#define VENG_ImplementContext(contextType) \
		public: \
		inline contextType(Veng::IO::Window& window, Veng::IO::Input& input) : \
			Veng::LayerSystem::Context(window, input) {}

	class Context
	{
	public:
		inline Context(IO::Window& window, IO::Input& input);

	public:
		IO::Window& Window;
		IO::Input& Input;
	};

	/***** IMPL *****/

	inline Context::Context(IO::Window& window, IO::Input& input) :
		Window(window),
		Input(input)
	{
	}
}

#endif // !_VENG_LAYERSYSTEM_LAYER
