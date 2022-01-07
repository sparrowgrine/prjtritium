//
// Created by nxmq0 on 1/3/2022.
//

#include "EFLGenerator.h"
#include "EFTGenerator.h"
#include "EFTIOGenerator.h"
#include "GBUFCTRLGenerator.h"
#include "GBUFGenerator.h"
#include "IOGenerator.h"
#include "MEMGenerator.h"
#include "MULTGenerator.h"
#include "RoutingGenerator.h"
#include "types.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

namespace fs = std::filesystem;
using namespace tritium;

uint32_t vpr_to_phy_track(uint32_t vtrack, uint32_t x, uint32_t y, bool isXRoute, bool isLongRoute)
{
	const uint32_t segNum{isXRoute ? x : y};
	const uint32_t chanNum{isXRoute ? y : x};
	const uint32_t group_size{isLongRoute ? 40U : 8U};
	const uint32_t group_start{isLongRoute ? 8U : 0U};

	const uint32_t dir{2U};

	const uint32_t idek{(((segNum - 1U) + (chanNum - (isXRoute ? 0U : 1U))) * dir) % group_size};
	const uint32_t group_size_over_dir{group_size / dir};
	const uint32_t vtrack_sub_group_start{vtrack - group_start};

	return (dir * (((group_size_over_dir - (idek / dir)) + (vtrack_sub_group_start / dir)) % group_size_over_dir)) +
	       group_start + (vtrack_sub_group_start % dir);
}

static Bel::Type type_for_str(const std::string &str)
{
	if (str == "io")
		return Bel::Type::IO;
	else if (str == "gbuf")
		return Bel::Type::GBUF;
	else if (str == "gbuf_ctrl")
		return Bel::Type::GBUF_CTRL;
	else if (str == "eftio")
		return Bel::Type::EFTIO;
	else if (str == "efl")
		return Bel::Type::EFL;
	else if (str == "eft")
		return Bel::Type::EFT;
	else if (str == "mem")
		return Bel::Type::MEM;
	else if (str == "mult")
		return Bel::Type::MULT;
	else
		return Bel::Type::EMPTY;
}

void gen_wbl(std::unordered_map<vec2, GridCell> &wbl, Device &dev)
{
	for (auto &wire : dev.wires)
	{
		if (wire->track > 47) continue;
		switch (wire->dir)
		{
			case tritium::Wire::Direction::NORTH:
				for (uint32_t i = wire->start.y; i <= wire->end.y; i++)
				{
					const uint32_t track{vpr_to_phy_track(wire->track, wire->start.x, i, false, wire->track > 7)};
					wbl[vec2{wire->start.x, i}].ytracks[track] = wire.get();
				}
				break;

			case tritium::Wire::Direction::SOUTH:
				for (uint32_t i = wire->end.y; i <= wire->start.y; i++)
				{
					const uint32_t track{vpr_to_phy_track(wire->track, wire->start.x, i, false, wire->track > 7)};
					wbl[vec2{wire->start.x, i}].ytracks[track] = wire.get();
				}
				break;

			case tritium::Wire::Direction::EAST:
				for (uint32_t i = wire->start.x; i <= wire->end.x; i++)
				{
					const uint32_t track{vpr_to_phy_track(wire->track, i, wire->start.y, true, wire->track > 7)};
					wbl[vec2{i, wire->start.y}].xtracks[track] = wire.get();
				}
				break;

			case tritium::Wire::Direction::WEST:
				for (uint32_t i = wire->end.x; i <= wire->start.x; i++)
				{
					const uint32_t track{vpr_to_phy_track(wire->track, i, wire->start.y, true, wire->track > 7)};
					wbl[vec2{i, wire->start.y}].xtracks[track] = wire.get();
				}
				break;
		}
	}
}

void parse_beldb(Device &dev, std::unordered_map<vec2, GridCell> &wbl, const std::filesystem::path &fname)
{
	std::ifstream belfile(fname);
	if (!belfile)
	{
		std::cerr << "Unable to open beldb!\n";
		std::terminate();
	}

	std::regex belentry{R"(type: (\w+) x: (\d+) y: (\d+))"};
	while (belfile.good())
	{
		std::string line;
		std::getline(belfile, line);

		std::smatch match;
		if (!std::regex_match(line, match, belentry))
		{
			std::cerr << "Invalid entry in beldb!\n";
			std::terminate();
		}

		auto typestr{match[1].str()};
		auto belx{std::stoul(match[2].str())};
		auto bely{std::stoul(match[3].str())};
		auto &bel{dev.make_bel(type_for_str(typestr), {belx, bely})};

		switch (bel.type)
		{
			case Bel::Type::EFL: EFLGenerator{dev, wbl, bel}.generate(); break;
			case Bel::Type::EFT: EFTGenerator{dev, wbl, bel}.generate(); break;
			case Bel::Type::EFTIO: EFTIOGenerator{dev, wbl, bel}.generate(); break;
			case Bel::Type::GBUF: GBUFGenerator{dev, wbl, bel}.generate(); break;
			case Bel::Type::GBUF_CTRL: GBUFCTRLGenerator{dev, wbl, bel}.generate(); break;
			case Bel::Type::IO: IOGenerator{dev, wbl, bel}.generate(); break;
			case Bel::Type::MEM: MEMGenerator{dev, wbl, bel}.generate(); break;
			case Bel::Type::MULT: MULTGenerator{dev, wbl, bel}.generate(); break;
			default: break;
		}
	}
}

int main(int argc, char **argv)
{
	std::cout << "nya\n";

	Device dev{
	    .dims       = {.x = 78, .y = 163},
	    .name       = "oph_77x162_b3_d1",
	    .speedGrade = "C3",
	};

	RoutingGenerator rg{dev};
	rg.generateRoutes();

	std::unordered_map<vec2, GridCell> wbl;
	for (uint32_t i = 0; i < dev.dims.x; i++)
	{
		for (uint32_t j = 0; j < dev.dims.y; j++)
		{
			wbl[vec2{i, j}] = GridCell{};
		}
	}

	gen_wbl(wbl, dev);

	parse_beldb(dev, wbl, fs::path{argc > 1 ? argv[1] : "../beldb"} / "oph_77x162_b3_d1.beldb");

	cista::file chipdb{"oph_77x162_b3_d1_C3.chipdb", "w+"};

	std::cout << "awoo\n";

	cista::serialize<cista::mode::WITH_VERSION>(chipdb, dev);

	std::cout << "meow\n";

	return 0;
}
