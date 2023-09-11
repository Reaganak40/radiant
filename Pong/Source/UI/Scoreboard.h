#include "Pong.h"
#include "GameObjects/Ball.h"

class Scoreboard : public rdt::GuiTemplate {
private:
	const float m_gui_width;
	const float m_gui_height;

	int m_window_width;
	int m_window_height;

	ImGuiWindowFlags m_window_config;
	ImFont* scoreboardFont;

	Ball& m_ball;
	unsigned int player1_score;
	unsigned int player2_score;
public:
	Scoreboard(Ball& ballReference);

	void OnUpdate(const float deltaTime) override;
	void OnRender() override;
};