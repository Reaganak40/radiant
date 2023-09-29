#pragma once

#ifdef RDT_PLATFORM_WINDOWS

extern rdt::Application* rdt::CreateApplication();

int main(int argc, char** argv)
{
	auto app = rdt::CreateApplication();
	app->Run();
	delete app;
}

#endif