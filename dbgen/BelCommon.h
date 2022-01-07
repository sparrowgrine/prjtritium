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
		BelCommon(Device &dev, std::unordered_map<vec2, GridCell> &wbl, Bel &bel) : dev(dev), wbl(wbl), bel(bel) {}

		virtual void generate();

	protected:
		enum class IMUXIndex : uint8_t
		{
			UNKNOWN = 0,
			IMUXA   = 1,
			IMUXB   = 2,
			IMUXC   = 3,
			IMUXD   = 4,
			IMUXCE  = 5,
			IMUXSR  = 6
		};

		Pip &make_pip(const data::string &name, vec2 loc);
		inline Pip &make_pip(const data::string &name) { return make_pip(name, bel.start); }

		Wire &make_intwire(const data::string &name, vec2 loc);
		inline Wire &make_intwire(const data::string &name) { return make_intwire(name, bel.start); }

		BelPin &make_pin(const data::string &name, BelPin::PinType type, vec2 loc);
		inline BelPin &make_pin(const data::string &name, BelPin::PinType type)
		{
			return make_pin(name, type, bel.start);
		}

		template<typename T, typename U>
		void link_termini(T &src, U &dst)
		{
			auto &wire{*dev.wires.emplace_back(data::make_unique<Wire>(Wire{}))};
			wire.name.push_back(dev.id("INT"));
			wire.name.push_back(dev.id(bel.typestr()));
			wire.name.push_back(dev.id(fmt::format("X{}Y{}", src.loc.x, src.loc.y)));
			wire.name.push_back(dev.id(fmt::format("X{}Y{}", dst.loc.x, dst.loc.y)));
			wire.type  = Wire::WireType::INTERNAL;
			wire.start = src.loc;
			wire.end   = dst.loc;
			link_to_wire(src, wire);
			link_wire_to(wire, dst);
		}

		static void link_wire_to(Wire &wire, Pip &tgt);
		static void link_wire_to(Wire &wire, BelPin &tgt);

		static void link_to_wire(Pip &src, Wire &wire);
		static void link_to_wire(BelPin &src, Wire &wire);

		void link_wires_to_imux_by_index(Pip &imux, IMUXIndex idx);

		Wire &out_lwire_for_dir(Wire::Direction dir, vec2 loc);
		inline Wire &out_lwire_for_dir(Wire::Direction dir) { return out_lwire_for_dir(dir, bel.start); }

		Wire &out_swire_for_dir(Wire::Direction dir, vec2 loc);
		inline Wire &out_swire_for_dir(Wire::Direction dir) { return out_swire_for_dir(dir, bel.start); }
	};
}

#endif // LIBTRITIUM_BELCOMMON_H
