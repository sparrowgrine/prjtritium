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

#include <charconv>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>
#include <string_view>

namespace fs = std::filesystem;
using namespace tritium;

static Bel::Type type_for_str(std::string_view str)
{
	using namespace std::string_view_literals;

	if (str == "io"sv)
		return Bel::Type::IO;
	else if (str == "gbuf"sv)
		return Bel::Type::GBUF;
	else if (str == "gbuf_ctrl"sv)
		return Bel::Type::GBUF_CTRL;
	else if (str == "eftio"sv)
		return Bel::Type::EFTIO;
	else if (str == "efl"sv)
		return Bel::Type::EFL;
	else if (str == "eft"sv)
		return Bel::Type::EFT;
	else if (str == "mem"sv)
		return Bel::Type::MEM;
	else if (str == "mult"sv)
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
					const uint32_t track{wire->getPTrackAt(vec2{wire->start.x, i})};
					wbl[vec2{wire->start.x, i}].ytracks[track] = wire.get();
				}
				break;

			case tritium::Wire::Direction::SOUTH:
				for (uint32_t i = wire->end.y; i <= wire->start.y; i++)
				{
					const uint32_t track{wire->getPTrackAt(vec2{wire->start.x, i})};
					wbl[vec2{wire->start.x, i}].ytracks[track] = wire.get();
				}
				break;

			case tritium::Wire::Direction::EAST:
				for (uint32_t i = wire->start.x; i <= wire->end.x; i++)
				{
					const uint32_t track{wire->getPTrackAt(vec2{i, wire->start.y})};
					wbl[vec2{i, wire->start.y}].xtracks[track] = wire.get();
				}
				break;

			case tritium::Wire::Direction::WEST:
				for (uint32_t i = wire->end.x; i <= wire->start.x; i++)
				{
					const uint32_t track{wire->getPTrackAt(vec2{i, wire->start.y})};
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

		auto stou32 = [](std::string_view str)
		{
			uint32_t result;
			auto [ptr, ec]{std::from_chars(str.data(), str.data() + str.size(), result)};
			assert(ec == std::errc());
			return result;
		};
		auto typestr{match[1].str()};
		auto belx{stou32(match[2].str())};
		auto bely{stou32(match[3].str())};
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

	//	std::ofstream echowires("wires.echo");
	//	for (auto &wire : dev.wires)
	//	{
	//		echowires << fmt::format("{}\n", wire->getName(dev));
	//	}
	//	echowires.close();

	// std::ofstream echoswitch8("iswitch8.echo");
	// for (auto &pip : dev.pips)
	// {
	// 	echoswitch8 << fmt::format("{} -> {}\n", pip->inputs[0]->getName(dev), pip->outputs[0]->getName(dev));
	// }
	// echoswitch8.close();

	std::unordered_map<vec2, GridCell> wbl;
	for (uint32_t i = 0; i < dev.dims.x; i++)
	{
		for (uint32_t j = 0; j < dev.dims.y; j++)
		{
			wbl[vec2{i, j}] = GridCell{};
		}
	}

	gen_wbl(wbl, dev);
	rg.generateSwitchboxes(wbl);

	//	std::ofstream echosb("sb.echo");
	//	for (auto &pip : dev.pips)
	//	{
	//		if (pip->name[0] != dev.id("SBOX")) continue;
	//		echosb << fmt::format("{} -> {}\n", pip->inputs[0]->getName(dev), pip->outputs[0]->getName(dev));
	//		echosb << fmt::format("{} -> {}\n", pip->inputs[0]->getName(dev), pip->outputs[1]->getName(dev));
	//		echosb << fmt::format("{} -> {}\n", pip->inputs[0]->getName(dev), pip->outputs[2]->getName(dev));
	//		echosb << fmt::format("{} -> {}\n", pip->inputs[0]->getName(dev), pip->outputs[3]->getName(dev));
	//	}
	//	echosb.close();

	std::cout << fmt::format(
	    "Successfully generated routing, with {} wires and {} aliases.\n", dev.wires.size(), dev.pips.size());

	parse_beldb(dev, wbl, fs::path{argc > 1 ? argv[1] : "../beldb"} / "oph_77x162_b3_d1.beldb");
	rg.applySwitchCapacitances();
	cista::file chipdb{"oph_77x162_b3_d1_C3.chipdb", "w+"};

	std::cout << "awoo\n";

	cista::serialize<cista::mode::WITH_VERSION>(chipdb, dev);
	auto devfile = cista::mmap("oph_77x162_b3_d1_C3.chipdb", cista::mmap::protection::READ);
	auto devload{cista::deserialize<tritium::Device>(devfile)};

	std::cout << "meow\n";

	return 0;
}
