# frozen_string_literal: true
module EFXDBGen
  module OPH
    class BelCommon
      def initialize(device, wbl, bel)
        @device = device
        @wbl = wbl
        @bel = bel
      end
      def link_wire_to(wire, tgt)
        if tgt.instance_of? Pip
          tgt.inputs << wire
          wire.sinks << Terminus.new(pip: tgt)
        elsif tgt.instance_of? BelPin
          tgt.wire = wire
          wire.sinks << Terminus.new(bp: tgt)
        end
      end
      def link_to_wire(src, wire)
        if src.instance_of? Pip
          src.outputs << wire
          wire.sources << Terminus.new(pip: src)
        elsif src.instance_of? BelPin
          src.wire = wire
          wire.sources << Terminus.new(bp: src)
        end
      end

      def link_termini(src, dst,loc = @bel.start)
        wire = make_intwire("#{src.name.split(':').last}:#{dst.name.split(':').last}",loc)
        link_to_wire(src, wire)
        link_wire_to(wire, dst)
      end

      def make_intwire(wirename,loc = @bel.start)
        wire = Wire.new(name: "INT:#{@bel.type}:X#{loc.x}Y#{loc.y}:#{wirename}", type: Wire::Type::INTERNAL)
        wire.start = @bel.start
        wire.end = @bel.end
        @device.wires << wire
        wire
      end

      def make_pip(pipname, loc = @bel.start)
        pip = Pip.new(name: "#{@bel.type}:X#{loc.x}Y#{loc.y}:#{pipname}")
        pip.position = @bel.start
        @device.pips << pip
        pip
      end

      def make_pin(pinname, type, loc = @bel.start)
        bp = BelPin.new(name: "#{@bel.type}:X#{loc.x}Y#{loc.y}:#{pinname}")
        bp.parent = @bel
        @bel.pins << bp
        bp.type = type
        bp
      end

      def out_lwire_for_dir(dir, loc = @bel.start)
        case dir
        when :NORTH
          @wbl[[loc.x,loc.y]].ytracks.find do |wire|
            wire.start == loc && wire.dir == dir && wire.name.start_with?('VL')
          end
        when :SOUTH
          tgtloc = Loc.new(x: loc.x, y: loc.y-1)
          @wbl[[tgtloc.x,tgtloc.y]].ytracks.find do |wire|
            wire.start == tgtloc && wire.dir == dir && wire.name.start_with?('VL')
          end
        when :EAST
          tgtloc = Loc.new(x: loc.x+1, y: loc.y-1)
          @wbl[[tgtloc.x,tgtloc.y]].xtracks.find do |wire|
            wire.start == tgtloc && wire.dir == dir && wire.name.start_with?('HL')
          end
        when :WEST
          tgtloc = Loc.new(x: loc.x, y: loc.y-1)
          @wbl[[tgtloc.x,tgtloc.y]].xtracks.find do |wire|
            wire.start == tgtloc && wire.dir == dir && wire.name.start_with?('HL')
          end
        end
      end

      def out_swire_for_dir(dir, loc = @bel.start)
        case dir
        when :NORTH
          @wbl[[loc.x,loc.y]].ytracks.find do |wire|
            wire.start == loc && wire.dir == dir && wire.name.start_with?('VS')
          end
        when :SOUTH
          tgtloc = Loc.new(x: loc.x, y: loc.y-1)
          @wbl[[tgtloc.x,tgtloc.y]].ytracks.find do |wire|
            wire.start == tgtloc && wire.dir == dir && wire.name.start_with?('VS')
          end
        when :EAST
          tgtloc = Loc.new(x: loc.x+1, y: loc.y-1)
          @wbl[[tgtloc.x,tgtloc.y]].xtracks.find do |wire|
            wire.start == tgtloc && wire.dir == dir && wire.name.start_with?('HS')
          end
        when :WEST
          tgtloc = Loc.new(x: loc.x, y: loc.y-1)
          @wbl[[tgtloc.x,tgtloc.y]].xtracks.find do |wire|
            wire.start == tgtloc && wire.dir == dir && wire.name.start_with?('HS')
          end
        end
      end

      def make_imuxes_for_pins(pinmap = {}, loc = @bel.start)
        pinmap.map do |muxid, pinname|
          imux = make_pip(OPHConsts::MUXNAMES[muxid],loc)
          bp = make_pin(pinname, BelPin::PinType::INPUT, loc)
          link_termini(imux, bp, loc)
          # OPHConsts::HTRACKIDS[muxid].each do |htrack|
          #   wire = @wbl[[loc.x, loc.y]].xtracks[htrack]
          #   puts wire
          #   link_wire_to(wire,imux)
          # end
          # OPHConsts::VTRACKIDS[muxid].each do |vtrack|
          #   wire = @wbl[[loc.x, loc.y - 1]].ytracks[vtrack]
          #   link_wire_to(wire, imux)
          # end
          bp
        end
      end
    end
  end
end
