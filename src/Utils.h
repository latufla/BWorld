#pragma once
#include <array>
#include "geometry\Geometry.h"
#include <Box2D\Common\b2Math.h>

namespace bw {
	class Utils {
	public:
		Utils() = delete;
		~Utils() = delete;

		static std::string Utils::toString(const std::array<float, 16>&);
		static std::array<float, 16>& Utils::fromBox2d(const b2Transform&, std::array<float, 16>&);
	};
}

