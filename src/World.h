#pragma once
#include <Box2D\Box2D.h>
#include <unordered_map>
#include "geometry\Geometry.h"
#include <memory>
#include <array>

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

		bool addObject(uint32_t, ObjectType, const Point&, float = 0.0f);
		void removeObject(uint32_t);

		void attachShape(uint32_t, const Point&, float);
		void attachShape(uint32_t toObj, std::vector<Point>&);


		Point getPosition(uint32_t id);
		float getRotation(uint32_t id);

		std::array<float, 16> getTransform(uint32_t);
				
		Point getGlobalCoM(uint32_t);
		Point getLocalCoM(uint32_t);


		// slow, no proxy
		DebugShapeList getDebugShapes(uint32_t);
	
	private:
		b2World world;
		std::unordered_map<uint32_t, b2Body*> idToObject;

	};
}
