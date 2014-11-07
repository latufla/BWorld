#pragma once
#include <vector>
#include <string>

namespace bw{
	struct Point {
		float x;
		float y;

		operator std::string () const {
			return "{ Point x: " + std::to_string(x) + " y: " + std::to_string(y) + " }";
		}
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