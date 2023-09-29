#include "Pong.h"

class ReturnMenu : public rdt::GuiTemplate {
private:
	const float m_gui_width;
	const float m_gui_height;

	int m_window_width;
	int m_window_height;

	ImGuiWindowFlags m_window_config;
	ImFont* m_return_font;

	bool m_return_requested;
public:
	ReturnMenu();
	~ReturnMenu();

	void OnUpdate(const float deltaTime) override;
	void OnRender() override;

	bool RequestedMenu() { return m_return_requested; }
	void Reset() { m_return_requested = false; }
};