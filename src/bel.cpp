//
// Created by nxmq0 on 12/25/2021.
//

#include "types.h"

#include <string_view>

namespace tritium
{
	std::string Bel::getName(Device &dev)
	{
		std::string out{dev.str(name.front())};
		for (auto id = name.begin() + 1; id != name.end(); ++id)
		{
			out += ":";
			out += dev.str(*id);
		}
		return out;
	}

	std::string_view Bel::typestr() const
	{
		using namespace std::string_view_literals;

		switch (type)
		{
#define TRITIUM_BEL_TYPE_NAME(IDEN, ...)                                                                              \
	case Bel::Type::IDEN: return #IDEN ""sv;
			TRITIUM_BEL_TYPE_MACRO(TRITIUM_BEL_TYPE_NAME)
#undef TRITIUM_BEL_TYPE_NAME
			default: return "????"sv;
		}
	}
}
