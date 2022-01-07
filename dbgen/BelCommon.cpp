//
// Created by nxmq0 on 1/3/2022.
//

#include "BelCommon.h"

#include <fmt/core.h>

#include <iostream>
#include <span>

void tritium::BelCommon::generate()
{
	throw "Cannot generate an undifferentiated cell!";
}

tritium::Pip &tritium::BelCommon::make_pip(const data::string &name, tritium::vec2 loc)
{
	auto &pip{*dev.pips.emplace_back(data::make_unique<Pip>(Pip{}))};
	pip.name.push_back(dev.id("INT"));
	pip.name.push_back(dev.id(bel.typestr()));
	pip.name.push_back(dev.id(fmt::format("X{}Y{}", loc.x, loc.y)));
	pip.name.push_back(dev.id(name));
	pip.loc = loc;
	return pip;
}

tritium::Wire &tritium::BelCommon::make_intwire(const data::string &name, tritium::vec2 loc)
{
	auto &wire{*dev.wires.emplace_back(data::make_unique<Wire>(Wire{}))};
	wire.name.push_back(dev.id("INT"));
	wire.name.push_back(dev.id(bel.typestr()));
	wire.name.push_back(dev.id(fmt::format("X{}Y{}", loc.x, loc.y)));
	wire.name.push_back(dev.id(name));
	wire.type  = Wire::WireType::INTERNAL;
	wire.start = loc;
	wire.end   = loc;
	return wire;
}

tritium::BelPin &tritium::BelCommon::make_pin(const data::string &name, BelPin::PinType type, tritium::vec2 loc)
{
	auto &bp{*bel.pins.emplace_back(data::make_unique<BelPin>(BelPin{}))};
	bp.name.push_back(dev.id(bel.typestr()));
	bp.name.push_back(dev.id(fmt::format("X{}Y{}", loc.x, loc.y)));
	bp.name.push_back(dev.id(name));
	bp.type = type;
	return bp;
}

void tritium::BelCommon::link_wire_to(tritium::Wire &wire, tritium::Pip &tgt)
{
	tgt.inputs.emplace_back(&wire);
	wire.sinks.emplace_back(data::ptr<Pip>{&tgt});
}

void tritium::BelCommon::link_wire_to(tritium::Wire &wire, tritium::BelPin &tgt)
{
	tgt.wire = &wire;
	wire.sinks.emplace_back(data::ptr<BelPin>{&tgt});
}

void tritium::BelCommon::link_to_wire(tritium::Pip &src, tritium::Wire &wire)
{
	src.outputs.emplace_back(&wire);
	wire.sources.emplace_back(data::ptr<Pip>{&src});
}

void tritium::BelCommon::link_to_wire(tritium::BelPin &src, tritium::Wire &wire)
{
	src.wire = &wire;
	wire.sources.emplace_back(data::ptr<BelPin>{&src});
}

tritium::Wire &tritium::BelCommon::out_lwire_for_dir(tritium::Wire::Direction dir, tritium::vec2 loc)
{
	auto wire_ytracks = [&](const tritium::vec2 target) -> std::span<tritium::Wire *>
	{
		if (auto wires = wbl.find(target); wires != wbl.end())
			return {wires->second.ytracks};
		else
			return {};
	};

	auto wire_xtracks = [&](const tritium::vec2 target) -> std::span<tritium::Wire *>
	{
		if (auto wires = wbl.find(target); wires != wbl.end())
			return {wires->second.xtracks};
		else
			return {};
	};

	switch (dir)
	{
		case Wire::Direction::NORTH:
			for (const vec2 target{loc}; auto wire : wire_ytracks(target))
				if (wire && wire->start == target && wire->dir == dir && wire->name.at(0) == dev.id("VL"))
					return *wire;
			std::cerr << fmt::format("Unable to find northern output lwire on bel {}!!!!!\n", bel.getName(dev));
			std::cerr << "ERR: Misformed routing!!!!\n";
			std::terminate();

		case Wire::Direction::SOUTH:
			for (const vec2 target{loc - vec2{0, 1}}; auto wire : wire_ytracks(target))
				if (wire && wire->start == target && wire->dir == dir && wire->name.at(0) == dev.id("VL"))
					return *wire;
			std::cerr << fmt::format("Unable to find southern output lwire on bel {}!!!!!\n", bel.getName(dev));
			std::cerr << "ERR: Misformed routing!!!!\n";
			std::terminate();

		case Wire::Direction::EAST:
			for (const vec2 target{loc.x + 1, loc.y - 1}; auto wire : wire_xtracks(target))
				if (wire && wire->start == target && wire->dir == dir && wire->name.at(0) == dev.id("HL"))
					return *wire;
			std::cerr << fmt::format("Unable to find eastern output lwire on bel {}!!!!!\n", bel.getName(dev));
			std::cerr << "ERR: Misformed routing!!!!\n";
			std::terminate();

		case Wire::Direction::WEST:
			for (const vec2 target{loc - vec2{0, 1}}; auto wire : wire_xtracks(target))
				if (wire && wire->start == target && wire->dir == dir && wire->name.at(0) == dev.id("HL"))
					return *wire;
			std::cerr << fmt::format("Unable to find western output lwire on bel {}!!!!!\n", bel.getName(dev));
			std::cerr << "ERR: Misformed routing!!!!\n";
			std::terminate();

		default: std::cerr << "ERR: Invalid Output Wire Request!!!!\n"; std::terminate();
	}
}

