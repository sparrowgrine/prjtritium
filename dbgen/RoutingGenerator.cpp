//
// Created by nxmq0 on 1/3/2022.
//

#include "RoutingGenerator.h"

#include <fmt/core.h>

namespace tritium
{
	void RoutingGenerator::generateRoutes()
	{
		generate_horz_lwires();
		generate_vert_lwires();
		generate_horz_swires();
		generate_vert_swires();
		generate_global_clks();
		generate_semicol_clks();
	}

	inline uint32_t next_ltrack(uint32_t track, int32_t incr = 1)
	{
		if (incr < 0)
		{
			return next_ltrack(track, 2147483616 + incr);
		}
		return (((track - 8) + incr) % 40) + 8;
	}

	inline uint32_t next_strack(uint32_t track, int32_t incr = 1)
	{
		if (incr < 0)
		{
			return next_strack(track, 2147483646 + incr);
		}
		return (track + incr) & 7;
	}

#define nextidx index = next_ltrack(index, 1)
#define nextsbi pairIndex = next_ltrack(pairIndex, 1)

	void RoutingGenerator::generate_horz_lwires()
	{
		uint32_t index = 8;
		for (uint32_t y = 0; y < dev.dims.y - 1; y++)
		{
			uint32_t pairIndex = next_ltrack(8, -2 * y + 2);
			uint32_t firstwire = 20 - (y % 20);
			for (uint32_t line = firstwire; line <= 20; line++)
			{
				mk_wire(
				  "HL", Wire::Direction::EAST, {1, y}, {line, y}, nextidx, nextsbi);
				mk_wire(
				  "HL", Wire::Direction::WEST, {line, y}, {1, y}, nextidx, nextsbi);
			}
			for (uint32_t line = 1; line <= firstwire; line++)
			{
				mk_wire(
				  "HL", Wire::Direction::EAST, {1, y}, {line, y}, nextidx, nextsbi);
				mk_wire(
				  "HL", Wire::Direction::WEST, {line, y}, {1, y}, nextidx, nextsbi);
			}
			for (int i = 0; i < y + 1; i++)
			{
				nextidx;
				nextidx;
			}
			for (uint32_t line = 21; line <= dev.dims.x - 1; line++)
			{
				mk_wire(
				  "HL", Wire::Direction::EAST, {line - 19, y}, {line, y}, nextidx, 10);
				mk_wire(
				  "HL", Wire::Direction::WEST, {line, y}, {line - 19, y}, nextidx, 11);
			}
			for (uint32_t line = dev.dims.x - 19; line <= dev.dims.x - 1; line++)
			{
				mk_wire("HL",
				        Wire::Direction::EAST,
				        {line, y},
				        {dev.dims.x - 1, y},
				        nextidx,
				        10);
				mk_wire("HL",
				        Wire::Direction::WEST,
				        {dev.dims.x - 1, y},
				        {line, y},
				        nextidx,
				        11);
			}
			index     = 8;
			pairIndex = next_ltrack(8, (48 - 2 * (y % 20)));
			for (uint32_t i = 0; i < 20; i++)
			{
				mk_wire("HL",
				        Wire::Direction::EAST,
				        {dev.dims.x, y},
				        {dev.dims.x, y},
				        nextidx,
				        nextsbi);
				mk_wire("HL",
				        Wire::Direction::WEST,
				        {dev.dims.x, y},
				        {dev.dims.x, y},
				        nextidx,
				        nextsbi);
			}
		}
	}

