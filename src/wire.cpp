//
// Created by nxmq0 on 1/2/2022.
//

#include "types.h"

namespace tritium
{
	std::string Wire::getName(Device &dev)
	{
		std::string out{dev.str(name.front())};
		for (auto id = name.begin() + 1; id != name.end(); ++id)
		{
			out += ":";
			out += dev.str(*id);
		}
		return out;
	}
}