//
// Created by nxmq0 on 1/3/2022.
//

#ifndef LIBTRITIUM_ROUTINGGENERATOR_H
#define LIBTRITIUM_ROUTINGGENERATOR_H
#include "types.h"
namespace tritium {
    class RoutingGenerator {
        Device & dev;
    public:
        RoutingGenerator(Device& dev) : dev(dev) {}
        void generateRoutes();

        void generate_horz_lwires();

        void generate_vert_lwires();

        void generate_horz_swires();

        void generate_vert_swires();
    };
}



#endif //LIBTRITIUM_ROUTINGGENERATOR_H
