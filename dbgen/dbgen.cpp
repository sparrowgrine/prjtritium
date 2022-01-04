//
// Created by nxmq0 on 1/3/2022.
//

#include "EFLGenerator.h"
#include "RoutingGenerator.h"
#include "types.h"

#include <iostream>

int main(int argc, char **argv)
{
	auto dev       = tritium::Device();
	dev.name       = "oph_77x162_b3_d1";
	dev.speedGrade = "C3";
	dev.dims.x     = 78;
	dev.dims.y     = 163;
	auto rg        = tritium::RoutingGenerator(dev);
	rg.generateRoutes();
	std::cout << "meow" << std::endl;
	return 0;
}
