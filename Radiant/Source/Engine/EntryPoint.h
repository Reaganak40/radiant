#pragma once
#ifdef RDT_PLATFORM_WINDOWS

extern rdt::Application* rdt::CreateApplication();

int main(int argc, char** argv)
{
	rdt::Log::Initialize();

	auto app = rdt::CreateApplication();

	app->OnStart();
	app->Run();
	delete app;
}

#endif