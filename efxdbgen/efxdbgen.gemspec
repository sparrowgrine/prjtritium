Gem::Specification.new do |s|
  s.name        = 'efxdbgen'
  s.version     = '0.0.0'
  s.summary     = 'ChipDB Generator for Efinix FPGAs'
  s.description = 'efxdbgen contains the routines for generating the ChipDB files used by prjtritium to describe the FPGA fabrics of Efinix devices to nextpnr.'
  s.authors     = ['Natalia Machado']
  s.email       = 'nat@catgirl.ee'
  s.files       = %w[lib/OPH/OPHGen.rb lib/OPH/Device_pb.rb]
  s.homepage    = 'https://rubygems.org/gems/efxdbgen'
  s.license     = 'MIT'
  s.add_runtime_dependency 'fast_trie', '0.5.1'
	s.add_runtime_dependency 'google/protobuf', '3.19.1'

end

