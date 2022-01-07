//
// Created by nxmq0 on 1/6/2022.
//

#include "EFTIOGenerator.h"

void tritium::EFTIOGenerator::generate()
{
	auto &i0{make_pin("I0", BelPin::PinType::INPUT)};
	auto &i1{make_pin("I1", BelPin::PinType::INPUT)};
	auto &i2{make_pin("I2", BelPin::PinType::INPUT)};
	auto &i3{make_pin("I3", BelPin::PinType::INPUT)};

	auto &ipad{make_pin("IPAD", BelPin::PinType::INPUT)};
	auto &opad{make_pin("OPAD", BelPin::PinType::OUTPUT)};

	auto &imuxa{make_pip("IMUXA")};
	auto &imuxb{make_pip("IMUXB")};
	auto &imuxc{make_pip("IMUXC")};
	auto &imuxd{make_pip("IMUXD")};

	link_wires_to_imux_by_index(imuxa,BelCommon::IMUXIndex::IMUXA);
	link_wires_to_imux_by_index(imuxb,BelCommon::IMUXIndex::IMUXB);
	link_wires_to_imux_by_index(imuxc,BelCommon::IMUXIndex::IMUXC);
	link_wires_to_imux_by_index(imuxd,BelCommon::IMUXIndex::IMUXD);

	link_termini(imuxa,i0);
	link_termini(imuxb,i1);
	link_termini(imuxc,i2);
	link_termini(imuxd,i3);

	auto &rtmux0{make_pip("RTMUX0")};
	auto &rtmux1{make_pip("RTMUX1")};
	auto &rtmux2{make_pip("RTMUX2")};

	link_termini(ipad, rtmux0);
	link_termini(ipad, rtmux1);
	link_termini(ipad, rtmux2);
	link_termini(imuxa, rtmux0);
	link_termini(imuxa, rtmux1);
	link_termini(imuxa, rtmux2);
	link_termini(imuxb, rtmux0);
	link_termini(imuxb, rtmux1);
	link_termini(imuxb, rtmux2);
	link_termini(imuxc, rtmux0);
	link_termini(imuxc, rtmux1);
	link_termini(imuxc, rtmux2);
	link_termini(imuxd, rtmux0);
	link_termini(imuxd, rtmux1);
	link_termini(imuxd, rtmux2);

	auto &omuxshort{make_pip("OMUXSHORT")};

	link_termini(rtmux0, omuxshort);
	link_termini(rtmux1, omuxshort);
	link_termini(rtmux2, omuxshort);
    link_termini(ipad, omuxshort);

	link_to_wire(omuxshort, out_swire_for_dir(Wire::Direction::NORTH));
	link_to_wire(omuxshort, out_swire_for_dir(Wire::Direction::EAST));
	link_to_wire(omuxshort, out_swire_for_dir(Wire::Direction::WEST));
	link_to_wire(omuxshort, out_swire_for_dir(Wire::Direction::SOUTH));

	auto &omuxnorth{make_pip("OMUXNORTH")};
	link_termini(rtmux0, omuxnorth);
	link_termini(rtmux1, omuxnorth);
	link_to_wire(omuxnorth, out_lwire_for_dir(Wire::Direction::NORTH));

	auto &omuxsouth{make_pip("OMUXSOUTH")};
	link_termini(rtmux0, omuxsouth);
	link_termini(rtmux1, omuxsouth);
	link_to_wire(omuxsouth, out_lwire_for_dir(Wire::Direction::SOUTH));

	auto &omuxwest{make_pip("OMUXWEST")};
	link_termini(rtmux0, omuxwest);
	link_termini(rtmux2, omuxwest);
	link_to_wire(omuxwest, out_lwire_for_dir(Wire::Direction::WEST));

	auto &omuxeast{make_pip("OMUXEAST")};
	link_termini(rtmux0, omuxeast);
	link_termini(rtmux2, omuxeast);
	link_to_wire(omuxeast, out_lwire_for_dir(Wire::Direction::EAST));

	auto &opadmux{make_pip("OPADMUX")};
	link_termini(i0,opadmux);
	link_termini(i1,opadmux);
	link_termini(i2,opadmux);
	link_termini(i3,opadmux);
	link_termini(opadmux, opad);
}
