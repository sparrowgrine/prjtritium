//
// Created by nxmq0 on 1/3/2022.
//

#include "BelCommon.h"
#include <fmt/core.h>
#include <iostream>

template<>
struct std::hash<tritium::vec2> {
    size_t operator()(const tritium::vec2 &v) const noexcept {
        return cista::hash_combine(v.x,v.y);
    };
};

void tritium::BelCommon::generate() {
    throw "Cannot generate an undifferentiated cell!";
}

tritium::Pip &tritium::BelCommon::mk_pip(const data::string& name, tritium::vec2 loc) {
    auto& pip = *dev.pips.emplace_back(data::make_unique<Pip>(Pip{}));
    pip.name.push_back(dev.id("INT").idx);
    pip.name.push_back(dev.id(bel.typestr()).idx);
    pip.name.push_back(dev.id(fmt::format("X{}Y{}",loc.x,loc.y)).idx);
    pip.name.push_back(dev.id(name).idx);
    pip.loc = loc;
    return pip;
}

tritium::Wire &tritium::BelCommon::mk_intwire(const data::string& name, tritium::vec2 loc) {
    auto& wire=  *dev.wires.emplace_back(data::make_unique<Wire>(Wire{}));
    wire.name.push_back(dev.id("INT").idx);
    wire.name.push_back(dev.id(bel.typestr()).idx);
    wire.name.push_back(dev.id(fmt::format("X{}Y{}",loc.x,loc.y)).idx);
    wire.name.push_back(dev.id(name).idx);
    wire.type = Wire::WireType::INTERNAL;
    wire.start = loc;
    wire.end = loc;
    return wire;
}

tritium::BelPin &tritium::BelCommon::mk_pin(const data::string &name, BelPin::PinType type, tritium::vec2 loc) {
    auto& bp =  *bel.pins.emplace_back(data::make_unique<BelPin>(BelPin{}));
    bp.name.push_back(dev.id(bel.typestr()).idx);
    bp.name.push_back(dev.id(fmt::format("X{}Y{}",loc.x,loc.y)).idx);
    bp.name.push_back(dev.id(name).idx);
    bp.type = type;
    return bp;
}

void tritium::BelCommon::lnk_wire_to(tritium::Wire &wire, tritium::Pip &tgt) {
    tgt.inputs.push_back(&wire);
    wire.sinks.emplace_back(&tgt);
}

void tritium::BelCommon::lnk_wire_to(tritium::Wire &wire, tritium::BelPin &tgt) {
    tgt.wire = &wire;
    wire.sinks.emplace_back(&tgt);
}

void tritium::BelCommon::lnk_to_wire(tritium::Pip &src, tritium::Wire &wire) {
    src.outputs.push_back(&wire);
    wire.sources.emplace_back(&src);
}

void tritium::BelCommon::lnk_to_wire(tritium::BelPin &src, tritium::Wire &wire) {
    src.wire = &wire;
    wire.sources.emplace_back(&src);
}

tritium::Wire &tritium::BelCommon::out_lwire_for_dir(tritium::Wire::Direction dir, tritium::vec2 loc) {
    switch(dir) {
        case Wire::Direction::NORTH:
            for(auto wire : wbl[loc].ytracks) {
                if(wire->start == loc && wire->dir == dir && wire->name.at(0) == dev.id("VL").idx)
                    return *wire;
            }
            std::cerr << "ERR: Misformed routing!!!!" << std::endl;
            std::terminate();
        case Wire::Direction::SOUTH:
            for(auto wire : wbl[loc-vec2{0,1}].ytracks) {
                if(wire->start == loc && wire->dir == dir && wire->name.at(0) == dev.id("VL").idx)
                    return *wire;
            }
            std::cerr << "ERR: Misformed routing!!!!" << std::endl;
            std::terminate();
        case Wire::Direction::EAST:
            for(auto wire : wbl[vec2{loc.x+1,loc.y-1}].xtracks) {
                if(wire->start == loc && wire->dir == dir && wire->name.at(0) == dev.id("HL").idx)
                    return *wire;
            }
            std::cerr << "ERR: Misformed routing!!!!" << std::endl;
            std::terminate();
        case Wire::Direction::WEST:
            for(auto wire : wbl[loc-vec2{0,1}].xtracks) {
                if(wire->start == loc && wire->dir == dir && wire->name.at(0) == dev.id("HL").idx)
                    return *wire;
            }
            std::cerr << "ERR: Misformed routing!!!!" << std::endl;
            std::terminate();
        default:
            std::cerr << "ERR: Invalid Output Wire Request!!!!" << std::endl;
            std::terminate();
            break;
    }
    return *new Wire(); //never gonna get here lol
}

tritium::Wire &tritium::BelCommon::out_swire_for_dir(tritium::Wire::Direction dir, tritium::vec2 loc) {
    switch(dir) {
        case Wire::Direction::NORTH:
            for(auto wire : wbl[loc].ytracks) {
                if(wire->start == loc && wire->dir == dir && wire->name.at(0) == dev.id("VS").idx)
                    return *wire;
            }
            std::cerr << "ERR: Misformed routing!!!!" << std::endl;
            std::terminate();
        case Wire::Direction::SOUTH:
            for(auto wire : wbl[loc-vec2{0,1}].ytracks) {
                if(wire->start == loc && wire->dir == dir && wire->name.at(0) == dev.id("VS").idx)
                    return *wire;
            }
            std::cerr << "ERR: Misformed routing!!!!" << std::endl;
            std::terminate();
        case Wire::Direction::EAST:
            for(auto wire : wbl[vec2{loc.x+1,loc.y-1}].xtracks) {
                if(wire->start == loc && wire->dir == dir && wire->name.at(0) == dev.id("HS").idx)
                    return *wire;
            }
            std::cerr << "ERR: Misformed routing!!!!" << std::endl;
            std::terminate();
        case Wire::Direction::WEST:
            for(auto wire : wbl[loc-vec2{0,1}].xtracks) {
                if(wire->start == loc && wire->dir == dir && wire->name.at(0) == dev.id("HS").idx)
                    return *wire;
            }
            std::cerr << "ERR: Misformed routing!!!!" << std::endl;
            std::terminate();
        default:
            std::cerr << "ERR: Invalid Output Wire Request!!!!" << std::endl;
            std::terminate();
    }
    return *new Wire(); //never gonna get here lol

}
