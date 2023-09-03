#include "Polygon/Polygon.h"

namespace Radiant {
	class Collision {
	private:
	public:
		Collision() {}
		~Collision() {}

		static bool CheckCollisionSAT(const Polygon& A, const Polygon& B);
	private:
		static void GetProjections(const std::vector<Vec2d>& vertices, const Vec2d& axis, double& outMin, double& outMax);
	};

}