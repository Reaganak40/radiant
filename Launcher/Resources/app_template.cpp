#include <Radiant.h>
#include <Engine/EntryPoint.h>

using namespace rdt;

class RADIANTPROJECTNAME : public Application
{
public:
	RADIANTPROJECTNAME()
	{
		Start("Radiant", 1280, 720);
	}

	~RADIANTPROJECTNAME()
	{

	}

};

rdt::Application* rdt::CreateApplication()
{
	return new RADIANTPROJECTNAME;
}