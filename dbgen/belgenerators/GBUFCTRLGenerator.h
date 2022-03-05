//
// Created by nxmq0 on 1/6/2022.
//

#ifndef PRJTRITIUM_GBUFCTRLGENERATOR_H
#define PRJTRITIUM_GBUFCTRLGENERATOR_H

#include "BelCommon.h"

namespace tritium
{
	class GBUFCTRLGenerator : public BelCommon
	{
	public:
		GBUFCTRLGenerator(Device &dev, std::unordered_map<vec2, GridCell> &wbl, Bel &bel) : BelCommon(dev, wbl, bel) {}

		void generate();
	};
}

#endif // PRJTRITIUM_GBUFCTRLGENERATOR_H
