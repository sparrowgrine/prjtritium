//
// Created by nxmq0 on 1/3/2022.
//

#ifndef LIBTRITIUM_EFTGENERATOR_H
#define LIBTRITIUM_EFTGENERATOR_H

#include "BelCommon.h"

namespace tritium
{
	class EFTGenerator : public BelCommon
	{
	public:
		EFTGenerator(Device &dev, std::unordered_map<vec2, GridCell> &wbl, Bel &bel) : BelCommon(dev, wbl, bel) {}

		void generate();
	};
}

#endif // LIBTRITIUM_EFTGENERATOR_H
