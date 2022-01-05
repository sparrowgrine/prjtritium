//
// Created by nxmq0 on 1/3/2022.
//

#include "EFLGenerator.h"
#include "RoutingGenerator.h"
#include "types.h"

#include <iostream>

int main(int argc, char **argv)
{
	tritium::Device dev{
	  .name       = "oph_77x162_b3_d1",
	  .speedGrade = "C3",
	  .dims       = {.x = 78, .y = 163},
	};

	tritium::RoutingGenerator rg{dev};
	rg.generateRoutes();

	std::cout << "meow\n";

	return 0;
}
