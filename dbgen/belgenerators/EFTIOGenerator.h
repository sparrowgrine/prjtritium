//
// Created by nxmq0 on 1/6/2022.
//

#ifndef PRJTRITIUM_EFTIOGENERATOR_H
#define PRJTRITIUM_EFTIOGENERATOR_H

#include "BelCommon.h"

namespace tritium
{
	class EFTIOGenerator : public BelCommon
	{
	public:
		EFTIOGenerator(Device &dev, std::unordered_map<vec2, GridCell> &wbl, Bel &bel) : BelCommon(dev, wbl, bel) {}

		void generate();
	};
}

#endif // PRJTRITIUM_EFTIOGENERATOR_H
