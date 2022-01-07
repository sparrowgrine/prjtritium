//
// Created by nxmq0 on 1/6/2022.
//

#ifndef PRJTRITIUM_MEMGENERATOR_H
#define PRJTRITIUM_MEMGENERATOR_H

#include "BelCommon.h"

namespace tritium
{
	class MEMGenerator : public BelCommon
	{
	public:
		MEMGenerator(Device &dev, std::unordered_map<vec2, GridCell> &wbl, Bel &bel) : BelCommon(dev, wbl, bel) {}

		void generate() override;
	};
}

#endif // PRJTRITIUM_MEMGENERATOR_H
