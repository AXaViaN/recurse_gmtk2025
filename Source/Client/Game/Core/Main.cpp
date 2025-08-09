#include "Core/ApplicationContext.h"
#include "Core/InitializationLayer.h"

#include <AX/Tool/PathManager.h>
#include <AX/Tool/Reporter.h>
#include <Veng/Core/Application.h>

int main(int argc, char** argv)
{
	auto applicationName = std::string("recurse");

	AX::Tool::PathManager::Initialize(argc, argv);
	AX::Tool::Reporter::InitLogFile("output.log", applicationName);
	{
		auto application = Veng::Core::Application(applicationName);

		application.InitializeLayerSystem<Game::Core::ApplicationContext>();
		application.AddLayer<Game::Core::InitializationLayer>();

		application.Run();
	}

	return 0;
}
