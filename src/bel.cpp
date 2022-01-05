//
// Created by nxmq0 on 12/25/2021.
//

#include "types.h"

namespace tritium
{
	data::string Bel::typestr() const
	{
		switch (type)
		{
			case Bel::Type::IO: return "IO";
			case Bel::Type::GBUF: return "GBUF";
			case Bel::Type::GBUF_CTRL: return "GBUF_CTRL";
			case Bel::Type::EFTIO: return "EFTIO";
			case Bel::Type::EFL: return "EFL";
			case Bel::Type::EFT: return "EFT";
			case Bel::Type::MEM: return "MEM";
			case Bel::Type::MULT: return "MULT";
			case Bel::Type::EMPTY: return "EMPTY";
			default: return "????";
		}
	}
}
