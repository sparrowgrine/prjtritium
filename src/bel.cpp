//
// Created by nxmq0 on 12/25/2021.
//

#include <cassert>
#include "types.h"

namespace tritium {
    data::string Bel::typestr() const {
        switch(type) {
            case Bel::beltype_t::IO:
                return "IO";
            case Bel::beltype_t::GBUF:
                return "GBUF";
            case Bel::beltype_t::GBUF_CTRL:
                return "GBUF_CTRL";
            case Bel::beltype_t::EFTIO:
                return "EFTIO";
            case Bel::beltype_t::EFL:
                return "EFL";
            case Bel::beltype_t::EFT:
                return "EFT";
            case Bel::beltype_t::MEM:
                return "MEM";
            case Bel::beltype_t::MULT:
                return "MULT";
            case Bel::beltype_t::EMPTY:
                return "EMPTY";
            default:
                return "????";
        }
    }
}


