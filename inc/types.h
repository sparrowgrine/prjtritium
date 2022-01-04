//
// Created by nxmq0 on 1/3/2022.
//

#ifndef LIBTRITIUM_TYPES_H
#define LIBTRITIUM_TYPES_H


#include <utility>

#include "cista.h"

namespace data = cista::offset;
namespace tritium {
    class BelPin;
    class Wire;
    class Terminus;
    class Pip;
    struct IdString {
        cista::indexed<data::string> str;
        uint32_t idx;
    };
    struct vec2 {
        uint32_t x;
        uint32_t y;
        vec2 operator+(vec2 o) const { return {x+o.x,y+o.y}; }
        vec2 operator-(vec2 o) const { return {x-o.x,y-o.y}; }
        bool operator==(vec2 o) const { return x==o.x && y==o.y; }
        size_t hash() const {
            return cista::hash_combine(x,y);
        }
    };

    struct Bel {
        enum class beltype_t : uint8_t {
            EMPTY = 0,
            IO = 1,
            GBUF = 2,
            GBUF_CTRL = 3,
            EFTIO = 4,
            EFL = 5,
            EFT = 6,
            MEM = 7,
            MULT = 8,

        };
        data::vector<uint32_t> name;
        beltype_t type;
        vec2 start;
        vec2 end;
        data::vector<data::unique_ptr<BelPin>> pins;
        [[nodiscard]] data::string typestr() const;

    };
    struct Terminus {
        enum class TermType : uint8_t {
            TERM_PIP = 0,
            TERM_BP = 1
        };
        TermType type;
        union {
            data::ptr<BelPin> bp;
            data::ptr<Pip> pip;
        };
        explicit Terminus(data::ptr<Pip> pip) {
            type = TermType::TERM_PIP;
            this->pip = std::move(pip);
        }
        explicit Terminus(data::ptr<BelPin> bp) {
            type = TermType::TERM_BP;
            this->bp = std::move(bp);
        }
        Terminus(Terminus && o) noexcept {
            type = o.type;
            switch(type) {
                case TermType::TERM_PIP:
                    this->pip = std::move(o.pip);
                    break;
                case TermType::TERM_BP:
                    this->bp = std::move(o.bp);
                    break;
            }
        }
    };
    struct Wire {
        enum class WireDirection : uint8_t {
            UNDEF = 0,
            NORTH = 1,
            SOUTH = 2,
            EAST = 3,
            WEST = 4
        };
        enum class WireType : uint8_t {
            LOCAL = 0,
            GLOBAL = 1,
            INTERNAL = 2,
            INTERBEL = 3
        };
        data::vector<uint32_t> name;
        WireDirection dir;
        WireType type;
        vec2 start;
        vec2 end;
        float R;
        float C;
        uint32_t track;
        uint32_t switchBoxIndex;
        data::vector<Terminus> sources;
        data::vector<Terminus> sinks;
    };
    struct Pip {
        data::vector<uint32_t> name;
        vec2 loc;
        data::vector<data::ptr<Wire>> inputs;
        data::vector<data::ptr<Wire>> outputs;
    };
    struct BelPin {
        enum class PinType : uint8_t {
            UNDEF = 0,
            INPUT = 1,
            OUTPUT = 2,
            INOUT = 3
        };
        data::vector<uint32_t> name;
        PinType type;
        vec2 loc;
        data::ptr<Wire> wire;
    };
    class Device {
        friend class BelCommon;
        data::string name;
        data::string speedGrade;
        vec2 dims;
        data::vector<data::unique_ptr<tritium::Bel>> bels;
        data::vector<data::unique_ptr<tritium::Wire>> wires;
        data::vector<data::unique_ptr<tritium::Pip>> pips;
        data::vector<tritium::IdString> idstrings;
        data::hash_map<data::string, uint32_t> idstring_str_to_idx;
        data::vector<data::ptr<data::string>> idstring_idx_to_str;

    public:
        tritium::IdString id(data::string str) {
            auto it = idstring_str_to_idx.find(str);
            if(it != idstring_str_to_idx.end()) {
                return idstrings.at(it->second);
            } else {
                uint32_t strid = idstring_str_to_idx.size();
                auto idstr = IdString{{str},strid};
                idstring_idx_to_str.push_back(&idstr.str);
                idstrings.emplace_back(idstr);
                idstring_str_to_idx.insert({str,strid});
                return idstr;
            }
        }
    };
}

#endif //LIBTRITIUM_TYPES_H
