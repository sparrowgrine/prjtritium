# frozen_string_literal: true

module EFXDBGen
  module OPH
    ##
    # EFLGen handles the creation of an EFL Bel
    # Input Pins: L4_I0,L4_I1,L4_I2,L4_I3,FA_I0,FA_I1,FA_CI
    # Output Pins: L4_O,FA_O,FA_CO
    #
    class EFLGen < BelCommon

      def generate
        # generate imuxes w/ connections directly to lut, as we need the EXPMUXes for the adder
        ipins_lut = make_imuxes_for_pins(0 => 'L4_I0', 1 => 'L4_I1', 2 => 'L4_I2', 3 => 'L4_I3')
        i0 = make_pin('FA_I0', BelPin::PinType::INPUT)
        i1 = make_pin('FA_I1', BelPin::PinType::INPUT)
        ci = make_pin('FA_CI', BelPin::PinType::INPUT)
        co = make_pin('FA_I0', BelPin::PinType::OUTPUT)
        opin_lut = make_pin('L4_O', BelPin::PinType::OUTPUT)
        opin_add = make_pin('FA_O', BelPin::PinType::OUTPUT)
        opin_mux = make_pip('OMUXCELL')
        link_termini(opin_lut, opin_mux)
        link_termini(opin_add, opin_mux)
        expmuxa = make_pip('EXPMUXA')
        expmuxb = make_pip('EXPMUXA')
        link_termini(ipins_lut[0],expmuxa)
        link_termini(ipins_lut[2],expmuxa)
        link_termini(ipins_lut[1],expmuxb)
        link_termini(ipins_lut[3],expmuxb)
        link_termini(expmuxa,i0)
        link_termini(expmuxb,i1)
        rtmuxns = make_pip('RTMUXNS')
        rtmuxew = make_pip('RTMUXEW')
        # inwires = ipins_lut.map(&:wires).flatten
        # inwires.each do |iwire|
        #   link_wire_to(iwire,rtmuxns)
        #   link_wire_to(iwire,rtmuxew)
        # end
        omuxshort = make_pip('OMUXSHORT')
        link_termini(rtmuxew,omuxshort)
        link_termini(rtmuxns,omuxshort)
        link_termini(opin_mux,omuxshort)
        link_to_wire(omuxshort,out_swire_for_dir(:NORTH))
        link_to_wire(omuxshort,out_swire_for_dir(:EAST))
        link_to_wire(omuxshort,out_swire_for_dir(:WEST))
        link_to_wire(omuxshort,out_swire_for_dir(:SOUTH))
        puts "omuxshort wires:"
        puts omuxshort.outputs.map(&:name)
        omuxnorth = make_pip('OMUXNORTH')
        link_termini(rtmuxns,omuxnorth)
        link_termini(opin_mux,omuxnorth)
        link_to_wire(omuxnorth,out_lwire_for_dir(:NORTH))
        omuxsouth = make_pip('OMUXSOUTH')
        link_termini(rtmuxns,omuxsouth)
        link_termini(opin_mux,omuxsouth)
        link_to_wire(omuxsouth,out_lwire_for_dir(:SOUTH))
        omuxwest = make_pip('OMUXWEST')
        link_termini(rtmuxns,omuxwest)
        link_termini(opin_mux,omuxwest)
        link_to_wire(omuxwest,out_lwire_for_dir(:WEST))
        omuxeast = make_pip('OMUXEAST')
        link_termini(rtmuxew,omuxeast)
        link_termini(opin_mux,omuxeast)
        link_to_wire(omuxeast,out_lwire_for_dir(:EAST))
      end
    end
  end
end
