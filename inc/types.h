//
// Created by nxmq0 on 1/3/2022.
//

#ifndef LIBTRITIUM_TYPES_H
#define LIBTRITIUM_TYPES_H

#include <utility>

#include "cista.h"

#include <utility>

namespace data = cista::offset;

namespace tritium
{
	class BelPin;
	class Wire;
	class Pip;
	class Device;

	struct vec2
	{
		uint32_t x;
		uint32_t y;

		vec2 operator+(vec2 o) const { return {x + o.x, y + o.y}; }
		vec2 operator-(vec2 o) const { return {x - o.x, y - o.y}; }

		bool operator==(vec2 o) const { return x == o.x && y == o.y; }

		size_t hash() const { return cista::hash_combine(x, y); }
	};

#define TRITIUM_BEL_TYPE_MACRO(MACRO)                                                                                 \
	MACRO(EMPTY, 0)                                                                                                   \
	MACRO(IO, 1)                                                                                                      \
	MACRO(GBUF, 2)                                                                                                    \
	MACRO(GBUF_CTRL, 3)                                                                                               \
	MACRO(EFTIO, 4)                                                                                                   \
	MACRO(EFL, 5)                                                                                                     \
	MACRO(EFT, 6)                                                                                                     \
	MACRO(MEM, 7)                                                                                                     \
	MACRO(MULT, 8)

	struct Bel
	{
		enum class Type : uint32_t
		{
#define TRITIUM_DECLARE_BEL_TYPE(IDEN, VALUE) IDEN = VALUE,
			TRITIUM_BEL_TYPE_MACRO(TRITIUM_DECLARE_BEL_TYPE)
#undef TRITIUM_DECLARE_BEL_TYPE
		};

		data::vector<uint32_t> name;
		vec2 start;
		vec2 end;
		data::vector<data::unique_ptr<BelPin>> pins;
		Type type;

		[[nodiscard]] std::string_view typestr() const;
		std::string getName(Device &dev);
	};

	struct Wire
	{
		enum class Direction : uint32_t
		{
			UNDEF = 0,
			NORTH = 1,
			SOUTH = 2,
			EAST  = 3,
			WEST  = 4
		};

		enum class WireType : uint32_t
		{
			LOCAL    = 0,
			GLOBAL   = 1,
			INTERNAL = 2,
			INTERBEL = 3
		};

		data::vector<uint32_t> name;
		Direction dir;
		WireType type;
		vec2 start;
		vec2 end;
		float R;
		float C;
		uint32_t track;
		uint32_t sbi;
		data::vector<data::variant<data::ptr<BelPin>, data::ptr<Pip>>> sources;
		data::vector<data::variant<data::ptr<BelPin>, data::ptr<Pip>>> sinks;

		std::string getName(Device &dev);
	};

	struct Pip
	{
		data::vector<uint32_t> name;
		vec2 loc;
		data::vector<data::ptr<Wire>> inputs;
		data::vector<data::ptr<Wire>> outputs;
	};

	struct BelPin
	{
		enum class PinType : uint32_t
		{
			UNDEF  = 0,
			INPUT  = 1,
			OUTPUT = 2,
			INOUT  = 3
		};

		data::vector<uint32_t> name;
		vec2 loc;
		data::ptr<Wire> wire;
		PinType type;
	};

	struct Device
	{
		vec2 dims;
		data::string name;
		data::string speedGrade;
		data::vector<data::unique_ptr<tritium::Bel>> bels;
		data::vector<data::unique_ptr<tritium::Wire>> wires;
		data::vector<data::unique_ptr<tritium::Pip>> pips;
		data::hash_map<data::string, uint32_t> idstring_str_to_idx;
		data::vector<data::string> idstring_idx_to_str;

	public:
		Bel &make_bel(Bel::Type type, vec2 start);
		uint32_t id(std::string_view str);
		const data::string &str(uint32_t id);
	};
}
template<>
struct std::hash<tritium::vec2>
{
	size_t operator()(const tritium::vec2 &v) const noexcept { return cista::hash_combine(v.x, v.y); };
};

#endif // LIBTRITIUM_TYPES_H
