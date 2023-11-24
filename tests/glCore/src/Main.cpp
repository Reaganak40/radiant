#include <memory>
#include <string>
#include <vector>

#include <Radiant/Utils.h>
#include <Radiant/Logger.h>
#include <Radiant/glCore.h>

using namespace rdt;

int main(void) {
	rdt::logger::Log::log_to_stdout();

	auto config = CreateWindowConfig();
	config->SetWindowName("OpenGL Showcase");
	config->SetBackgroundColor({0.1f, 0.1f, 0.1, 1.0f});
	config->SetAspectRatio(AR_1_1);
	config->SetShouldMaximizeWindow(true);

	glCore::glWindow testApp;
	testApp.LaunchWindow(config);

	glCore::glVertex vertices[4];

	vertices[0].SetPosition( -0.5f, -0.5f, 0.0f);
	vertices[1].SetPosition(  0.5f, -0.5f, 0.0f);
	vertices[2].SetPosition(  0.5f,  0.5f, 0.0f);
	vertices[3].SetPosition( -0.5f,  0.5f, 0.0f);

	vertices[0].SetColor(0.2f, 0.2f, 0.8f, 1.0f);
	vertices[1].SetColor(0.8f, 0.2f, 0.2f, 1.0f);
	vertices[2].SetColor(0.2f, 0.8f, 0.2f, 1.0f);
	vertices[3].SetColor(0.8f, 0.2f, 0.2f, 1.0f);

	vertices[0].SetTexCoords(0.0f, 0.0f);
	vertices[1].SetTexCoords(0.0f, 0.0f);
	vertices[2].SetTexCoords(0.0f, 0.0f);
	vertices[3].SetTexCoords(0.0f, 0.0f);
	
	vertices[0].SetTexIndex(0.0f);
	vertices[1].SetTexIndex(0.0f);
	vertices[2].SetTexIndex(0.0f);
	vertices[3].SetTexIndex(0.0f);

	unsigned int indices[] = { 0, 1, 2, 2, 3, 0 };

	while (!testApp.AppShouldClose()) {
		
		/* Prepare for next draw frame */
		testApp.BeginFrame();
		testApp.Clear();

		/* Draw Calls */
		testApp.SubmitVertices(vertices, 4);
		testApp.SubmitIndices(indices, 6, 4);
		testApp.Draw();

		/* Show draws and poll window events */
		testApp.EndFrame();
	}

	return 0;
}