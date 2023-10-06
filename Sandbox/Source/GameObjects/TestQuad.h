#include <Radiant.h>

class TestQuad : public rdt::GameObject {
private:
	rdt::Vec2d m_spawnPos;
public:
	TestQuad(const std::string& alias, rdt::Vec2d spawnPos);
	~TestQuad();

	/*
			To implement function when the scene this object belongs to is
			being binded.
		*/
	void OnBind() override final;

	void OnRender() override final;
};