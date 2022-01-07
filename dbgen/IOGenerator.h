//
// Created by nxmq0 on 1/6/2022.
//

#ifndef PRJTRITIUM_IOGENERATOR_H
#define PRJTRITIUM_IOGENERATOR_H

#include "BelCommon.h"

namespace tritium
{
	class IOGenerator : public BelCommon
	{
	public:
		IOGenerator(Device &dev, std::unordered_map<vec2, GridCell> &wbl, Bel &bel) : BelCommon(dev, wbl, bel) {}

		void generate() override;
	};
}

#endif // PRJTRITIUM_IOGENERATOR_H
