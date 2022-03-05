//
// Created by nxmq0 on 12/25/2021.
//

#include "types.h"
#include <fmt/core.h>

namespace tritium
{
	Bel &Device::make_bel(Bel::Type type, vec2 start)
	{
		auto &bel{*bels.emplace_back(data::make_unique<Bel>(Bel{}))};
		bel.id = bels.size()-1;
		bel.type  = type;
		bel.start = start;
		if (bel.type == Bel::Type::MEM || bel.type == Bel::Type::MULT)
		{
			bel.end = bel.start + vec2{0, 19};
		}
		else
		{
			bel.end = bel.start;
		}
		bel.name.push_back(id(bel.typestr()));
		bel.name.push_back(id(fmt::format("X{}Y{}", start.x, start.y)));
		return bel;
	}

	uint32_t Device::id(std::string_view str)
	{
		auto it = idstring_str_to_idx.find(str);
		if (it != idstring_str_to_idx.end())
		{
			return it->second;
		}
		else
		{
			uint32_t strid = idstring_str_to_idx.size();
			idstring_idx_to_str.emplace_back(str);
			idstring_str_to_idx.insert({str, strid});
			return strid;
		}
	}

	const data::string &Device::str(uint32_t id) { return idstring_idx_to_str.at(id); }
}
