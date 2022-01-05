//
// Created by nxmq0 on 1/3/2022.
//

#ifndef LIBTRITIUM_EFLGENERATOR_H
#define LIBTRITIUM_EFLGENERATOR_H

#include "BelCommon.h"

namespace tritium
{
	class EFLGenerator : public BelCommon
	{
	public:
		EFLGenerator(Device &dev,
		             std::unordered_map<vec2, GridCell> &wbl,
		             Bel &bel)
		: BelCommon(dev, wbl, bel)
		{
		}

		void generate() override;
	};
}

#endif // LIBTRITIUM_EFLGENERATOR_H
