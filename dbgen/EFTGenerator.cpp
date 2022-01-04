//
// Created by nxmq0 on 1/3/2022.
//

#include "EFTGenerator.h"

void tritium::EFTGenerator::generate() {
    auto l4i0 = mk_pin("L4_I0",BelPin::PinType::INPUT);
    auto l4i1 = mk_pin("L4_I0",BelPin::PinType::INPUT);
    auto l4i2 = mk_pin("L4_I0",BelPin::PinType::INPUT);
    auto l4i3 = mk_pin("L4_I0",BelPin::PinType::INPUT);
    auto i0 = mk_pin("FA_I0", BelPin::PinType::INPUT);
    auto i1 = mk_pin("FA_I1", BelPin::PinType::INPUT);
    auto ci = mk_pin("FA_CI", BelPin::PinType::INPUT);
    auto co = mk_pin("FA_I0", BelPin::PinType::OUTPUT);
    auto opin_lut = mk_pin("L4_O", BelPin::PinType::OUTPUT);
    auto opin_add = mk_pin("FA_O", BelPin::PinType::OUTPUT);
    auto opin_mux = mk_pip("OMUXCELL");
    auto opin_ff = mk_pin("FF_O",BelPin::PinType::OUTPUT);
    lnk_termini(opin_lut, opin_mux);
    lnk_termini(opin_add, opin_mux);
    auto expmuxa = mk_pip("EXPMUXA");
    auto expmuxb = mk_pip("EXPMUXA");
    lnk_termini(l4i0,expmuxa);
    lnk_termini(l4i2,expmuxa);
    lnk_termini(l4i1,expmuxb);
    lnk_termini(l4i3,expmuxb);
    lnk_termini(expmuxa,i0);
    lnk_termini(expmuxb,i1);
    auto rtmuxns = mk_pip("RTMUXNS");
    auto rtmuxew = mk_pip("RTMUXEW");
    // inwires = ipins_lut.map(&:wires).flatten
    // inwires.each do |iwire|
    //   lnk_wire_to(iwire,rtmuxns)
    //   lnk_wire_to(iwire,rtmuxew)
    // end
    auto omuxshort = mk_pip("OMUXSHORT");
    lnk_termini(rtmuxew,omuxshort);
    lnk_termini(rtmuxns,omuxshort);
    lnk_termini(opin_mux,omuxshort);
    lnk_to_wire(omuxshort,out_swire_for_dir(Wire::Direction::NORTH));
    lnk_to_wire(omuxshort,out_swire_for_dir(Wire::Direction::EAST));
    lnk_to_wire(omuxshort,out_swire_for_dir(Wire::Direction::WEST));
    lnk_to_wire(omuxshort,out_swire_for_dir(Wire::Direction::SOUTH));
    auto omuxnorth = mk_pip("OMUXNORTH");
    lnk_termini(rtmuxns,omuxnorth);
    lnk_termini(opin_mux,omuxnorth);
    lnk_to_wire(omuxnorth,out_lwire_for_dir(Wire::Direction::NORTH));
    auto omuxsouth = mk_pip("OMUXSOUTH");
    lnk_termini(rtmuxns,omuxsouth);
    lnk_termini(opin_mux,omuxsouth);
    lnk_to_wire(omuxsouth,out_lwire_for_dir(Wire::Direction::SOUTH));
    auto omuxwest = mk_pip("OMUXWEST");
    lnk_termini(rtmuxns,omuxwest);
    lnk_termini(opin_mux,omuxwest);
    lnk_to_wire(omuxwest,out_lwire_for_dir(Wire::Direction::WEST));
    auto omuxeast = mk_pip("OMUXEAST");
    lnk_termini(rtmuxew,omuxeast);
    lnk_termini(opin_mux,omuxeast);
    lnk_to_wire(omuxeast,out_lwire_for_dir(Wire::Direction::EAST));
}