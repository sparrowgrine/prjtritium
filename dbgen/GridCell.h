//
// Created by nxmq0 on 1/3/2022.
//

#ifndef LIBTRITIUM_GRIDCELL_H
#define LIBTRITIUM_GRIDCELL_H

#include "types.h"

#include <array>

namespace tritium
{
	struct GridCell
	{
		std::array<Wire *, 48> xtracks;
		std::array<Wire *, 48> ytracks;
	};
}

#endif // LIBTRITIUM_GRIDCELL_H
