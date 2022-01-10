//
// Created by nxmq0 on 1/3/2022.
//

#include "RoutingGenerator.h"

#include <fmt/core.h>
#include <iostream>

namespace tritium
{
	using WD = Wire::Direction;
	using WT = Wire::Type;

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

			std::array<Wire *, 40> frontWires{};

			for (uint32_t line = firstwire; line <= 20; line++)
			{
				auto &e{make_wire(WT::LONG, WD::EAST, {1, y}, {line, y}, trackIndex.next(), switchIndex.next())};
				frontWires[line + 19] = &e;
				auto &w{make_wire(WT::LONG, WD::WEST, {line, y}, {1, y}, trackIndex.next(), switchIndex.next())};
				frontWires[line - 1] = &w;
			}

			for (uint32_t line = 1; line < firstwire; line++)
			{
				auto &e{make_wire(WT::LONG, WD::EAST, {1, y}, {line, y}, trackIndex.next(), switchIndex.next())};
				frontWires[line + 19] = &e;
				auto &w{make_wire(WT::LONG, WD::WEST, {line, y}, {1, y}, trackIndex.next(), switchIndex.next())};
				frontWires[line - 1] = &w;
			}

			for (int i = 0; i < 20; i++)
			{
				make_alias(*frontWires[i], *frontWires[39-i]);
			}

			for (int i = 0; i < y + 1; i++)
			{
				trackIndex.next();
				trackIndex.next();
			}


			for (uint32_t line = 21; line < dev.dims.x; line++)
			{
				make_wire(WT::LONG, WD::EAST, {line - 19, y}, {line, y}, trackIndex.next(), 10);
				make_wire(WT::LONG, WD::WEST, {line, y}, {line - 19, y}, trackIndex.next(), 11);
			}

			std::array<Wire *, 38> rearWires{};
			std::array<Wire *, 40> endWires{};


			for (uint32_t line = dev.dims.x - 19; line < dev.dims.x; line++)
			{
				auto &e{make_wire(WT::LONG, WD::EAST, {line, y}, {dev.dims.x - 1, y}, trackIndex.next(), 10)};
				rearWires[line-59] = &e;
				auto &w{make_wire(WT::LONG, WD::WEST, {dev.dims.x - 1, y}, {line, y}, trackIndex.next(), 11)};
				rearWires[line-40] = &w;
			}

			trackIndex.track  = 8;
			switchIndex.track = 8;
			switchIndex.next((48 - 2 * (y % 20)));

			for (uint32_t i = 0; i < 20; i++)
			{
				auto &e{make_wire(WT::LONG, WD::EAST, {dev.dims.x, y}, {dev.dims.x, y}, trackIndex.next(), switchIndex.next())};
				endWires[e.track-8] = &e;
				auto &w{make_wire(WT::LONG, WD::WEST, {dev.dims.x, y}, {dev.dims.x, y}, trackIndex.next(), switchIndex.next())};
				endWires[w.track-8] = &w;
			}

			for(auto wire : rearWires) {
				if((wire->track & 1) == 0)
					make_alias(*wire,*endWires[wire->track-8]);
				else
					make_alias(*endWires[wire->track-8],*wire);
			}

