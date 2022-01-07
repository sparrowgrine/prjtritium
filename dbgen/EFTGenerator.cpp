//
// Created by nxmq0 on 1/3/2022.
//

#include "EFTGenerator.h"

void tritium::EFTGenerator::generate()
{
	auto &l4i0{make_pin("L4_I0", BelPin::PinType::INPUT)};
	auto &l4i1{make_pin("L4_I1", BelPin::PinType::INPUT)};
	auto &l4i2{make_pin("L4_I2", BelPin::PinType::INPUT)};
	auto &l4i3{make_pin("L4_I3", BelPin::PinType::INPUT)};

	auto &imuxa{make_pip("IMUXA")};
	auto &imuxb{make_pip("IMUXB")};
	auto &imuxc{make_pip("IMUXC")};
	auto &imuxd{make_pip("IMUXD")};

	link_wires_to_imux_by_index(imuxa,BelCommon::IMUXIndex::IMUXA);
	link_wires_to_imux_by_index(imuxb,BelCommon::IMUXIndex::IMUXB);
	link_wires_to_imux_by_index(imuxc,BelCommon::IMUXIndex::IMUXC);
	link_wires_to_imux_by_index(imuxd,BelCommon::IMUXIndex::IMUXD);

	link_termini(imuxa,l4i0);
	link_termini(imuxb,l4i1);
	link_termini(imuxc,l4i2);
	link_termini(imuxd,l4i3);

	auto &i0{make_pin("FA_I0", BelPin::PinType::INPUT)};
	auto &i1{make_pin("FA_I1", BelPin::PinType::INPUT)};
	auto &ci{make_pin("FA_CI", BelPin::PinType::INPUT)};
	auto &co{make_pin("FA_I0", BelPin::PinType::OUTPUT)};

	auto &opin_lut{make_pin("L4_O", BelPin::PinType::OUTPUT)};
	auto &opin_add{make_pin("FA_O", BelPin::PinType::OUTPUT)};
	auto &opin_mux{make_pip("OMUXCELL")};

	auto &ff_d{make_pin("FF_D", BelPin::PinType::OUTPUT)};
	auto &ff_q{make_pin("FF_Q", BelPin::PinType::OUTPUT)};
    auto &ff_clk{make_pin("FF_CLK", BelPin::PinType::INPUT)};
	auto &ff_ce{make_pin("FF_CE", BelPin::PinType::INPUT)};
	auto &ff_sr{make_pin("FF_SR", BelPin::PinType::INPUT)};

	link_termini(opin_lut, opin_mux);
	link_termini(opin_add, opin_mux);

	auto &expmuxa{make_pip("EXPMUXA")};
	auto &expmuxb{make_pip("EXPMUXA")};

	link_termini(l4i0, expmuxa);
	link_termini(l4i2, expmuxa);
	link_termini(l4i1, expmuxb);
	link_termini(l4i3, expmuxb);
	link_termini(expmuxa, i0);
	link_termini(expmuxb, i1);

	auto &rtmuxns{make_pip("RTMUXNS")};
	auto &rtmuxew{make_pip("RTMUXEW")};

	link_termini(imuxa,rtmuxns);
	link_termini(imuxa,rtmuxew);
	link_termini(imuxb,rtmuxns);
	link_termini(imuxb,rtmuxew);
	link_termini(imuxc,rtmuxns);
	link_termini(imuxc,rtmuxew);
	link_termini(imuxd,rtmuxns);
	link_termini(imuxd,rtmuxew);

	auto &omuxshort{make_pip("OMUXSHORT")};

	link_termini(rtmuxew, omuxshort);
	link_termini(rtmuxns, omuxshort);
	link_termini(opin_mux, omuxshort);

	link_to_wire(omuxshort, out_swire_for_dir(Wire::Direction::NORTH));
	link_to_wire(omuxshort, out_swire_for_dir(Wire::Direction::EAST));
	link_to_wire(omuxshort, out_swire_for_dir(Wire::Direction::WEST));
	link_to_wire(omuxshort, out_swire_for_dir(Wire::Direction::SOUTH));

	auto &omuxnorth{make_pip("OMUXNORTH")};
	link_termini(rtmuxns, omuxnorth);
	link_termini(opin_mux, omuxnorth);
	link_to_wire(omuxnorth, out_lwire_for_dir(Wire::Direction::NORTH));

	auto &omuxsouth{make_pip("OMUXSOUTH")};
	link_termini(rtmuxns, omuxsouth);
	link_termini(opin_mux, omuxsouth);
	link_to_wire(omuxsouth, out_lwire_for_dir(Wire::Direction::SOUTH));

	auto &omuxwest{make_pip("OMUXWEST")};
	link_termini(rtmuxns, omuxwest);
	link_termini(opin_mux, omuxwest);
	link_to_wire(omuxwest, out_lwire_for_dir(Wire::Direction::WEST));

	auto &omuxeast{make_pip("OMUXEAST")};
	link_termini(rtmuxew, omuxeast);
	link_termini(opin_mux, omuxeast);
	link_to_wire(omuxeast, out_lwire_for_dir(Wire::Direction::EAST));
}
