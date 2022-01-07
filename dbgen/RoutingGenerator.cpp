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

	struct ltrack_index
	{
		uint32_t track{0};

		uint32_t next(int32_t incr = 1)
		{
			uint32_t oldtrack = track;
			if (incr < 0)
			{
				track = (((track - 8) + ((2147483640 + incr) % 40)) % 40) + 8;
			}
			else
			{
				track = (((track - 8) + incr) % 40) + 8;
			}
			return oldtrack;
		}
	};

	struct strack_index
	{
		uint32_t track{0};

		uint32_t next(int32_t incr = 1)
		{
			uint32_t oldtrack = track;
			if (incr < 0)
			{
				track = track + ((2147483648 + incr) & 7);
			}
			else
			{
				track = (track + incr) & 7;
			}
			return oldtrack;
		}
	};

	void RoutingGenerator::generate_horz_lwires()
	{
		ltrack_index trackIndex{8};
		for (uint32_t y = 0; y < dev.dims.y - 1; y++)
		{
			ltrack_index switchIndex{8};
			switchIndex.next(-2 * y + 2);

			uint32_t firstwire = 20 - (y % 20);

			for (uint32_t line = firstwire; line <= 20; line++)
			{
				make_wire("HL", Wire::Direction::EAST, {1, y}, {line, y}, trackIndex.next(), switchIndex.next());
				make_wire("HL", Wire::Direction::WEST, {line, y}, {1, y}, trackIndex.next(), switchIndex.next());
			}

			for (uint32_t line = 1; line < firstwire; line++)
			{
				make_wire("HL", Wire::Direction::EAST, {1, y}, {line, y}, trackIndex.next(), switchIndex.next());
				make_wire("HL", Wire::Direction::WEST, {line, y}, {1, y}, trackIndex.next(), switchIndex.next());
			}

			for (int i = 0; i < y + 1; i++)
			{
				trackIndex.next();
				trackIndex.next();
			}

			for (uint32_t line = 21; line <= dev.dims.x - 1; line++)
			{
				make_wire("HL", Wire::Direction::EAST, {line - 19, y}, {line, y}, trackIndex.next(), 10);
				make_wire("HL", Wire::Direction::WEST, {line, y}, {line - 19, y}, trackIndex.next(), 11);
			}

			for (uint32_t line = dev.dims.x - 19; line <= dev.dims.x - 1; line++)
			{
				make_wire("HL", Wire::Direction::EAST, {line, y}, {dev.dims.x - 1, y}, trackIndex.next(), 10);
				make_wire("HL", Wire::Direction::WEST, {dev.dims.x - 1, y}, {line, y}, trackIndex.next(), 11);
			}

			trackIndex.track  = 8;
			switchIndex.track = 8;
			switchIndex.next((48 - 2 * (y % 20)));

			for (uint32_t i = 0; i < 20; i++)
			{
				make_wire("HL",
				          Wire::Direction::EAST,
				          {dev.dims.x, y},
				          {dev.dims.x, y},
				          trackIndex.next(),
				          switchIndex.next());
				make_wire("HL",
				          Wire::Direction::WEST,
				          {dev.dims.x, y},
				          {dev.dims.x, y},
				          trackIndex.next(),
				          switchIndex.next());
			}
		}
	}

	void RoutingGenerator::generate_vert_lwires()
	{
		ltrack_index trackIndex{8};
		for (uint32_t x = 1; x < dev.dims.x; x++)
		{
			ltrack_index switchIndex{8};
			switchIndex.next(-2 * x + 2);
			uint32_t firstwire = 21 - (x % 20);

			for (uint32_t line = firstwire; line <= 20; line++)
			{
				make_wire("VL", Wire::Direction::NORTH, {x, 1}, {x, line}, trackIndex.next(), switchIndex.next());
				make_wire("VL", Wire::Direction::SOUTH, {x, line}, {x, 1}, trackIndex.next(), switchIndex.next());
			}

			for (uint32_t line = 1; line < firstwire; line++)
			{
				make_wire("VL", Wire::Direction::NORTH, {x, 1}, {x, line}, trackIndex.next(), switchIndex.next());
				make_wire("VL", Wire::Direction::SOUTH, {x, line}, {x, 1}, trackIndex.next(), switchIndex.next());
			}

			for (int i = 0; i < x; i++)
			{
				trackIndex.next();
				trackIndex.next();
			}

			for (uint32_t line = 21; line <= dev.dims.y - 1; line++)
			{
				make_wire("VL", Wire::Direction::NORTH, {x, line - 19}, {x, line}, trackIndex.next(), 8);
				make_wire("VL", Wire::Direction::SOUTH, {x, line}, {x, line - 19}, trackIndex.next(), 9);
			}

			for (uint32_t line = dev.dims.y - 19; line <= dev.dims.y - 1; line++)
			{
				make_wire("VL", Wire::Direction::NORTH, {x, line}, {x, dev.dims.y - 1}, trackIndex.next(), 8);
				make_wire("VL", Wire::Direction::SOUTH, {x, dev.dims.y - 1}, {x, line}, trackIndex.next(), 9);
			}

			trackIndex.track  = 8;
			switchIndex.track = 8;
			switchIndex.next(48 - 2 * ((x + 2) % 20));

			for (uint32_t i = 0; i < 20; i++)
			{
				make_wire("VL", Wire::Direction::NORTH, {x, 0}, {x, 0}, trackIndex.next(), switchIndex.next());
				make_wire("VL", Wire::Direction::SOUTH, {x, 0}, {x, 0}, trackIndex.next(), switchIndex.next());
			}
		}
	}

	void RoutingGenerator::generate_horz_swires()
	{
		strack_index trackIndex{0};
		for (uint32_t y = 0; y < dev.dims.y - 1; y++)
		{
			strack_index switchIndex{0};
			switchIndex.next((-2 * y) + 2);
			uint32_t firstwire = 4 - (y % 4);

			for (uint32_t line = firstwire; line <= 4; line++)
			{
				make_wire("HS", Wire::Direction::EAST, {1, y}, {line, y}, trackIndex.next(), switchIndex.next());
				make_wire("HS", Wire::Direction::WEST, {line, y}, {1, y}, trackIndex.next(), switchIndex.next());
			}

			for (uint32_t line = 1; line < firstwire; line++)
			{
				make_wire("HS", Wire::Direction::EAST, {1, y}, {line, y}, trackIndex.next(), switchIndex.next());
				make_wire("HS", Wire::Direction::WEST, {line, y}, {1, y}, trackIndex.next(), switchIndex.next());
			}

			for (int i = 0; i < y + 1; i++)
			{
				trackIndex.next();
				trackIndex.next();
			}

			for (uint32_t line = 5; line <= dev.dims.x - 1; line++)
			{
				make_wire("HS", Wire::Direction::EAST, {line - 3, y}, {line, y}, trackIndex.next(), 2);
				make_wire("HS", Wire::Direction::WEST, {line, y}, {line - 3, y}, trackIndex.next(), 3);
			}

			for (uint32_t line = dev.dims.x - 3; line <= dev.dims.x - 1; line++)
			{
				make_wire("HS", Wire::Direction::EAST, {line, y}, {dev.dims.x - 1, y}, trackIndex.next(), 2);
				make_wire("HS", Wire::Direction::WEST, {dev.dims.x - 1, y}, {line, y}, trackIndex.next(), 3);
			}

			trackIndex.track  = 0;
			switchIndex.track = 0;
			switchIndex.next(8 - 2 * (y % 4));

			for (uint32_t i = 0; i < 4; i++)
			{
				make_wire("HS",
				          Wire::Direction::EAST,
				          {dev.dims.x, y},
				          {dev.dims.x, y},
				          trackIndex.next(),
				          switchIndex.next());
				make_wire("HS",
				          Wire::Direction::WEST,
				          {dev.dims.x, y},
				          {dev.dims.x, y},
				          trackIndex.next(),
				          switchIndex.next());
			}
		}
	}

	void RoutingGenerator::generate_vert_swires()
	{
		strack_index trackIndex{0};
		for (uint32_t x = 1; x < dev.dims.x; x++)
		{
			strack_index switchIndex{0};
			switchIndex.next(-2 * x + 2);
			uint32_t firstwire = 5 - (x % 4);

			for (uint32_t line = firstwire; line <= 4; line++)
			{
				make_wire("VS", Wire::Direction::NORTH, {x, 1}, {x, line}, trackIndex.next(), switchIndex.next());
				make_wire("VS", Wire::Direction::SOUTH, {x, line}, {x, 1}, trackIndex.next(), switchIndex.next());
			}

			for (uint32_t line = 1; line < firstwire; line++)
			{
				make_wire("VS", Wire::Direction::NORTH, {x, 1}, {x, line}, trackIndex.next(), switchIndex.next());
				make_wire("VS", Wire::Direction::SOUTH, {x, line}, {x, 1}, trackIndex.next(), switchIndex.next());
			}

			for (int i = 0; i < x; i++)
			{
				trackIndex.next();
				trackIndex.next();
			}

			for (uint32_t line = 5; line <= dev.dims.y - 1; line++)
			{
				make_wire("VS", Wire::Direction::NORTH, {x, line - 3}, {x, line}, trackIndex.next(), 0);
				make_wire("VS", Wire::Direction::SOUTH, {x, line}, {x, line - 3}, trackIndex.next(), 1);
			}

			for (uint32_t line = dev.dims.y - 3; line <= dev.dims.y - 1; line++)
			{
				make_wire("VS", Wire::Direction::NORTH, {x, line}, {x, dev.dims.y - 1}, trackIndex.next(), 0);
				make_wire("VS", Wire::Direction::SOUTH, {x, dev.dims.y - 1}, {x, line}, trackIndex.next(), 1);
			}

			trackIndex.track  = 0;
			switchIndex.track = 0;
			switchIndex.next(8 - 2 * ((x + 2) % 4));

			for (uint32_t i = 0; i < 4; i++)
			{
				make_wire("VS", Wire::Direction::NORTH, {x, 0}, {x, 0}, trackIndex.next(), switchIndex.next());
				make_wire("VS", Wire::Direction::SOUTH, {x, 0}, {x, 0}, trackIndex.next(), switchIndex.next());
			}
		}
	}

	void RoutingGenerator::generate_semicol_clks()
	{
		for (uint32_t x = 0; x < dev.dims.x; x++)
		{
			// TODO: fix sizes to be generic
			make_wire("CC", Wire::Direction::NORTH, {x, 81}, {x, 160}, 49, 49);
			make_wire("CC", Wire::Direction::SOUTH, {x, 80}, {x, 1}, 49, 49);
			make_wire("CC", Wire::Direction::NORTH, {x, 81}, {x, 160}, 50, 50);
			make_wire("CC", Wire::Direction::SOUTH, {x, 80}, {x, 1}, 50, 50);
			make_wire("CC", Wire::Direction::NORTH, {x, 81}, {x, 160}, 51, 51);
			make_wire("CC", Wire::Direction::SOUTH, {x, 80}, {x, 1}, 51, 51);
			make_wire("CC", Wire::Direction::NORTH, {x, 81}, {x, 160}, 52, 52);
			make_wire("CC", Wire::Direction::SOUTH, {x, 80}, {x, 1}, 52, 52);
		}
	}

	void RoutingGenerator::generate_global_clks() {}

	Wire &RoutingGenerator::make_wire(
	    const data::string &type, Wire::Direction dir, vec2 start, vec2 end, uint32_t track, uint32_t sbi)
	{
		auto &wire{*dev.wires.emplace_back(data::make_unique<Wire>(Wire{}))};
		wire.start = start;
		wire.end   = end;
		wire.sbi   = sbi;
		wire.track = track;
		wire.dir   = dir;
		wire.name.push_back(dev.id(type));

		switch (dir)
		{
			case Wire::Direction::NORTH:
				wire.name.push_back(dev.id(fmt::format("X{}", start.x)));
				wire.name.push_back(dev.id("N"));
				wire.name.push_back(dev.id(fmt::format("Y[{}:{}]", start.y, end.y)));
				break;

			case Wire::Direction::SOUTH:
				wire.name.push_back(dev.id(fmt::format("X{}", start.x)));
				wire.name.push_back(dev.id("S"));
				wire.name.push_back(dev.id(fmt::format("Y[{}:{}]", start.y, end.y)));
				break;

			case Wire::Direction::EAST:
				wire.name.push_back(dev.id(fmt::format("Y{}", start.y)));
				wire.name.push_back(dev.id("E"));
				wire.name.push_back(dev.id(fmt::format("X[{}:{}]", start.x, end.x)));
				break;

			case Wire::Direction::WEST:
				wire.name.push_back(dev.id(fmt::format("Y{}", start.y)));
				wire.name.push_back(dev.id("W"));
				wire.name.push_back(dev.id(fmt::format("X[{}:{}]", start.x, end.x)));
				break;
		}

		wire.name.push_back(dev.id(fmt::format("{}", wire.track)));
		wire.name.push_back(dev.id(fmt::format("{}", wire.sbi)));
		wire.type = Wire::WireType::LOCAL;
		return wire;
	}
}
