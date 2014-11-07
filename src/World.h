#pragma once
#include <Box2D\Box2D.h>
#include <unordered_map>
#include <array>
#include <utility>

#include "ContactListener.h"

namespace bw{
	enum ObjectType {
		STATIC_OBJECT = 0,
		KINEMATIC_OBJECT,
		DYNAMIC_OBJECT,
	};
	
	struct Point {
		float x;
		float y;

		operator std::string() const {
			return "{ Point x: " + std::to_string(x) + " y: " + std::to_string(y) + " }";
		}
	};

	typedef std::vector<Point> VertexList;

	class World {
	public:
		World();
		~World() = default;

		bool addObject(uint32_t, ObjectType, const Point&, float = 0.0f);
		void removeObject(uint32_t);

		void attachShape(uint32_t, const Point&, float);
		void attachShape(uint32_t toObj, std::vector<Point>&);

		void doStep(float); // mSec

		void setDensity(uint32_t, float);
		void setRestitution(uint32_t, float);
		void setFriction(uint32_t, float); // via objects, not via world and objects
		
		void setSensor(uint32_t, bool);

		void applyLinearImpulse(uint32_t, const Point&);
		void setLinearDamping(uint32_t, float); // ex: damping = 2.0f -> after 1 second velocity /= 2
		
		Point getPosition(uint32_t id);
		float getRotation(uint32_t id);

		std::array<float, 16> getTransform(uint32_t); // renderer oriented
				
		Point getGlobalCoM(uint32_t);
		Point getLocalCoM(uint32_t);

		std::string objectToString(uint32_t);
		
		std::vector<uint32_t> checkContacts(uint32_t);
		bool hasContact(uint32_t, const Point&);

		// renderer oriented
		// in object coordinates -> once get, then apply transform
		std::vector<VertexList> getDebugLocalShapes(uint32_t); 
	
	private:
		void doContactsStep();

		VertexList createDebugCircle(b2CircleShape*, float);
		VertexList createDebugPolygon(b2PolygonShape*);

		const uint32_t VELOCITY_ITERATIONS = 6;
		const uint32_t POSITION_ITERATIONS = 2;

		const uint8_t DEBUG_CIRCLE_SHAPE_STEPS = 8;
		
		b2World world;
		std::unordered_map<uint32_t, b2Body*> idToObject;
		std::unordered_map<b2Body*, uint32_t> objectToId;

		ContactListener contactListener;
		std::vector < std::pair<uint32_t, uint32_t> > contacts;
	
	};
}
