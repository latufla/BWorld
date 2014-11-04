#include <vector>

namespace bl{
	struct Point {
		float x;
		float y;
	};

	struct DebugShape {};

	struct DebugCircle : public DebugShape {
		DebugCircle(const Point& center, float radius) {
			this->center = center;
			this->radius = radius;
		}
		Point center;
		float radius;
	};

	typedef std::vector<Point> VertexList;
	struct DebugPolygon : public DebugShape {
		DebugPolygon(const VertexList& vertices) {
			this->vertices = vertices;
		}
		VertexList vertices;
	};
}