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
		ltrack_index index{8};
		for (uint32_t y = 0; y < dev.dims.y - 1; y++)
		{
			ltrack_index pairIndex{8};
			pairIndex.next(-2 * y + 2);

			uint32_t firstwire = 20 - (y % 20);

			for (uint32_t line = firstwire; line <= 20; line++)
			{
				make_wire("HL",
				          Wire::Direction::EAST,
				          {1, y},
				          {line, y},
				          *index.next(),
				          *pairIndex.next());

				make_wire("HL",
				          Wire::Direction::WEST,
				          {line, y},
				          {1, y},
				          *index.next(),
				          *pairIndex.next());
			}

			for (uint32_t line = 1; line <= firstwire; line++)
			{
				make_wire("HL",
				          Wire::Direction::EAST,
				          {1, y},
				          {line, y},
				          *index.next(),
				          *pairIndex.next());

				make_wire("HL",
				          Wire::Direction::WEST,
				          {line, y},
				          {1, y},
				          *index.next(),
				          *pairIndex.next());
			}

			for (int i = 0; i < y + 1; i++)
			{
				index.next();
				index.next();
			}

			for (uint32_t line = 21; line <= dev.dims.x - 1; line++)
			{
				make_wire("HL",
				          Wire::Direction::EAST,
				          {line - 19, y},
				          {line, y},
				          *index.next(),
				          10);

				make_wire("HL",
				          Wire::Direction::WEST,
				          {line, y},
				          {line - 19, y},
				          *index.next(),
				          11);
			}

			for (uint32_t line = dev.dims.x - 19; line <= dev.dims.x - 1; line++)
			{
				make_wire("HL",
				          Wire::Direction::EAST,
				          {line, y},
				          {dev.dims.x - 1, y},
				          *index.next(),
				          10);

				make_wire("HL",
				          Wire::Direction::WEST,
				          {dev.dims.x - 1, y},
				          {line, y},
				          *index.next(),
				          11);
			}

			index.track     = 8;
			pairIndex.track = 8;
			pairIndex.next((48 - 2 * (y % 20)));

			for (uint32_t i = 0; i < 20; i++)
			{
				make_wire("HL",
				          Wire::Direction::EAST,
				          {dev.dims.x, y},
				          {dev.dims.x, y},
				          *index.next(),
				          *pairIndex.next());

				make_wire("HL",
				          Wire::Direction::WEST,
				          {dev.dims.x, y},
				          {dev.dims.x, y},
				          *index.next(),
				          *pairIndex.next());
			}
		}
	}

	void RoutingGenerator::generate_vert_lwires()
	{
		ltrack_index index{8};
		for (uint32_t x = 0; x < dev.dims.x; x++)
		{
			ltrack_index pairIndex{8};
			pairIndex.next(-2 * x + 2);
			uint32_t firstwire = 21 - (x % 20);

			for (uint32_t line = firstwire; line <= 20; line++)
			{
				make_wire("VL",
				          Wire::Direction::NORTH,
				          {x, 1},
				          {x, line},
				          *index.next(),
				          *pairIndex.next());

				make_wire("VL",
				          Wire::Direction::SOUTH,
				          {x, line},
				          {x, 1},
				          *index.next(),
				          *pairIndex.next());
			}

			for (uint32_t line = 1; line <= firstwire; line++)
			{
				make_wire("VL",
				          Wire::Direction::NORTH,
				          {x, 1},
				          {x, line},
				          *index.next(),
				          *pairIndex.next());

				make_wire("VL",
				          Wire::Direction::SOUTH,
				          {x, line},
				          {x, 1},
				          *index.next(),
				          *pairIndex.next());
			}

			for (int i = 0; i < x; i++)
			{
				index.next();
				index.next();
			}

			for (uint32_t line = 21; line <= dev.dims.y - 1; line++)
			{
				make_wire("VL",
				          Wire::Direction::NORTH,
				          {x, line - 19},
				          {x, line},
				          *index.next(),
				          8);

				make_wire("VL",
				          Wire::Direction::SOUTH,
				          {x, line},
				          {x, line - 19},
				          *index.next(),
				          9);
			}

			for (uint32_t line = dev.dims.y - 19; line <= dev.dims.y - 1; line++)
			{
				make_wire("VL",
				          Wire::Direction::NORTH,
				          {x, line},
				          {x, dev.dims.y - 1},
				          *index.next(),
				          8);

				make_wire("VL",
				          Wire::Direction::SOUTH,
				          {x, dev.dims.y - 1},
				          {x, line},
				          *index.next(),
				          9);
			}

			index.track     = 8;
			pairIndex.track = 8;
			pairIndex.next(48 - 2 * ((x + 2) % 20));

			for (uint32_t i = 0; i < 20; i++)
			{
				make_wire("VL",
				          Wire::Direction::NORTH,
				          {x, 0},
				          {x, 0},
				          *index.next(),
				          *pairIndex.next());

				make_wire("VL",
				          Wire::Direction::SOUTH,
				          {x, 0},
				          {x, 0},
				          *index.next(),
				          *pairIndex.next());
			}
		}
	}

	void RoutingGenerator::generate_horz_swires()
	{
		strack_index index{0};
		for (uint32_t y = 0; y < dev.dims.y - 1; y++)
		{
			strack_index pairIndex{0};
			pairIndex.next(-2 * y + 2);
			uint32_t firstwire = 4 - (y % 4);

			for (uint32_t line = firstwire; line <= 4; line++)
			{
				make_wire("HS",
				          Wire::Direction::EAST,
				          {1, y},
				          {line, y},
				          *index.next(),
				          *pairIndex.next());

				make_wire("HS",
				          Wire::Direction::WEST,
				          {line, y},
				          {1, y},
				          *index.next(),
				          *pairIndex.next());
			}

			for (uint32_t line = 1; line <= firstwire; line++)
			{
				make_wire("HS",
				          Wire::Direction::EAST,
				          {1, y},
				          {line, y},
				          *index.next(),
				          *pairIndex.next());

				make_wire("HS",
				          Wire::Direction::WEST,
				          {line, y},
				          {1, y},
				          *index.next(),
				          *pairIndex.next());
			}

			for (int i = 0; i < y + 1; i++)
			{
				index.next();
				index.next();
			}

			for (uint32_t line = 5; line <= dev.dims.x - 1; line++)
			{
				make_wire("HS",
				          Wire::Direction::EAST,
				          {line - 3, y},
				          {line, y},
				          *index.next(),
				          2);

				make_wire("HS",
				          Wire::Direction::WEST,
				          {line, y},
				          {line - 3, y},
				          *index.next(),
				          3);
			}

			for (uint32_t line = dev.dims.x - 3; line <= dev.dims.x - 1; line++)
			{
				make_wire("HS",
				          Wire::Direction::EAST,
				          {line, y},
				          {dev.dims.x - 1, y},
				          *index.next(),
				          2);

				make_wire("HS",
				          Wire::Direction::WEST,
				          {dev.dims.x - 1, y},
				          {line, y},
				          *index.next(),
				          3);
			}

			index.track     = 0;
			pairIndex.track = 0;
			pairIndex.next(8 - 2 * (y % 4));

			for (uint32_t i = 0; i < 4; i++)
			{
				make_wire("HS",
				          Wire::Direction::EAST,
				          {dev.dims.x, y},
				          {dev.dims.x, y},
				          *index.next(),
				          *pairIndex.next());

				make_wire("HS",
				          Wire::Direction::WEST,
				          {dev.dims.x, y},
				          {dev.dims.x, y},
				          *index.next(),
				          *pairIndex.next());
			}
		}
	}

	void RoutingGenerator::generate_vert_swires()
	{
		strack_index index{0};
		for (uint32_t x = 0; x < dev.dims.x; x++)
		{
			strack_index pairIndex{0};
			pairIndex.next(-2 * x + 2);
			uint32_t firstwire = 5 - (x % 4);

			for (uint32_t line = firstwire; line <= 4; line++)
			{
				make_wire("VS",
				          Wire::Direction::NORTH,
				          {x, 1},
				          {x, line},
				          *index.next(),
				          *pairIndex.next());

				make_wire("VS",
				          Wire::Direction::SOUTH,
				          {x, line},
				          {x, 1},
				          *index.next(),
				          *pairIndex.next());
			}

			for (uint32_t line = 1; line <= firstwire; line++)
			{
				make_wire("VS",
				          Wire::Direction::NORTH,
				          {x, 1},
				          {x, line},
				          *index.next(),
				          *pairIndex.next());

				make_wire("VS",
				          Wire::Direction::SOUTH,
				          {x, line},
				          {x, 1},
				          *index.next(),
				          *pairIndex.next());
			}

			for (int i = 0; i < x; i++)
			{
				index.next();
				index.next();
			}

			for (uint32_t line = 21; line <= dev.dims.y - 1; line++)
			{
				make_wire("VS",
				          Wire::Direction::NORTH,
				          {x, line - 3},
				          {x, line},
				          *index.next(),
				          0);

				make_wire("VS",
				          Wire::Direction::SOUTH,
				          {x, line},
				          {x, line - 3},
				          *index.next(),
				          1);
			}

			for (uint32_t line = dev.dims.y - 3; line <= dev.dims.y - 1; line++)
			{
				make_wire("VS",
				          Wire::Direction::NORTH,
				          {x, line},
				          {x, dev.dims.y - 1},
				          *index.next(),
				          0);

				make_wire("VS",
				          Wire::Direction::SOUTH,
				          {x, dev.dims.y - 1},
				          {x, line},
				          *index.next(),
				          1);
			}

			index.track     = 0;
			pairIndex.track = 0;
			pairIndex.next(8 - 2 * ((x + 2) % 4));

			for (uint32_t i = 0; i < 20; i++)
			{
				make_wire("VS",
				          Wire::Direction::NORTH,
				          {x, 0},
				          {x, 0},
				          *index.next(),
				          *pairIndex.next());

				make_wire("VS",
				          Wire::Direction::SOUTH,
				          {x, 0},
				          {x, 0},
				          *index.next(),
				          *pairIndex.next());
			}
		}
	}

	void RoutingGenerator::generate_semicol_clks()
	{
		for (uint32_t x = 0; x < dev.dims.x; x++)
		{
			// TODO: fix sizes to be generic
			make_wire("CC", Wire::Direction::NORTH, {x, 81}, {x, 160}, 50, 50);
			make_wire("CC", Wire::Direction::SOUTH, {x, 80}, {x, 1}, 50, 50);
			make_wire("CC", Wire::Direction::NORTH, {x, 81}, {x, 160}, 51, 51);
			make_wire("CC", Wire::Direction::SOUTH, {x, 80}, {x, 1}, 51, 51);
			make_wire("CC", Wire::Direction::NORTH, {x, 81}, {x, 160}, 52, 52);
			make_wire("CC", Wire::Direction::SOUTH, {x, 80}, {x, 1}, 52, 52);
			make_wire("CC", Wire::Direction::NORTH, {x, 81}, {x, 160}, 53, 53);
			make_wire("CC", Wire::Direction::SOUTH, {x, 80}, {x, 1}, 53, 53);
		}
	}

	void RoutingGenerator::generate_global_clks() {}

	Wire &RoutingGenerator::make_wire(const data::string &type,
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
