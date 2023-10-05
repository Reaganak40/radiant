#include <Radiant.h>

class TestQuad : public rdt::GameObject {
private:
public:
	TestQuad(const std::string& alias);
	~TestQuad();

	/*
			To implement function when the scene this object belongs to is
			being binded.
		*/
	void OnBind() override final;

	void OnRender() override final;
};