			ltrack_index aliasIdx{39};
			aliasIdx.next(4*y);
			for(int i = 0; i < 40; i+=2) {
                make_alias(*endWires[i],*endWires[aliasIdx.next(-2)-8]);
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

			std::array<Wire *,40> frontWires{};
			std::array<Wire *,40> frontWiresByTrack{};


			for (uint32_t line = firstwire; line <= 20; line++)
			{
				auto &n{make_wire(WT::LONG, WD::NORTH, {x, 1}, {x, line}, trackIndex.next(), switchIndex.next())};
				frontWires[line-1] = &n;
				frontWiresByTrack[n.track-8] = &n;
				auto &s{make_wire(WT::LONG, WD::SOUTH, {x, line}, {x, 1}, trackIndex.next(), switchIndex.next())};
				frontWires[line+19] = &s;
				frontWiresByTrack[s.track-8] = &s;
			}

			for (uint32_t line = 1; line < firstwire; line++)
			{
				auto &n{make_wire(WT::LONG, WD::NORTH, {x, 1}, {x, line}, trackIndex.next(), switchIndex.next())};
				frontWires[line-1] = &n;
				frontWiresByTrack[n.track-8] = &n;
				auto &s{make_wire(WT::LONG, WD::SOUTH, {x, line}, {x, 1}, trackIndex.next(), switchIndex.next())};
				frontWires[line+19] = &s;
				frontWiresByTrack[s.track-8] = &s;
			}

			for (int i = 0; i < x; i++)
			{
				trackIndex.next();
				trackIndex.next();
			}

			for (uint32_t line = 21; line < dev.dims.y-1; line++)
			{
				make_wire(WT::LONG, WD::NORTH, {x, line - 19}, {x, line}, trackIndex.next(), 8);
				make_wire(WT::LONG, WD::SOUTH, {x, line}, {x, line - 19}, trackIndex.next(), 9);
			}

			std::array<Wire*, 40> rearWires{};

			for (uint32_t line = dev.dims.y - 20; line < dev.dims.y; line++)
			{
				auto &n{make_wire(WT::LONG, WD::NORTH, {x, line}, {x, dev.dims.y - 1}, trackIndex.next(), 8)};
				rearWires[line-123] = &n;
				auto &s{make_wire(WT::LONG, WD::SOUTH, {x, dev.dims.y - 1}, {x, line}, trackIndex.next(), 9)};
				rearWires[line-143] = &s;
			}

			for(int i = 0; i < 20; i++) {
				make_alias(*rearWires[39-i],*rearWires[i]);
			}


			trackIndex.track  = 8;
			switchIndex.track = 8;
			switchIndex.next(48 - 2 * ((x + 2) % 20));

	        std::array<Wire *, 40> startWires{};

			for (uint32_t i = 0; i < 20; i++)
			{
				auto &n{make_wire(WT::LONG, WD::NORTH, {x, 0}, {x, 0}, trackIndex.next(), switchIndex.next())};
				startWires[n.track-8] = &n;
				auto &s{make_wire(WT::LONG, WD::SOUTH, {x, 0}, {x, 0}, trackIndex.next(), switchIndex.next())};
				startWires[s.track-8] = &s;
			}

			for(auto wire : frontWires) {
				if(wire->end.y == 20 || wire->start.y == 20) continue;
				if((wire->track & 1) == 1)
					make_alias(*wire,*startWires[wire->track-8]);
				else
					make_alias(*startWires[wire->track-8],*wire);
			}
			ltrack_index aliasIdx{42};
			aliasIdx.next(4*x);
			for(int i = 1; i < 40; i+=2) {
				make_alias(*startWires[i],*startWires[aliasIdx.next(-2)-8]);
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

			std::array<Wire *, 8> frontWires{};

			for (uint32_t line = firstwire; line <= 4; line++)
			{
				auto &e{make_wire(WT::SHORT, WD::EAST, {1, y}, {line, y}, trackIndex.next(), switchIndex.next())};
				frontWires[e.sbi] = &e;
				auto &w{make_wire(WT::SHORT, WD::WEST, {line, y}, {1, y}, trackIndex.next(), switchIndex.next())};
				frontWires[w.sbi] = &w;
			}

			for (uint32_t line = 1; line < firstwire; line++)
			{
				auto &e{make_wire(WT::SHORT, WD::EAST, {1, y}, {line, y}, trackIndex.next(), switchIndex.next())};
				frontWires[e.sbi] = &e;
				auto &w{make_wire(WT::SHORT, WD::WEST, {line, y}, {1, y}, trackIndex.next(), switchIndex.next())};
				frontWires[w.sbi] = &w;
			}

			for (int i = 1; i < 8; i += 2)
			{
				make_alias(*frontWires[i], *frontWires[7 - i]);
			}

			for (int i = 0; i < y + 1; i++)
			{
				trackIndex.next();
				trackIndex.next();
			}

			for (uint32_t line = 5; line <= dev.dims.x - 1; line++)
			{
				make_wire(WT::SHORT, WD::EAST, {line - 3, y}, {line, y}, trackIndex.next(), 2);
				make_wire(WT::SHORT, WD::WEST, {line, y}, {line - 3, y}, trackIndex.next(), 3);
			}

			std::array<Wire *, 8> rearWires{};
			std::array<Wire *, 8> endWiresByTrack{};
			std::array<Wire *, 8> endWiresBySBI{};

			for (uint32_t line = dev.dims.x - 3; line <= dev.dims.x - 1; line++)
			{
				auto &e{make_wire(WT::SHORT, WD::EAST, {line, y}, {dev.dims.x - 1, y}, trackIndex.next(), 2)};
				rearWires[e.track] = &e;
				auto &w{make_wire(WT::SHORT, WD::WEST, {dev.dims.x - 1, y}, {line, y}, trackIndex.next(), 3)};
				rearWires[w.track] = &w;
			}

			trackIndex.track  = 0;
			switchIndex.track = 0;
			switchIndex.next(8 - 2 * (y % 4));

			for (uint32_t i = 0; i < 4; i++)
			{
				auto &e{make_wire(
				    WT::SHORT, WD::EAST, {dev.dims.x, y}, {dev.dims.x, y}, trackIndex.next(), switchIndex.next())};
				endWiresBySBI[e.sbi]     = &e;
				endWiresByTrack[e.track] = &e;
				auto &w{make_wire(
				    WT::SHORT, WD::WEST, {dev.dims.x, y}, {dev.dims.x, y}, trackIndex.next(), switchIndex.next())};
				endWiresBySBI[w.sbi]     = &w;
				endWiresByTrack[w.track] = &w;
			}

			for (auto wire : rearWires)
			{
				if (wire == nullptr) continue;
				if (wire->dir == WD::EAST)
				{
					make_alias(*wire, *endWiresByTrack[wire->track]);
				}
				else
				{
					make_alias(*endWiresByTrack[wire->track], *wire);
				}
			}

			if ((y & 1) == 0)
			{
				make_alias(*endWiresByTrack[0], *endWiresByTrack[7]);
				make_alias(*endWiresByTrack[2], *endWiresByTrack[5]);
				make_alias(*endWiresByTrack[4], *endWiresByTrack[3]);
				make_alias(*endWiresByTrack[6], *endWiresByTrack[1]);
			}
			else
			{
				make_alias(*endWiresByTrack[0], *endWiresByTrack[3]);
				make_alias(*endWiresByTrack[2], *endWiresByTrack[1]);
				make_alias(*endWiresByTrack[4], *endWiresByTrack[7]);
				make_alias(*endWiresByTrack[6], *endWiresByTrack[5]);
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

			std::array<Wire *, 8> frontWires{};

			for (uint32_t line = firstwire; line <= 4; line++)
			{
				auto &n{make_wire(WT::SHORT, WD::NORTH, {x, 1}, {x, line}, trackIndex.next(), switchIndex.next())};
				frontWires[n.track] = &n;
				auto &s{make_wire(WT::SHORT, WD::SOUTH, {x, line}, {x, 1}, trackIndex.next(), switchIndex.next())};
				frontWires[s.track] = &s;
			}

			for (uint32_t line = 1; line < firstwire; line++)
			{
				auto &n{make_wire(WT::SHORT, WD::NORTH, {x, 1}, {x, line}, trackIndex.next(), switchIndex.next())};
				frontWires[n.track] = &n;
				auto &s{make_wire(WT::SHORT, WD::SOUTH, {x, line}, {x, 1}, trackIndex.next(), switchIndex.next())};
				frontWires[s.track] = &s;
			}

			for (int i = 0; i < x; i++)
			{
				trackIndex.next();
				trackIndex.next();
			}

			for (uint32_t line = 5; line < dev.dims.y - 1; line++)
			{
				make_wire(WT::SHORT, WD::NORTH, {x, line - 3}, {x, line}, trackIndex.next(), 0);
				make_wire(WT::SHORT, WD::SOUTH, {x, line}, {x, line - 3}, trackIndex.next(), 1);
			}

			std::array<Wire *, 8> rearTracks{};
			for (uint32_t line = dev.dims.y - 4; line < dev.dims.y; line++)
			{
				auto &n{make_wire(WT::SHORT, WD::NORTH, {x, line}, {x, dev.dims.y - 1}, trackIndex.next(), 0)};
				rearTracks[n.start.y - 159] = &n; // rearTracks[0]..rearTracks[3]
				auto &s{make_wire(WT::SHORT, WD::SOUTH, {x, dev.dims.y - 1}, {x, line}, trackIndex.next(), 1)};
				rearTracks[s.end.y - 155] = &s; // rearTracks[4]..rearTracks[7]
			}

			make_alias(*rearTracks[0], *rearTracks[7]);
			make_alias(*rearTracks[1], *rearTracks[6]);
			make_alias(*rearTracks[2], *rearTracks[5]);
			make_alias(*rearTracks[3], *rearTracks[4]);

			trackIndex.track  = 0;
			switchIndex.track = 0;
			switchIndex.next(8 - 2 * ((x + 2) % 4));

			std::array<Wire *, 8> startWires{};

			for (uint32_t i = 0; i < 4; i++)
			{
				auto &n{make_wire(WT::SHORT, WD::NORTH, {x, 0}, {x, 0}, trackIndex.next(), switchIndex.next())};
				startWires[n.track] = &n;
				auto &s{make_wire(WT::SHORT, WD::SOUTH, {x, 0}, {x, 0}, trackIndex.next(), switchIndex.next())};
				startWires[s.track] = &s;
			}

			for (auto wire : frontWires)
			{
				if (wire->start.y == 4 || wire->end.y == 4) continue;
				if (wire->dir == WD::SOUTH)
				{
					make_alias(*wire, *startWires[wire->track]);
				}
				else
				{
					make_alias(*startWires[wire->track], *wire);
				}
			}

			if ((x & 1) == 0)
			{
				make_alias(*startWires[1], *startWires[2]);
				make_alias(*startWires[3], *startWires[0]);
				make_alias(*startWires[5], *startWires[6]);
				make_alias(*startWires[7], *startWires[4]);
			}
			else
			{
				make_alias(*startWires[1], *startWires[6]);
				make_alias(*startWires[3], *startWires[4]);
				make_alias(*startWires[5], *startWires[2]);
				make_alias(*startWires[7], *startWires[0]);
			}
		}
	}

	void RoutingGenerator::generate_semicol_clks()
	{
		for (uint32_t x = 0; x < dev.dims.x; x++)
		{
			// TODO: fix sizes to be generic
			make_wire(WT::SEMICOL, WD::NORTH, {x, 81}, {x, 160}, 49, 49);
			make_wire(WT::SEMICOL, WD::SOUTH, {x, 80}, {x, 1}, 49, 49);
			make_wire(WT::SEMICOL, WD::NORTH, {x, 81}, {x, 160}, 50, 50);
			make_wire(WT::SEMICOL, WD::SOUTH, {x, 80}, {x, 1}, 50, 50);
			make_wire(WT::SEMICOL, WD::NORTH, {x, 81}, {x, 160}, 51, 51);
			make_wire(WT::SEMICOL, WD::SOUTH, {x, 80}, {x, 1}, 51, 51);
			make_wire(WT::SEMICOL, WD::NORTH, {x, 81}, {x, 160}, 52, 52);
			make_wire(WT::SEMICOL, WD::SOUTH, {x, 80}, {x, 1}, 52, 52);
		}
	}

	void RoutingGenerator::generate_global_clks() {}

	Wire &RoutingGenerator::make_wire(
	    Wire::Type type, Wire::Direction dir, vec2 start, vec2 end, uint32_t track, uint32_t sbi)
	{
		auto &wire{*dev.wires.emplace_back(data::make_unique<Wire>(Wire{}))};
		wire.start = start;
		wire.end   = end;
		wire.sbi   = sbi;
		wire.track = track;
		wire.dir   = dir;
		wire.type  = type;
		//TODO: Figure out timing constant interaction so we can use `*_RMETAL` and `*_CMETAL`.
//		wire.R = start.dist(end);
//		wire.C = start.dist(end);
		switch (type)
		{
			case WT::INTERNAL: wire.name.push_back(dev.id("INT")); break;
			case WT::INTERBEL: wire.name.push_back(dev.id("IB")); break;
			case WT::SHORT:
				if (wire.isHorizontal())
					wire.name.push_back(dev.id("HS"));
				else
					wire.name.push_back(dev.id("VS"));
				break;
			case WT::LONG:
				if (wire.isHorizontal())
					wire.name.push_back(dev.id("HL"));
				else
					wire.name.push_back(dev.id("VL"));
				break;
			case WT::SEMICOL: wire.name.push_back(dev.id("CC")); break;
			case WT::GLOBAL: wire.name.push_back(dev.id("GL")); break;
			default: std::cerr << "Invalid wiretype!!!!!!\n"; std::terminate();
		}

		switch (dir)
		{
			case WD::NORTH:
				wire.name.push_back(dev.id(fmt::format("X{}", start.x)));
				wire.name.push_back(dev.id("N"));
				wire.name.push_back(dev.id(fmt::format("Y[{}:{}]", start.y, end.y)));
				break;

			case WD::SOUTH:
				wire.name.push_back(dev.id(fmt::format("X{}", start.x)));
				wire.name.push_back(dev.id("S"));
				wire.name.push_back(dev.id(fmt::format("Y[{}:{}]", start.y, end.y)));
				break;

			case WD::EAST:
				wire.name.push_back(dev.id(fmt::format("Y{}", start.y)));
				wire.name.push_back(dev.id("E"));
				wire.name.push_back(dev.id(fmt::format("X[{}:{}]", start.x, end.x)));
				break;

			case WD::WEST:
				wire.name.push_back(dev.id(fmt::format("Y{}", start.y)));
				wire.name.push_back(dev.id("W"));
				wire.name.push_back(dev.id(fmt::format("X[{}:{}]", start.x, end.x)));
				break;
		}

		wire.name.push_back(dev.id(fmt::format("{}", wire.track)));
		wire.name.push_back(dev.id(fmt::format("{}", wire.sbi)));
		return wire;
	}

	tritium::Pip &tritium::RoutingGenerator::make_alias(Wire &a, Wire &b)
	{
		vec2 loc{a.end};
		//		std::cout << fmt::format("Aliasing wires {} and {}.\n", a.getName(dev), b.getName(dev));
		//		std::cout << fmt::format("Wire A ptrack: {}\n",a.getPTrackAt(a.end));
		//		std::cout << fmt::format("Wire B ptrack: {}\n",b.getPTrackAt(b.start));
		auto &pip{*dev.pips.emplace_back(data::make_unique<Pip>(Pip{}))};
		dev.pips[0];
		pip.name.push_back(dev.id("ALIAS"));
		pip.name.push_back(dev.id(fmt::format("X{}Y{}", loc.x, loc.y)));
		pip.name.push_back(dev.id(fmt::format("{}{}", a.isHorizontal() ? "X" : "Y", a.track)));
		pip.name.push_back(dev.id(fmt::format("{}{}", b.isHorizontal() ? "X" : "Y", b.track)));
		pip.inputs.emplace_back(data::ptr<Wire>{&a});
		pip.outputs.emplace_back(data::ptr<Wire>{&b});
		pip.loc     = loc;
		pip.isAlias = true;
		return pip;
	}
}
