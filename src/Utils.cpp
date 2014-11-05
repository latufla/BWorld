#include "stdafx.h"
#include "Utils.h"
#include <gtc\type_ptr.hpp>

using std::array;

using std::string;
using std::to_string;

using glm::mat4;
using glm::value_ptr;
using glm::vec4;
using glm::vec3;


string Utils::toString(const mat4& m) {
	string res = "{";
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			res += to_string(m[j][i]) + " ";
		}
		if(i!= 3)
			res += "\n ";
	}
	res += "}";
	return res;
}

string Utils::toString(const vec3& v) {
	return "{x: " + to_string(v.x) + " y:" + to_string(v.y) + " z: " + to_string(v.z) + "}";
}

std::string Utils::toString(const std::array<float, 16>& a) {
	string res = "{";
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			res += to_string(a.at(i + j * 4)) + " ";
		}
		if (i != 3)
			res += "\n ";
	}
	res += "}";
	return res;
}


std::array<float, 16> Utils::toArray(const mat4& m) {
	array <float, 16> res;

	const float *pSource = (const float*)value_ptr(m);
	for (int i = 0; i < 16; ++i)
		res[i] = pSource[i];

	return res;
}