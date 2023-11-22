#include "pch.h"
#include "Application.h"
#include <Radiant/Logger.h>

void rdt::Application::TestImpl()
{
	logger::Log::log_to_stdout();
	RDT_CORE_INFO("Hello world!");
}
