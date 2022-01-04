//
// Created by nxmq0 on 1/3/2022.
//

#ifndef LIBTRITIUM_BELCOMMON_H
#define LIBTRITIUM_BELCOMMON_H

#include "GridCell.h"
#include "types.h"

#include <fmt/core.h>

namespace data = cista::offset;
namespace tritium
{
	class BelCommon
	{
		Device &dev;
		std::unordered_map<vec2, GridCell> &wbl;
		Bel &bel;

	public:
		BelCommon(Device &dev, std::unordered_map<vec2, GridCell> &wbl, Bel &bel)
		: dev(dev), wbl(wbl), bel(bel)
		{
		}
		virtual void generate();

	protected:
		Pip &mk_pip(const data::string &name, vec2 loc);
		inline Pip &mk_pip(const data::string &name)
		{
			return mk_pip(name, bel.start);
		}
		Wire &mk_intwire(const data::string &name, vec2 loc);
		inline Wire &mk_intwire(const data::string &name)
		{
			return mk_intwire(name, bel.start);
		}
		BelPin &mk_pin(const data::string &name, BelPin::PinType type, vec2 loc);
		inline BelPin &mk_pin(const data::string &name, BelPin::PinType type)
		{
			return mk_pin(name, type, bel.start);
		}
		template<typename a, typename b>
		void lnk_termini(a &src, b &dst)
		{
			auto &wire = *dev.wires.emplace_back(data::make_unique<Wire>(Wire{}));
			wire.name.push_back(dev.id("INT").idx);
			wire.name.push_back(dev.id(bel.typestr()).idx);
			wire.name.push_back(
			  dev.id(fmt::format("X{}Y{}", src.loc.x, src.loc.y)).idx);
			wire.name.push_back(
			  dev.id(fmt::format("X{}Y{}", dst.loc.x, dst.loc.y)).idx);
			wire.type  = Wire::WireType::INTERNAL;
			wire.start = src.loc;
			wire.end   = dst.loc;
			lnk_to_wire(src, wire);
			lnk_wire_to(wire, dst);
		}
		static void lnk_wire_to(Wire &wire, Pip &tgt);
		static void lnk_wire_to(Wire &wire, BelPin &tgt);
		static void lnk_to_wire(Pip &src, Wire &wire);
		static void lnk_to_wire(BelPin &src, Wire &wire);
		Wire &out_lwire_for_dir(Wire::Direction dir, vec2 loc);
		inline Wire &out_lwire_for_dir(Wire::Direction dir)
		{
			return out_lwire_for_dir(dir, bel.start);
		}
		Wire &out_swire_for_dir(Wire::Direction dir, vec2 loc);
		inline Wire &out_swire_for_dir(Wire::Direction dir)
		{
			return out_swire_for_dir(dir, bel.start);
		}
	};
}

#endif // LIBTRITIUM_BELCOMMON_H
