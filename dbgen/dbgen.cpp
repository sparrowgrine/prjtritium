//
// Created by nxmq0 on 1/3/2022.
//

#include "EFLGenerator.h"
#include "EFTGenerator.h"
#include "RoutingGenerator.h"
#include "types.h"

#include <fstream>
#include <iostream>
#include <regex>

using namespace tritium;
static Bel::Type type_for_str(const std::string &str)
{
	if (str == "IO")
	{
		return Bel::Type::IO;
	}
	else if (str == "GBUF")
	{
		return Bel::Type::GBUF;
	}
	else if (str == "GBUF_CTRL")
	{
		return Bel::Type::GBUF_CTRL;
	}
	else if (str == "EFTIO")
	{
		return Bel::Type::EFTIO;
	}
	else if (str == "EFL")
	{
		return Bel::Type::EFL;
	}
	else if (str == "EFT")
	{
		return Bel::Type::EFT;
	}
	else if (str == "MEM")
	{
		return Bel::Type::MEM;
	}
	else if (str == "MULT")
	{
		return Bel::Type::MULT;
	}
	else
	{
		return Bel::Type::EMPTY;
	}
}

void gen_wbl(std::unordered_map<vec2, GridCell> &wbl, Device &dev)
{
	for (auto &wire : dev.wires)
	{
		switch (wire->dir)
		{
			case tritium::Wire::Direction::NORTH:
				for (uint32_t i = wire->start.y; i <= wire->end.y; i++)
				{
					wbl[vec2{wire->start.x, i}].ytracks[wire->track] = wire.get();
				}
				break;
			case tritium::Wire::Direction::SOUTH:
				for (uint32_t i = wire->end.y; i <= wire->start.y; i++)
				{
					wbl[vec2{wire->start.x, i}].ytracks[wire->track] = wire.get();
				}
				break;
			case tritium::Wire::Direction::EAST:
				for (uint32_t i = wire->start.x; i <= wire->end.x; i++)
				{
					wbl[vec2{i, wire->start.y}].xtracks[wire->track] = wire.get();
				}
				break;
			case tritium::Wire::Direction::WEST:
				for (uint32_t i = wire->end.x; i <= wire->start.x; i++)
				{
					wbl[vec2{i, wire->start.y}].xtracks[wire->track] = wire.get();
				}
				break;
		}
	}
}

void parse_beldb(Device &dev, std::unordered_map<vec2, GridCell> &wbl, std::string fname)
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
		auto typestr = match[1].str();
		auto belx    = std::stoul(match[2].str());
		auto bely    = std::stoul(match[3].str());
		auto &bel{dev.make_bel(type_for_str(typestr), {belx, bely})};
		switch (bel.type)
		{
			case Bel::Type::EFL: EFLGenerator{dev, wbl, bel}.generate(); break;
			case Bel::Type::EFT: EFTGenerator{dev, wbl, bel}.generate(); break;
			default: break;
		}
	}
}

int main(int argc, char **argv)
{
	Device dev{.name = "oph_77x162_b3_d1", .speedGrade = "C3", .dims = {.x = 78, .y = 163}};
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
	parse_beldb(dev, wbl, "../beldb/oph_77x162_b3_d1.beldb");
	cista::file chipdb{"oph_77x162_b3_d1_C3.chipdb", "w+"};
	cista::serialize<cista::mode::WITH_INTEGRITY | cista::mode::WITH_VERSION>(chipdb, dev);
	std::cout << "meow\n";
	return 0;
}
