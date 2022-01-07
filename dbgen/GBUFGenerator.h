//
// Created by nxmq0 on 1/6/2022.
//

#ifndef PRJTRITIUM_GBUFGENERATOR_H
#define PRJTRITIUM_GBUFGENERATOR_H

#include "BelCommon.h"

namespace tritium
{
	class GBUFGenerator : public BelCommon
	{
	public:
		GBUFGenerator(Device &dev, std::unordered_map<vec2, GridCell> &wbl, Bel &bel) : BelCommon(dev, wbl, bel) {}

		void generate() override;
	};
}

#endif // PRJTRITIUM_GBUFGENERATOR_H
