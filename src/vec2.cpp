//
// Created by nxmq0 on 1/9/2022.
//
#include "types.h"

#include <fmt/core.h>

namespace tritium
{
	uint32_t vec2::dist(vec2 o) const
	{
		uint32_t xdist = x > o.x ? x - o.x : o.x - x;
		uint32_t ydist = y > o.y ? y - o.y : o.y - y;
		return xdist + ydist;
	}
}
