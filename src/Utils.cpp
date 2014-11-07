#include "stdafx.h"
#include "Utils.h"

using std::array;

using std::string;
using std::to_string;

namespace bw{
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

	array<float, 16>& Utils::fromBox2d(const b2Transform& from, array<float, 16>& outColumnOrderTo) {
		const b2Vec2& pos = from.p;
		outColumnOrderTo[12] = pos.x;
		outColumnOrderTo[13] = pos.y;

		const float angle = from.q.GetAngle();
		float cs = cos(angle);
		float sn = sin(angle);
		outColumnOrderTo[0] = cs;
		outColumnOrderTo[2] = -sn;
		outColumnOrderTo[8] = sn;
		outColumnOrderTo[10] = cs;
		return outColumnOrderTo;
	}
}
