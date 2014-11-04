#pragma once
#include <Box2D\Box2D.h>
#include <unordered_map>
#include "geometry\Geometry.h"
#include <memory>

namespace bl{
	enum ObjectType {
		STATIC_OBJECT = 0,
		KINEMATIC_OBJECT,
		DYNAMIC_OBJECT,
	};

	typedef std::vector<Point> VertexList;
	typedef std::vector<std::unique_ptr<DebugShape>> DebugShapeList;

	class World {
	public:
		World();
		~World() = default;

		bool addObject(uint32_t, ObjectType, const Point&);
		void removeObject(uint32_t);

		void attachShape(uint32_t, const Point&, float);
		void attachShape(uint32_t toObj, std::vector<Point>&);

		// slow, no proxy
		DebugShapeList getDebugShapes(uint32_t);
	
	private:
		b2World world;
		std::unordered_map<uint32_t, b2Body*> idToObject;
	};
}
