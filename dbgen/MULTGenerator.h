//
// Created by nxmq0 on 1/6/2022.
//

#ifndef PRJTRITIUM_MULTGENERATOR_H
#define PRJTRITIUM_MULTGENERATOR_H

#include "BelCommon.h"

namespace tritium
{
	class MULTGenerator : public BelCommon
	{
	public:
		MULTGenerator(Device &dev, std::unordered_map<vec2, GridCell> &wbl, Bel &bel) : BelCommon(dev, wbl, bel) {}

		void generate();
	};
}

#endif // PRJTRITIUM_MULTGENERATOR_H