	void RoutingGenerator::generate_vert_lwires()
	{
		uint32_t index = 8;
		for (uint32_t x = 0; x < dev.dims.x; x++)
		{
			uint32_t pairIndex = next_ltrack(8, -2 * x + 2);
			uint32_t firstwire = 21 - (x % 20);
			for (uint32_t line = firstwire; line <= 20; line++)
			{
				mk_wire(
				  "VL", Wire::Direction::NORTH, {x, 1}, {x, line}, nextidx, nextsbi);
				mk_wire(
				  "VL", Wire::Direction::SOUTH, {x, line}, {x, 1}, nextidx, nextsbi);
			}
			for (uint32_t line = 1; line <= firstwire; line++)
			{
				mk_wire(
				  "VL", Wire::Direction::NORTH, {x, 1}, {x, line}, nextidx, nextsbi);
				mk_wire(
				  "VL", Wire::Direction::SOUTH, {x, line}, {x, 1}, nextidx, nextsbi);
			}
			for (int i = 0; i < x; i++)
			{
				nextidx;
				nextidx;
			}
			for (uint32_t line = 21; line <= dev.dims.y - 1; line++)
			{
				mk_wire(
				  "VL", Wire::Direction::NORTH, {x, line - 19}, {x, line}, nextidx, 8);
				mk_wire(
				  "VL", Wire::Direction::SOUTH, {x, line}, {x, line - 19}, nextidx, 9);
			}
			for (uint32_t line = dev.dims.y - 19; line <= dev.dims.y - 1; line++)
			{
				mk_wire("VL",
				        Wire::Direction::NORTH,
				        {x, line},
				        {x, dev.dims.y - 1},
				        nextidx,
				        8);
				mk_wire("VL",
				        Wire::Direction::SOUTH,
				        {x, dev.dims.y - 1},
				        {x, line},
				        nextidx,
				        9);
			}
			index     = 8;
			pairIndex = next_ltrack(8, (48 - 2 * ((x + 2) % 20)));
			for (uint32_t i = 0; i < 20; i++)
			{
				mk_wire(
				  "VL", Wire::Direction::NORTH, {x, 0}, {x, 0}, nextidx, nextsbi);
				mk_wire(
				  "VL", Wire::Direction::SOUTH, {x, 0}, {x, 0}, nextidx, nextsbi);
			}
		}
	}

#undef nextidx
#undef nextsbi
#define nextidx index = next_strack(index, 1)
#define nextsbi pairIndex = next_strack(pairIndex, 1)

	void RoutingGenerator::generate_horz_swires()
	{
		uint32_t index = 0;
		for (uint32_t y = 0; y < dev.dims.y - 1; y++)
		{
			uint32_t pairIndex = next_ltrack(8, -2 * y + 2);
			uint32_t firstwire = 4 - (y % 4);
			for (uint32_t line = firstwire; line <= 4; line++)
			{
				mk_wire(
				  "HS", Wire::Direction::EAST, {1, y}, {line, y}, nextidx, nextsbi);
				mk_wire(
				  "HS", Wire::Direction::WEST, {line, y}, {1, y}, nextidx, nextsbi);
			}
			for (uint32_t line = 1; line <= firstwire; line++)
			{
				mk_wire(
				  "HS", Wire::Direction::EAST, {1, y}, {line, y}, nextidx, nextsbi);
				mk_wire(
				  "HS", Wire::Direction::WEST, {line, y}, {1, y}, nextidx, nextsbi);
			}
			for (int i = 0; i < y + 1; i++)
			{
				nextidx;
				nextidx;
			}
			for (uint32_t line = 5; line <= dev.dims.x - 1; line++)
			{
				mk_wire(
				  "HS", Wire::Direction::EAST, {line - 3, y}, {line, y}, nextidx, 2);
				mk_wire(
				  "HS", Wire::Direction::WEST, {line, y}, {line - 3, y}, nextidx, 3);
			}
			for (uint32_t line = dev.dims.x - 3; line <= dev.dims.x - 1; line++)
			{
				mk_wire("HS",
				        Wire::Direction::EAST,
				        {line, y},
				        {dev.dims.x - 1, y},
				        nextidx,
				        2);
				mk_wire("HS",
				        Wire::Direction::WEST,
				        {dev.dims.x - 1, y},
				        {line, y},
				        nextidx,
				        3);
			}
			index     = 0;
			pairIndex = next_ltrack(0, (8 - 2 * (y % 4)));
			for (uint32_t i = 0; i < 4; i++)
			{
				mk_wire("HS",
				        Wire::Direction::EAST,
				        {dev.dims.x, y},
				        {dev.dims.x, y},
				        nextidx,
				        nextsbi);
				mk_wire("HS",
				        Wire::Direction::WEST,
				        {dev.dims.x, y},
				        {dev.dims.x, y},
				        nextidx,
				        nextsbi);
			}
		}
	}

