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

		ltrack_index &next(int32_t incr = 1)
		{
			if (incr < 0) return next(2147483616 + incr);
			track = (((track - 8) + incr) % 40) + 8;
			return *this;
		}

		const uint32_t &operator*() const { return track; }
	};

	struct strack_index
	{
		uint32_t track{0};

		strack_index &next(int32_t incr = 1)
		{
			if (incr < 0) return next(2147483646 + incr);
			track = (track + incr) & 7;
			return *this;
		}

		const uint32_t &operator*() const { return track; }
	};

	void RoutingGenerator::generate_horz_lwires()
	{
		ltrack_index unknownIndex{8};
		for (uint32_t y = 0; y < dev.dims.y - 1; y++)
		{
			ltrack_index trackIndex{8};
			trackIndex.next(-2 * y + 2);

			uint32_t firstwire = 20 - (y % 20);

			for (uint32_t line = firstwire; line <= 20; line++)
			{
				make_wire("HL", Wire::Direction::EAST, {1, y}, {line, y}, *unknownIndex.next(), *trackIndex.next());
				make_wire("HL", Wire::Direction::WEST, {line, y}, {1, y}, *unknownIndex.next(), *trackIndex.next());
			}

			for (uint32_t line = 1; line <= firstwire; line++)
			{
				make_wire("HL", Wire::Direction::EAST, {1, y}, {line, y}, *unknownIndex.next(), *trackIndex.next());
				make_wire("HL", Wire::Direction::WEST, {line, y}, {1, y}, *unknownIndex.next(), *trackIndex.next());
			}

			for (int i = 0; i < y + 1; i++)
			{
				unknownIndex.next();
				unknownIndex.next();
			}

			for (uint32_t line = 21; line <= dev.dims.x - 1; line++)
			{
				make_wire("HL", Wire::Direction::EAST, {line - 19, y}, {line, y}, *unknownIndex.next(), 10);
				make_wire("HL", Wire::Direction::WEST, {line, y}, {line - 19, y}, *unknownIndex.next(), 11);
			}

			for (uint32_t line = dev.dims.x - 19; line <= dev.dims.x - 1; line++)
			{
				make_wire("HL", Wire::Direction::EAST, {line, y}, {dev.dims.x - 1, y}, *unknownIndex.next(), 10);
				make_wire("HL", Wire::Direction::WEST, {dev.dims.x - 1, y}, {line, y}, *unknownIndex.next(), 11);
			}

			unknownIndex.track = 8;
			trackIndex.track   = 8;
			trackIndex.next((48 - 2 * (y % 20)));

			for (uint32_t i = 0; i < 20; i++)
			{
				make_wire("HL",
				          Wire::Direction::EAST,
				          {dev.dims.x, y},
				          {dev.dims.x, y},
				          *unknownIndex.next(),
				          *trackIndex.next());
				make_wire("HL",
				          Wire::Direction::WEST,
				          {dev.dims.x, y},
				          {dev.dims.x, y},
				          *unknownIndex.next(),
				          *trackIndex.next());
			}
		}
	}

	void RoutingGenerator::generate_vert_lwires()
	{
		ltrack_index unknownIndex{8};
		for (uint32_t x = 0; x < dev.dims.x; x++)
		{
			ltrack_index trackIndex{8};
			trackIndex.next(-2 * x + 2);
			uint32_t firstwire = 21 - (x % 20);

			for (uint32_t line = firstwire; line <= 20; line++)
			{
				make_wire("VL", Wire::Direction::NORTH, {x, 1}, {x, line}, *unknownIndex.next(), *trackIndex.next());
				make_wire("VL", Wire::Direction::SOUTH, {x, line}, {x, 1}, *unknownIndex.next(), *trackIndex.next());
			}

			for (uint32_t line = 1; line <= firstwire; line++)
			{
				make_wire("VL", Wire::Direction::NORTH, {x, 1}, {x, line}, *unknownIndex.next(), *trackIndex.next());
				make_wire("VL", Wire::Direction::SOUTH, {x, line}, {x, 1}, *unknownIndex.next(), *trackIndex.next());
			}

			for (int i = 0; i < x; i++)
			{
				unknownIndex.next();
				unknownIndex.next();
			}

			for (uint32_t line = 21; line <= dev.dims.y - 1; line++)
			{
				make_wire("VL", Wire::Direction::NORTH, {x, line - 19}, {x, line}, *unknownIndex.next(), 8);
				make_wire("VL", Wire::Direction::SOUTH, {x, line}, {x, line - 19}, *unknownIndex.next(), 9);
			}

			for (uint32_t line = dev.dims.y - 19; line <= dev.dims.y - 1; line++)
			{
				make_wire("VL", Wire::Direction::NORTH, {x, line}, {x, dev.dims.y - 1}, *unknownIndex.next(), 8);
				make_wire("VL", Wire::Direction::SOUTH, {x, dev.dims.y - 1}, {x, line}, *unknownIndex.next(), 9);
			}

			unknownIndex.track = 8;
			trackIndex.track   = 8;
			trackIndex.next(48 - 2 * ((x + 2) % 20));

			for (uint32_t i = 0; i < 20; i++)
			{
				make_wire("VL", Wire::Direction::NORTH, {x, 0}, {x, 0}, *unknownIndex.next(), *trackIndex.next());
				make_wire("VL", Wire::Direction::SOUTH, {x, 0}, {x, 0}, *unknownIndex.next(), *trackIndex.next());
			}
		}
	}

	void RoutingGenerator::generate_horz_swires()
	{
		strack_index unknownIndex{0};
		for (uint32_t y = 0; y < dev.dims.y - 1; y++)
		{
			strack_index trackIndex{0};
			trackIndex.next(-2 * y + 2);
			uint32_t firstwire = 4 - (y % 4);

			for (uint32_t line = firstwire; line <= 4; line++)
			{
				make_wire("HS", Wire::Direction::EAST, {1, y}, {line, y}, *unknownIndex.next(), *trackIndex.next());
				make_wire("HS", Wire::Direction::WEST, {line, y}, {1, y}, *unknownIndex.next(), *trackIndex.next());
			}

			for (uint32_t line = 1; line <= firstwire; line++)
			{
				make_wire("HS", Wire::Direction::EAST, {1, y}, {line, y}, *unknownIndex.next(), *trackIndex.next());
				make_wire("HS", Wire::Direction::WEST, {line, y}, {1, y}, *unknownIndex.next(), *trackIndex.next());
			}

			for (int i = 0; i < y + 1; i++)
			{
				unknownIndex.next();
				unknownIndex.next();
			}

			for (uint32_t line = 5; line <= dev.dims.x - 1; line++)
			{
				make_wire("HS", Wire::Direction::EAST, {line - 3, y}, {line, y}, *unknownIndex.next(), 2);
				make_wire("HS", Wire::Direction::WEST, {line, y}, {line - 3, y}, *unknownIndex.next(), 3);
			}

			for (uint32_t line = dev.dims.x - 3; line <= dev.dims.x - 1; line++)
			{
				make_wire("HS", Wire::Direction::EAST, {line, y}, {dev.dims.x - 1, y}, *unknownIndex.next(), 2);
				make_wire("HS", Wire::Direction::WEST, {dev.dims.x - 1, y}, {line, y}, *unknownIndex.next(), 3);
			}

			unknownIndex.track = 0;
			trackIndex.track   = 0;
			trackIndex.next(8 - 2 * (y % 4));

			for (uint32_t i = 0; i < 4; i++)
			{
				make_wire("HS",
				          Wire::Direction::EAST,
				          {dev.dims.x, y},
				          {dev.dims.x, y},
				          *unknownIndex.next(),
				          *trackIndex.next());
				make_wire("HS",
				          Wire::Direction::WEST,
				          {dev.dims.x, y},
				          {dev.dims.x, y},
				          *unknownIndex.next(),
				          *trackIndex.next());
			}
		}
	}

	void RoutingGenerator::generate_vert_swires()
	{
		strack_index unknownIndex{0};
		for (uint32_t x = 0; x < dev.dims.x; x++)
		{
			strack_index trackIndex{0};
			trackIndex.next(-2 * x + 2);
			uint32_t firstwire = 5 - (x % 4);

			for (uint32_t line = firstwire; line <= 4; line++)
			{
				make_wire("VS", Wire::Direction::NORTH, {x, 1}, {x, line}, *unknownIndex.next(), *trackIndex.next());
				make_wire("VS", Wire::Direction::SOUTH, {x, line}, {x, 1}, *unknownIndex.next(), *trackIndex.next());
			}

			for (uint32_t line = 1; line <= firstwire; line++)
			{
				make_wire("VS", Wire::Direction::NORTH, {x, 1}, {x, line}, *unknownIndex.next(), *trackIndex.next());
				make_wire("VS", Wire::Direction::SOUTH, {x, line}, {x, 1}, *unknownIndex.next(), *trackIndex.next());
			}

			for (int i = 0; i < x; i++)
			{
				unknownIndex.next();
				unknownIndex.next();
			}

			for (uint32_t line = 21; line <= dev.dims.y - 1; line++)
			{
				make_wire("VS", Wire::Direction::NORTH, {x, line - 3}, {x, line}, *unknownIndex.next(), 0);
				make_wire("VS", Wire::Direction::SOUTH, {x, line}, {x, line - 3}, *unknownIndex.next(), 1);
			}

			for (uint32_t line = dev.dims.y - 3; line <= dev.dims.y - 1; line++)
			{
				make_wire("VS", Wire::Direction::NORTH, {x, line}, {x, dev.dims.y - 1}, *unknownIndex.next(), 0);
				make_wire("VS", Wire::Direction::SOUTH, {x, dev.dims.y - 1}, {x, line}, *unknownIndex.next(), 1);
			}

			unknownIndex.track = 0;
			trackIndex.track   = 0;
			trackIndex.next(8 - 2 * ((x + 2) % 4));

			for (uint32_t i = 0; i < 20; i++)
			{
				make_wire("VS", Wire::Direction::NORTH, {x, 0}, {x, 0}, *unknownIndex.next(), *trackIndex.next());
				make_wire("VS", Wire::Direction::SOUTH, {x, 0}, {x, 0}, *unknownIndex.next(), *trackIndex.next());
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
	    const data::string &type, Wire::Direction dir, vec2 start, vec2 end, uint32_t unknown, uint32_t track)
	{
		auto &wire   = *dev.wires.emplace_back(data::make_unique<Wire>(Wire{}));
		wire.start   = start;
		wire.end     = end;
		wire.unknown = unknown;
		wire.track   = track;
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

		wire.name.push_back(dev.id(fmt::format("{}", wire.unknown)));
		wire.name.push_back(dev.id(fmt::format("{}", wire.track)));
		wire.type = Wire::WireType::LOCAL;
		return wire;
	}
}
