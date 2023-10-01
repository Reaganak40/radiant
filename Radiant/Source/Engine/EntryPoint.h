#pragma once
#ifdef RDT_PLATFORM_WINDOWS

extern rdt::Application* rdt::CreateApplication();

int main(int argc, char** argv)
{
	rdt::Log::Init();

	auto app = rdt::CreateApplication();
	app->Run();
	delete app;
}

#endif