	void RoutingGenerator::generate_vert_swires()
	{
		uint32_t index = 0;
		for (uint32_t x = 0; x < dev.dims.x; x++)
		{
			uint32_t pairIndex = next_ltrack(0, -2 * x + 2);
			uint32_t firstwire = 5 - (x % 4);
			for (uint32_t line = firstwire; line <= 4; line++)
			{
				mk_wire(
				  "VS", Wire::Direction::NORTH, {x, 1}, {x, line}, nextidx, nextsbi);
				mk_wire(
				  "VS", Wire::Direction::SOUTH, {x, line}, {x, 1}, nextidx, nextsbi);
			}
			for (uint32_t line = 1; line <= firstwire; line++)
			{
				mk_wire(
				  "VS", Wire::Direction::NORTH, {x, 1}, {x, line}, nextidx, nextsbi);
				mk_wire(
				  "VS", Wire::Direction::SOUTH, {x, line}, {x, 1}, nextidx, nextsbi);
			}
			for (int i = 0; i < x; i++)
			{
				nextidx;
				nextidx;
			}
			for (uint32_t line = 21; line <= dev.dims.y - 1; line++)
			{
				mk_wire(
				  "VS", Wire::Direction::NORTH, {x, line - 3}, {x, line}, nextidx, 0);
				mk_wire(
				  "VS", Wire::Direction::SOUTH, {x, line}, {x, line - 3}, nextidx, 1);
			}
			for (uint32_t line = dev.dims.y - 3; line <= dev.dims.y - 1; line++)
			{
				mk_wire("VS",
				        Wire::Direction::NORTH,
				        {x, line},
				        {x, dev.dims.y - 1},
				        nextidx,
				        0);
				mk_wire("VS",
				        Wire::Direction::SOUTH,
				        {x, dev.dims.y - 1},
				        {x, line},
				        nextidx,
				        1);
			}
			index     = 0;
			pairIndex = next_ltrack(0, (8 - 2 * ((x + 2) % 4)));
			for (uint32_t i = 0; i < 20; i++)
			{
				mk_wire(
				  "VS", Wire::Direction::NORTH, {x, 0}, {x, 0}, nextidx, nextsbi);
				mk_wire(
				  "VS", Wire::Direction::SOUTH, {x, 0}, {x, 0}, nextidx, nextsbi);
			}
		}
	}

#undef nextidx
#undef nextsbi

	void RoutingGenerator::generate_semicol_clks()
	{
		for (uint32_t x = 0; x < dev.dims.x; x++)
		{
			// TODO: fix sizes to be generic
			mk_wire("CC", Wire::Direction::NORTH, {x, 81}, {x, 160}, 50, 50);
			mk_wire("CC", Wire::Direction::SOUTH, {x, 80}, {x, 1}, 50, 50);
			mk_wire("CC", Wire::Direction::NORTH, {x, 81}, {x, 160}, 51, 51);
			mk_wire("CC", Wire::Direction::SOUTH, {x, 80}, {x, 1}, 51, 51);
			mk_wire("CC", Wire::Direction::NORTH, {x, 81}, {x, 160}, 52, 52);
			mk_wire("CC", Wire::Direction::SOUTH, {x, 80}, {x, 1}, 52, 52);
			mk_wire("CC", Wire::Direction::NORTH, {x, 81}, {x, 160}, 53, 53);
			mk_wire("CC", Wire::Direction::SOUTH, {x, 80}, {x, 1}, 53, 53);
		}
	}

	void RoutingGenerator::generate_global_clks() {}

	Wire &RoutingGenerator::mk_wire(const data::string &type,
	                                Wire::Direction dir,
	                                vec2 start,
	                                vec2 end,
	                                uint32_t track,
	                                uint32_t sbi)
	{
		auto &wire = *dev.wires.emplace_back(data::make_unique<Wire>(Wire{}));
		wire.start = start;
		wire.end   = end;
		wire.track = track;
		wire.switchBoxIndex = sbi;
		wire.name.push_back(dev.id(type).idx);
		switch (dir)
		{
			case Wire::Direction::NORTH:
				wire.name.push_back(dev.id(fmt::format("X{}", start.x)).idx);
				wire.name.push_back(dev.id("N").idx);
				wire.name.push_back(
				  dev.id(fmt::format("Y[{}:{}]", start.y, end.y)).idx);
				break;
			case Wire::Direction::SOUTH:
				wire.name.push_back(dev.id(fmt::format("X{}", start.x)).idx);
				wire.name.push_back(dev.id("S").idx);
				wire.name.push_back(
				  dev.id(fmt::format("Y[{}:{}]", start.y, end.y)).idx);
				break;
			case Wire::Direction::EAST:
				wire.name.push_back(dev.id(fmt::format("Y{}", start.y)).idx);
				wire.name.push_back(dev.id("E").idx);
				wire.name.push_back(
				  dev.id(fmt::format("X[{}:{}]", start.x, end.x)).idx);
				break;
			case Wire::Direction::WEST:
				wire.name.push_back(dev.id(fmt::format("Y{}", start.y)).idx);
				wire.name.push_back(dev.id("W").idx);
				wire.name.push_back(
				  dev.id(fmt::format("X[{}:{}]", start.x, end.x)).idx);
				break;
		}
		wire.name.push_back(dev.id(fmt::format("{}", track)).idx);
		wire.name.push_back(dev.id(fmt::format("{}", sbi)).idx);
		wire.type = Wire::WireType::LOCAL;
		return wire;
	}
}
