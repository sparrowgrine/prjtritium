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
        i0 = mk_pin('FA_I0', BelPin::PinType::INPUT)
        i1 = mk_pin('FA_I1', BelPin::PinType::INPUT)
        ci = mk_pin('FA_CI', BelPin::PinType::INPUT)
        co = mk_pin('FA_I0', BelPin::PinType::OUTPUT)
        opin_lut = mk_pin('L4_O', BelPin::PinType::OUTPUT)
        opin_add = mk_pin('FA_O', BelPin::PinType::OUTPUT)
        opin_mux = mk_pip('OMUXCELL')
        lnk_termini(opin_lut, opin_mux)
        lnk_termini(opin_add, opin_mux)
        expmuxa = mk_pip('EXPMUXA')
        expmuxb = mk_pip('EXPMUXA')
        lnk_termini(ipins_lut[0],expmuxa)
        lnk_termini(ipins_lut[2],expmuxa)
        lnk_termini(ipins_lut[1],expmuxb)
        lnk_termini(ipins_lut[3],expmuxb)
        lnk_termini(expmuxa,i0)
        lnk_termini(expmuxb,i1)
        rtmuxns = mk_pip('RTMUXNS')
        rtmuxew = mk_pip('RTMUXEW')
        # inwires = ipins_lut.map(&:wires).flatten
        # inwires.each do |iwire|
        #   lnk_wire_to(iwire,rtmuxns)
        #   lnk_wire_to(iwire,rtmuxew)
        # end
        omuxshort = mk_pip('OMUXSHORT')
        lnk_termini(rtmuxew,omuxshort)
        lnk_termini(rtmuxns,omuxshort)
        lnk_termini(opin_mux,omuxshort)
        lnk_to_wire(omuxshort,out_swire_for_dir(:NORTH))
        lnk_to_wire(omuxshort,out_swire_for_dir(:EAST))
        lnk_to_wire(omuxshort,out_swire_for_dir(:WEST))
        lnk_to_wire(omuxshort,out_swire_for_dir(:SOUTH))
        puts "omuxshort wires:"
        puts omuxshort.outputs.map(&:name)
        omuxnorth = mk_pip('OMUXNORTH')
        lnk_termini(rtmuxns,omuxnorth)
        lnk_termini(opin_mux,omuxnorth)
        lnk_to_wire(omuxnorth,out_lwire_for_dir(:NORTH))
        omuxsouth = mk_pip('OMUXSOUTH')
        lnk_termini(rtmuxns,omuxsouth)
        lnk_termini(opin_mux,omuxsouth)
        lnk_to_wire(omuxsouth,out_lwire_for_dir(:SOUTH))
        omuxwest = mk_pip('OMUXWEST')
        lnk_termini(rtmuxns,omuxwest)
        lnk_termini(opin_mux,omuxwest)
        lnk_to_wire(omuxwest,out_lwire_for_dir(:WEST))
        omuxeast = mk_pip('OMUXEAST')
        lnk_termini(rtmuxew,omuxeast)
        lnk_termini(opin_mux,omuxeast)
        lnk_to_wire(omuxeast,out_lwire_for_dir(:EAST))
      end
    end
  end
end
