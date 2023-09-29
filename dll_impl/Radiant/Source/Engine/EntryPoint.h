#pragma once

#ifdef RDT_PLATFORM_WINDOWS

extern rdt::Application* rdt::CreateApplication();

int main(int argc, char** argv)
{
	rdt::Log::Init();
	RDT_CORE_TRACE("Starting Engine!");
	int a = 5;
	RDT_FATAL("App initialized! {0}", a);

	auto app = rdt::CreateApplication();
	app->Run();
	delete app;
}

#endif