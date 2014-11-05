#pragma once
#include <glm.hpp>
#include <array>

class Utils
{
public:
	Utils() = delete;
	~Utils() = delete;

	static std::string defaultVertexShader;
	static std::string defaultFragmentShader;

	static std::array<float, 16> Utils::toArray(const glm::mat4&);

	static std::string Utils::toString(const glm::mat4&);
	static std::string Utils::toString(const glm::vec3&);
	static std::string Utils::toString(const std::array<float, 16>&);
};

