//
// Created by nxmq0 on 1/6/2022.
//

#include "IOGenerator.h"

void tritium::IOGenerator::generate()
{
	auto &outpad{make_pin("OPAD", BelPin::PinType::INPUT)};
	auto &inpad{make_pin("IPAD", BelPin::PinType::OUTPUT)};
}
