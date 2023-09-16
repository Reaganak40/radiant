#include "Game.h"

enum UIAlignment {
	TEXT_CENTERED,
	TEXT_LEFT,
	TEXT_RIGHT
};
class UI : public rdt::GameObject {
private:
	rdt::Vec2d m_origin;
	std::string m_text;
	int m_numTiles;
	rdt::Vec2d m_tileSize;
	std::vector<rdt::UniqueID> m_model_IDs;

	UIAlignment m_alignment;
	bool m_show;
public:

	UI(int maxTiles, rdt::Vec2d tileSize);
	~UI();

	void OnBind() override final;
	void OnRelease() override final;
	void OnProcessInput(const float deltaTIme) override final;
	void OnFinalUpdate() override final;
	void OnRender() override final;

	void SetOrigin(rdt::Vec2d nOrigin);
	void SetText(const std::string& nText);
	void SetAlignment(UIAlignment nAlign);
	void SetShow(bool nShow);
	bool IsShowing();
private:
	rdt::Vec2i GetAtlasCoords(char c);
	void DrawTile(char c, int tile);
};