# frozen_string_literal: true

require '../OPH/Device_pb'
Dir[File.dirname(__FILE__)+"/*.rb"].sort.each {|file| require file  unless file == __FILE__ }
module EFXDBGen
  module OPH
    class OPHGen
      attr_accessor :device, :wbl
      class GridCell
        attr_accessor :ytracks, :xtracks

        def initialize
          @ytracks = []
          @xtracks = []
        end
      end

      def initialize(name, grade, xdim, ydim)
        @device = Device.new
        @device.name = name
        @device.speedGrade = grade
        @device.dims = Loc.new(x: xdim, y: ydim)
        puts @device
        @ydim = ydim
        @xdim = xdim
        @wbl = {}
      end

      def localwire_substring_for_info(type, dir, start, endloc)
        case type
        when 'VL', 'VS'
          "#{type}:X#{start.x}:#{dir.to_s.chars[0]}:Y[#{start.y}:#{endloc.y}]"
        when 'HL', 'HS'
          "#{type}:Y#{start.y}:#{dir.to_s.chars[0]}:X[#{start.x}:#{endloc.x}]"
        end
      end

      def make_local_wire(type, dir, start, endloc, unknown, sbi)
        Wire.new(name: "#{localwire_substring_for_info(type, dir, start, endloc)}:#{unknown}:#{sbi}",
                 dir: dir,
                 start: start,
                 end: endloc,
                 type: Wire::Type::LOCAL,
                 unknown: unknown,
                 switchBoxIndex: sbi)
      end

      def generate_HL
        indcyc = (8..47).cycle
        (0..@ydim - 2).each do |y|
          pairindcyc = (8..47).to_a.rotate((-2 * y) + 2).cycle
          firstwire = 20 - (y % 20)
          (firstwire..20).each do |initline|
            @device.wires << make_local_wire('HL', :EAST, Loc.new(y: y, x: 1),
                                             Loc.new(y: y, x: initline), indcyc.next, pairindcyc.next)
            @device.wires << make_local_wire('HL', :WEST,
                                             Loc.new(y: y, x: initline),
                                             Loc.new(y: y, x: 1),
                                             indcyc.next,
                                             pairindcyc.next)
          end
          (1...firstwire).each do |line|
            @device.wires << make_local_wire('HL', :EAST,
                                             Loc.new(y: y, x: 1),
                                             Loc.new(y: y, x: line),
                                             indcyc.next,
                                             pairindcyc.next)
            @device.wires << make_local_wire('HL', :WEST,
                                             Loc.new(y: y, x: line),
                                             Loc.new(y: y, x: 1),
                                             indcyc.next,
                                             pairindcyc.next)
          end
          (y + 1).times.each do
            indcyc.next
            indcyc.next
          end
          (21..@xdim - 1).each do |line|
            @device.wires << make_local_wire('HL', :EAST,
                                             Loc.new(y: y, x: line - 19),
                                             Loc.new(y: y, x: line),
                                             indcyc.next,
                                             10)
            @device.wires << make_local_wire('HL', :WEST,
                                             Loc.new(y: y, x: line),
                                             Loc.new(y: y, x: line - 19),
                                             indcyc.next,
                                             11)
          end
          (@xdim - 19..@xdim - 1).each do |line|
            @device.wires << make_local_wire('HL', :EAST,
                                             Loc.new(y: y, x: line),
                                             Loc.new(y: y, x: @xdim - 1),
                                             indcyc.next,
                                             10)
            @device.wires << make_local_wire('HL', :WEST,
                                             Loc.new(y: y, x: @xdim - 1),
                                             Loc.new(y: y, x: line),
                                             indcyc.next,
                                             11)
          end
          indcyc = (8..47).cycle
          pairindcyc = (8..47).to_a.rotate((48 - 2 * (y % 20))).cycle
          20.times.each do |_line|
            @device.wires << make_local_wire('HL', :EAST,
                                             Loc.new(y: y, x: @xdim),
                                             Loc.new(y: y, x: @xdim),
                                             indcyc.next,
                                             pairindcyc.next)
            @device.wires << make_local_wire('HL', :WEST,
                                             Loc.new(y: y, x: @xdim),
                                             Loc.new(y: y, x: @xdim),
                                             indcyc.next,
                                             pairindcyc.next)
          end
        end
      end

      def generate_VL
        indcyc = (8..47).cycle
        (1..@xdim - 1).each do |x|
          pairindcyc = (8..47).to_a.rotate((-2 * x) + 2).cycle
          firstwire = 21 - (x % 20)
          (firstwire..20).each do |initline|
            @device.wires << make_local_wire('VL', :NORTH,
                                             Loc.new(x: x, y: 1),
                                             Loc.new(x: x, y: initline),
                                             indcyc.next,
                                             pairindcyc.next)
            @device.wires << make_local_wire('VL', :SOUTH,
                                             Loc.new(x: x, y: initline),
                                             Loc.new(x: x, y: 1),
                                             indcyc.next,
                                             pairindcyc.next)
          end
          (1...firstwire).each do |line|
            @device.wires << make_local_wire('VL', :NORTH,
                                             Loc.new(x: x, y: 1),
                                             Loc.new(x: x, y: line),
                                             indcyc.next,
                                             pairindcyc.next)
            @device.wires << make_local_wire('VL', :SOUTH,
                                             Loc.new(x: x, y: line),
                                             Loc.new(x: x, y: 1),
                                             indcyc.next,
                                             pairindcyc.next)
          end
          x.times.each do
            indcyc.next
            indcyc.next
          end
          (21..@ydim - 1).each do |line|
            @device.wires << make_local_wire('VL', :NORTH,
                                             Loc.new(x: x, y: line - 19),
                                             Loc.new(x: x, y: line),
                                             indcyc.next,
                                             8)
            @device.wires << make_local_wire('VL', :SOUTH,
                                             Loc.new(x: x, y: line),
                                             Loc.new(x: x, y: line - 19),
                                             indcyc.next,
                                             9)
          end
          (@ydim - 19..@ydim - 1).each do |line|
            @device.wires << make_local_wire('VL', :NORTH,
                                             Loc.new(x: x, y: line),
                                             Loc.new(x: x, y: @ydim - 1),
                                             indcyc.next,
                                             8)
            @device.wires << make_local_wire('VL', :SOUTH,
                                             Loc.new(x: x, y: @ydim - 1),
                                             Loc.new(x: x, y: line),
                                             indcyc.next,
                                             9)
          end
          indcyc = (8..47).cycle
          pairindcyc = (8..47).to_a.rotate((48 - 2 * ((x + 2) % 20))).cycle
          20.times.each do
            @device.wires << make_local_wire('VL', :NORTH,
                                             Loc.new(x: x, y: 0),
                                             Loc.new(x: x, y: 0),
                                             indcyc.next,
                                             pairindcyc.next)
            @device.wires << make_local_wire('VL', :SOUTH,
                                             Loc.new(x: x, y: 0),
                                             Loc.new(x: x, y: 0),
                                             indcyc.next,
                                             pairindcyc.next)
          end
        end
      end

      def generate_VS
        indcyc = (0..7).cycle
        (1..@xdim - 1).each do |x|
          pairindcyc = (0..7).to_a.rotate((-2 * x) + 2).cycle
          firstwire = 5 - (x % 4)
          (firstwire..4).each do |initline|
            @device.wires << make_local_wire('VS', :NORTH,
                                             Loc.new(x: x, y: 1),
                                             Loc.new(x: x, y: initline),
                                             indcyc.next,
                                             pairindcyc.next)
            @device.wires << make_local_wire('VS', :SOUTH,
                                             Loc.new(x: x, y: initline),
                                             Loc.new(x: x, y: 1),
                                             indcyc.next,
                                             pairindcyc.next)
          end
          (1...firstwire).each do |line|
            @device.wires << make_local_wire('VS', :NORTH,
                                             Loc.new(x: x, y: 1),
                                             Loc.new(x: x, y: line),
                                             indcyc.next,
                                             pairindcyc.next)
            @device.wires << make_local_wire('VS', :SOUTH,
                                             Loc.new(x: x, y: line),
                                             Loc.new(x: x, y: 1),
                                             indcyc.next,
                                             pairindcyc.next)
          end
          x.times.each do
            indcyc.next
            indcyc.next
          end
          (5..@ydim - 1).each do |line|
            @device.wires << make_local_wire('VS', :NORTH,
                                             Loc.new(x: x, y: line - 3),
                                             Loc.new(x: x, y: line),
                                             indcyc.next,
                                             0)
            @device.wires << make_local_wire('VS', :SOUTH,
                                             Loc.new(x: x, y: line),
                                             Loc.new(x: x, y: line - 3),
                                             indcyc.next,
                                             1)
          end
          (@ydim - 3..@ydim - 1).each do |line|
            @device.wires << make_local_wire('VS', :NORTH,
                                             Loc.new(x: x, y: line),
                                             Loc.new(x: x, y: @ydim - 1),
                                             indcyc.next,
                                             0)
            @device.wires << make_local_wire('VS', :SOUTH,
                                             Loc.new(x: x, y: @ydim - 1),
                                             Loc.new(x: x, y: line),
                                             indcyc.next,
                                             1)
          end
          indcyc = (0..7).cycle
          pairindcyc = (0..7).to_a.rotate((8 - 2 * ((x + 2) % 4))).cycle
          4.times.each do
            @device.wires << make_local_wire('VS', :NORTH,
                                             Loc.new(x: x, y: 0),
                                             Loc.new(x: x, y: 0),
                                             indcyc.next,
                                             pairindcyc.next)
            @device.wires << make_local_wire('VS', :SOUTH,
                                             Loc.new(x: x, y: 0),
                                             Loc.new(x: x, y: 0),
                                             indcyc.next,
                                             pairindcyc.next)
          end
        end
      end

      def generate_HS
        indcyc = (0..7).cycle
        (0..@ydim - 2).each do |y|
          pairindcyc = (0..7).to_a.rotate((-2 * y) + 2).cycle
          firstwire = 4 - (y % 4)
          (firstwire..4).each do |initline|
            @device.wires << make_local_wire('HS', :EAST,
                                             Loc.new(y: y, x: 1),
                                             Loc.new(y: y, x: initline),
                                             indcyc.next,
                                             pairindcyc.next)
            @device.wires << make_local_wire('HS', :WEST,
                                             Loc.new(y: y, x: initline),
                                             Loc.new(y: y, x: 1),
                                             indcyc.next,
                                             pairindcyc.next)
          end
          (1...firstwire).each do |line|
            @device.wires << make_local_wire('HS', :EAST,
                                             Loc.new(y: y, x: 1),
                                             Loc.new(y: y, x: line),
                                             indcyc.next,
                                             pairindcyc.next)
            @device.wires << make_local_wire('HS', :WEST,
                                             Loc.new(y: y, x: line),
                                             Loc.new(y: y, x: 1),
                                             indcyc.next,
                                             pairindcyc.next)
          end
          (y + 1).times.each do
            indcyc.next
            indcyc.next
          end
          (5..@xdim - 1).each do |line|
            @device.wires << make_local_wire('HS', :EAST,
                                             Loc.new(y: y, x: line - 3),
                                             Loc.new(y: y, x: line),
                                             indcyc.next,
                                             2)
            @device.wires << make_local_wire('HS', :WEST,
                                             Loc.new(y: y, x: line),
                                             Loc.new(y: y, x: line - 3),
                                             indcyc.next,
                                             3)
          end
          (@xdim - 3..@xdim - 1).each do |line|
            @device.wires << make_local_wire('HS', :EAST,
                                             Loc.new(y: y, x: line),
                                             Loc.new(y: y, x: @xdim - 1),
                                             indcyc.next,
                                             2)
            @device.wires << make_local_wire('HS', :WEST,
                                             Loc.new(y: y, x: @xdim - 1),
                                             Loc.new(y: y, x: line),
                                             indcyc.next,
                                             3)
          end
          indcyc = (0..7).cycle
          pairindcyc = (0..7).to_a.rotate((8 - 2 * (y % 4))).cycle
          4.times.each do
            @device.wires << make_local_wire('HS', :EAST,
                                             Loc.new(y: y, x: @xdim),
                                             Loc.new(y: y, x: @xdim),
                                             indcyc.next,
                                             pairindcyc.next)
            @device.wires << make_local_wire('HS', :WEST,
                                             Loc.new(y: y, x: @xdim),
                                             Loc.new(y: y, x: @xdim),
                                             indcyc.next,
                                             pairindcyc.next)
          end
        end
      end

      def generate_wires_by_loc
        (0..@ydim).each do |y|
          (0..@xdim).each do |x|
            @wbl[[x, y]] = GridCell.new
          end
        end
        @device.wires.each do |wire| # prepare to be cronching numbers for a HOT MINUTE
          case wire.dir
          when :NORTH
            (wire.start.y..wire.end.y).each do |y|
              @wbl[[wire.start.x, y]].ytracks[wire.unknown] = wire
            end
          when :SOUTH
            (wire.end.y..wire.start.y).each do |y|
              @wbl[[wire.start.x, y]].ytracks[wire.unknown] = wire
            end
          when :EAST
            (wire.start.x..wire.end.x).each do |x|
              @wbl[[x, wire.start.y]].xtracks[wire.unknown] = wire
            end
          when :WEST
          (wire.end.x..wire.start.x).each do |x|
            @wbl[[x, wire.start.y]].xtracks[wire.unknown] = wire
          end
          end
        end
      end

      def generate_bels(beldbpath)
        beldb = IO.read(beldbpath)
        belspecs = beldb.scan(/type: (\w+) x: (\d+) y: (\d+)/)
        puts belspecs.size
        belspecs.each do |belspec|
          bel = Bel.new
          bel.type = Bel::BelType.resolve(belspec[0].upcase.to_sym)
          bel.start = Loc.new(x: belspec[1].to_i, y: belspec[2].to_i)
          case bel.type
          when :IO
            outpad = BelPin.new(type: BelPin::PinType::INPUT, name: 'OPAD')
            outpad.wire = Wire.new(name: "IO:X#{bel.start.x}Y#{bel.start.y}:OPAD", type: Wire::Type::INTERBEL)
            outpad.parent = bel
            @device.wires << outpad.wire
            bel.pins << outpad
            outpad.wire.sinks << Terminus.new(bp: outpad)
            inpad = BelPin.new(type: BelPin::PinType::OUTPUT, name: 'IPAD')
            inpad.wire = Wire.new(name: "IO:X#{bel.start.x}Y#{bel.start.y}:IPAD", type: Wire::Type::INTERBEL)
            inpad.parent = bel
            @device.wires << inpad.wire
            outpad.wire.sources << Terminus.new(bp: inpad)
            bel.pins << inpad
          when :EFL
            bel.end = bel.start
            puts "generating efl at #{bel.start}!"
            EFLGen.new(@device, @wbl, bel).generate
          when :EFT
            bel.end = bel.start
            # TODO: CIN/COUT + CARRY CHAIN
          when :EFTIO
            bel.end = bel.start
            # if bel.start.x == 1
          when :MEMORY, :MULT
            bel.end = Loc.new(x: bel.start.x, y: bel.start.y + 19)
          end
          @device.bels << bel
        end
      end
      def write_chipdb(path)
        IO.binwrite(path, Device.encode(@device))
      end
    end
  end
end
  ophgen = EFXDBGen::OPH::OPHGen.new('oph_77x162_b3_d1', 'C3', 78, 163)
ophgen.generate_HL
ophgen.generate_VL
ophgen.generate_VS
ophgen.generate_HS
ophgen.generate_wires_by_loc
ophgen.generate_bels '../../beldb/oph_77x162_b3_d1.beldb'
ophgen.write_chipdb 'oph_77x162_b3_d1.chipdb'

