#pragma once
#include <array>
#include <Box2D\Common\b2Math.h>

namespace bw {
	class Utils {
	public:
		Utils() = delete;
		~Utils() = delete;

		static std::string Utils::toString(const std::array<float, 16>& from);
		static std::array<float, 16>& Utils::fromBox2d(const b2Transform& from, std::array<float, 16>& outColumnOrderTo);
	};
}

