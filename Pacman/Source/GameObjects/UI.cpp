#include "UI.h"

using namespace rdt;


UI::UI(int maxTiles, Vec2d tileSize)
{
	m_numTiles = maxTiles;
	m_tileSize = tileSize;
	m_alignment = TEXT_RIGHT;
	m_show = true;
}

UI::~UI()
{
}

void UI::OnBind()
{

	for (int i = 0; i < m_numTiles; i++) {
		Vec2d spawnPos(m_origin.x + (i * m_tileSize.x), m_origin.y);
		m_model_ID = Physics::CreateObject(GetRealmID(), std::make_shared<Rect>(spawnPos, m_tileSize.x, m_tileSize.y));
		Physics::SetObjectProperties(GetRealmID(), m_model_ID, NoCollision);
		Physics::SetMaximumVelocity(GetRealmID(), m_model_ID, Vec2d::Zero());
		Physics::SetAcceleration(GetRealmID(), m_model_ID, Vec2d::Zero());

		m_model_IDs.push_back(m_model_ID);
	}
}

void UI::OnRelease()
{
}

void UI::OnProcessInput(const float deltaTIme)
{
}

void UI::OnFinalUpdate()
{
}

void UI::OnRender()
{
	if (!m_show || m_text.empty()) {
		return;
	}

	if (m_alignment == TEXT_RIGHT) {
		int currTile = m_numTiles - 1;
		int currChar = m_text.size() - 1;

		while (currTile >= 0 && currChar >= 0) {
			DrawTile(m_text[currChar], currTile);
			currTile--;
			currChar--;
		}
	}
	else if (m_alignment == TEXT_LEFT) {
		int currTile = 0;
		int currChar = 0;

		while (currTile < m_numTiles && currChar < m_text.size()) {
			DrawTile(m_text[currChar], currTile);
			currChar++;
			currTile++;
		}
	}
	else {
		int currTile = (m_numTiles - m_text.size()) / 2;;
		int currChar = 0;

		while (currTile < m_numTiles && currChar < m_text.size()) {
			DrawTile(m_text[currChar], currTile);
			currChar++;
			currTile++;
		}
	}

}

void UI::SetOrigin(rdt::Vec2d nOrigin)
{
	m_origin = nOrigin;
}

void UI::SetText(const std::string& nText)
{
	m_text = nText;
}

void UI::SetAlignment(UIAlignment nAlign)
{
	m_alignment = nAlign;
}

void UI::SetShow(bool nShow)
{
	m_show = nShow;
}

bool UI::IsShowing()
{
	return m_show;
}

rdt::Vec2i UI::GetAtlasCoords(char c)
{
	switch (c) {
	case 'A':
		return Vec2i(1, 0);
	case 'B':
		return Vec2i(1, 1);
	case 'C':
		return Vec2i(1, 2);
	case 'D':
		return Vec2i(1, 3);
	case 'E':
		return Vec2i(1, 4);
	case 'F':
		return Vec2i(1, 5);
	case 'G':
		return Vec2i(1, 6);
	case 'H':
		return Vec2i(1, 7);
	case 'I':
		return Vec2i(1, 8);
	case 'J':
		return Vec2i(1, 9);
	case 'K':
		return Vec2i(1, 10);
	case 'L':
		return Vec2i(1, 11);
	case 'M':
		return Vec2i(1, 12);
	case 'N':
		return Vec2i(1, 13);
	case 'O':
		return Vec2i(1, 14);
	case 'P':
		return Vec2i(0, 0);
	case 'Q':
		return Vec2i(0, 1);
	case 'R':
		return Vec2i(0, 2);
	case 'S':
		return Vec2i(0, 3);
	case 'T':
		return Vec2i(0, 4);
	case 'U':
		return Vec2i(0, 5);
	case 'V':
		return Vec2i(0, 6);
	case 'W':
		return Vec2i(0, 7);
	case 'X':
		return Vec2i(0, 8);
	case 'Y':
		return Vec2i(0, 9);
	case 'Z':
		return Vec2i(0, 10);
	case '0':
		return Vec2i(2, 0);
	case '1':
		return Vec2i(2, 1);
	case '2':
		return Vec2i(2, 2);
	case '3':
		return Vec2i(2, 3);
	case '4':
		return Vec2i(2, 4);
	case '5':
		return Vec2i(2, 5);
	case '6':
		return Vec2i(2, 6);
	case '7':
		return Vec2i(2, 7);
	case '8':
		return Vec2i(2, 8);
	case '9':
		return Vec2i(2, 9);
	case '/':
		return Vec2i(2, 10);
	case '!':
		return Vec2i(0, 11);
	case '-':
		return Vec2i(2, 11);
	default:
		return Vec2i(2, 12);
	}
}

void UI::DrawTile(char c, int tile)
{
	Renderer::Begin();
	Vec2i atlasCoords = GetAtlasCoords(c);
	Renderer::SetPolygonTexture("font", atlasCoords.x, atlasCoords.y);
	Renderer::AddPolygon(Physics::GetPolygon(GetRealmID(), m_model_IDs[tile]));
	Renderer::End();
}
