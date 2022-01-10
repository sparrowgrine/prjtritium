//
// Created by nxmq0 on 1/3/2022.
//

#ifndef LIBTRITIUM_GRIDCELL_H
#define LIBTRITIUM_GRIDCELL_H

#include "types.h"

#include <array>

/*
 * Each GridCell represents the available horizontal/vertical wires at a specific location.
 * These routes are allocated as follows:
 * Horizontal Routing:
 *  8 Short Wires
 *  40 Long Wires
 * Vertical Routing:
 *  8 Short Wires
 *  40 Long Wires
 *  4 Column Clocks
 *  4 Column SRs???
 *  4 Column CEs???
 * (Last two are speculated)
 * Anyways, thats why ytracks = 60 and xtracks = 48
 */
namespace tritium
{
	struct GridCell
	{
		std::array<Wire *, 48> xtracks;
		std::array<Wire *, 60> ytracks;
	};
}

#endif // LIBTRITIUM_GRIDCELL_H
