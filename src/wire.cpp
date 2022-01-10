//
// Created by nxmq0 on 1/2/2022.
//

#include "types.h"

#include <fmt/core.h>
#include <iostream>

namespace tritium
{
	std::string Wire::getName(Device &dev)
	{
		std::string out{dev.str(name.front())};
		for (auto id = name.begin() + 1; id != name.end(); ++id)
		{
			out += ":";
			out += dev.str(*id);
		}
		return out;
	}
	bool Wire::isVertical() const { return dir == Direction::SOUTH || dir == Direction::NORTH; }

	bool Wire::isHorizontal() const { return dir == Direction::EAST || dir == Direction::WEST; }

	uint32_t Wire::getPTrackAt(vec2 loc) const
	{
		switch(dir) {
			case Wire::Direction::NORTH:
				assert(loc.x == start.x && start.y <= loc.y && loc.y <= end.y);
				break;
			case Wire::Direction::SOUTH:
				assert(loc.x == start.x && end.y <= loc.y && loc.y <= start.y);
				break;
			case Wire::Direction::WEST:
				assert(loc.y == start.y && end.x <= loc.x && loc.x <= start.x);
				break;
			case Wire::Direction::EAST:
				assert(loc.y == start.y && start.x <= loc.x && loc.x <= end.x);
				break;
			default:
				std::cerr << "Invalid wire!!!!!\n";
				std::terminate();
		}
		const uint32_t segNum{isHorizontal() ? loc.x : loc.y};
		const uint32_t chanNum{isHorizontal() ? loc.y : loc.x};
		const uint32_t group_size{type == Type::LONG ? 40U : 8U};
		const uint32_t group_start{type == Type::LONG ? 8U : 0U};

		const uint32_t pos_mod_group{(((segNum - 1U) + (chanNum - (isHorizontal() ? 0U : 1U))) * 2U) % group_size};
		const uint32_t group_size_over_dir{group_size / 2U};
		const uint32_t vtrack_sub_group_start{track - group_start};

		return (2U * (((group_size_over_dir - (pos_mod_group / 2U)) + (vtrack_sub_group_start / 2U)) %
		              group_size_over_dir)) +
		       group_start + (vtrack_sub_group_start % 2U);
	}
}