tritium::Wire &tritium::BelCommon::out_swire_for_dir(tritium::Wire::Direction dir, tritium::vec2 loc)
{
	auto wire_ytracks = [&](const tritium::vec2 target) -> std::span<tritium::Wire *>
	{
		if (auto wires = wbl.find(target); wires != wbl.end())
			return {wires->second.ytracks};
		else
			return {};
	};

	auto wire_xtracks = [&](const tritium::vec2 target) -> std::span<tritium::Wire *>
	{
		if (auto wires = wbl.find(target); wires != wbl.end())
			return {wires->second.xtracks};
		else
			return {};
	};

	switch (dir)
	{
		case Wire::Direction::NORTH:
			for (const vec2 target{loc}; auto wire : wire_ytracks(target))
				if (wire && wire->start == target && wire->dir == dir && wire->name.at(0) == dev.id("VS"))
					return *wire;
			std::cerr << fmt::format("Unable to find northern output swire on bel {}!!!!!\n", bel.getName(dev));
			std::cerr << "ERR: Misformed routing!!!!\n";
			std::terminate();

		case Wire::Direction::SOUTH:
			for (const vec2 target{loc - vec2{0, 1}}; auto wire : wire_ytracks(target))
				if (wire && wire->start == target && wire->dir == dir && wire->name.at(0) == dev.id("VS"))
					return *wire;
			std::cerr << fmt::format("Unable to find southern output swire on bel {}!!!!!\n", bel.getName(dev));
			std::cerr << "ERR: Misformed routing!!!!\n";
			std::terminate();

		case Wire::Direction::EAST:
			for (const vec2 target{loc.x + 1, loc.y - 1}; auto wire : wire_xtracks(target))
				if (wire && wire->start == target && wire->dir == dir && wire->name.at(0) == dev.id("HS"))
					return *wire;
			std::cerr << "ERR: Misformed routing!!!!\n";
			std::cerr << fmt::format("Unable to find eastern output swire on bel {}!!!!!\n", bel.getName(dev));
			std::terminate();

		case Wire::Direction::WEST:
			for (const vec2 target{loc - vec2{0, 1}}; auto wire : wire_xtracks(target))
				if (wire && wire->start == target && wire->dir == dir && wire->name.at(0) == dev.id("HS"))
					return *wire;
			std::cerr << "ERR: Misformed routing!!!!\n";
			std::cerr << fmt::format("Unable to find western output swire on bel {}!!!!!\n", bel.getName(dev));
			std::terminate();

		default: std::cerr << "ERR: Invalid Output Wire Request!!!!\n"; std::terminate();
	}
}

void tritium::BelCommon::link_wires_to_imux_by_index(tritium::Pip &imux, IMUXIndex idx)
{
	switch (idx)
	{
		case IMUXIndex::IMUXA:
		{
			GridCell xgc = wbl[imux.loc - vec2{0, 1}];
			for (auto xidx : {2, 5, 8, 16, 23, 32, 39, 47})
			{
				Wire *w = xgc.xtracks[xidx];
			}
			GridCell ygc = wbl[imux.loc];
			for (auto yidx : {4, 7, 11, 22, 33, 38})
			{
				Wire *w = ygc.ytracks[yidx];
			}
		}
		break;

		case IMUXIndex::IMUXB:
		{
			GridCell xgc = wbl[imux.loc - vec2{0, 1}];
			for (auto xidx : {4, 10, 15, 26, 29, 42, 45})
			{
				Wire *w = xgc.xtracks[xidx];
			}
			GridCell ygc = wbl[imux.loc];
			for (auto yidx : {5, 6, 8, 19, 20, 35, 36})
			{
				Wire *w = ygc.ytracks[yidx];
			}
		}
		break;

		case IMUXIndex::IMUXC:
		{
			GridCell xgc = wbl[imux.loc - vec2{0, 1}];
			for (auto xidx : {0, 7, 13, 24, 31, 40})
			{
				Wire *w = xgc.xtracks[xidx];
			}
			GridCell ygc = wbl[imux.loc];
			for (auto yidx : {1, 2, 9, 14, 25, 30, 41, 46})
			{
				Wire *w = ygc.ytracks[yidx];
			}
		}
		break;

		case IMUXIndex::IMUXD:
		{
			GridCell xgc = wbl[imux.loc - vec2{0, 1}];
			for (auto xidx : {1, 3, 6, 18, 21, 34, 37})
			{
				Wire *w = xgc.xtracks[xidx];
			}
			GridCell ygc = wbl[imux.loc];
			for (auto yidx : {3, 12, 17, 27, 28, 43, 44})
			{
				Wire *w = ygc.ytracks[yidx];
			}
		}
		break;

		case IMUXIndex::IMUXCE:
		{
			GridCell xgc = wbl[imux.loc - vec2{0, 1}];
			for (auto xidx : {10, 13})
			{
				Wire *w = xgc.xtracks[xidx];
			}
			GridCell ygc = wbl[imux.loc];
			for (auto yidx : {36, 37})
			{
				Wire *w = ygc.ytracks[yidx];
			}
		}
		break;

		case IMUXIndex::IMUXSR:
		{
			GridCell xgc = wbl[imux.loc - vec2{0, 1}];
			for (auto xidx : {10, 13})
			{
				Wire *w = xgc.xtracks[xidx];
			}
			GridCell ygc = wbl[imux.loc];
			for (auto yidx : {16, 17})
			{
				Wire *w = ygc.ytracks[yidx];
			}
		}
		break;

		case IMUXIndex::UNKNOWN: std::cerr << "ERR: Unknown IMUXIndex!!!!\n"; std::terminate();
	}
}
