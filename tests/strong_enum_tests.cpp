#include "../include/strong_enum.h"
#include "catch.hpp"

#include <string>

using namespace se;
namespace {

struct enum_value1 : int_enum_value<1> {};
struct enum_value2 : int_enum_value<2> {};
using test_enum = strong_enum<int, enum_value1, enum_value2>;

}

TEST_CASE("Contains type trait can be used to check if a type is in a type list.", "[.compile-time]")
{
	static_assert(traits::contains_type<bool, bool, char, int, long>::value);
	static_assert(traits::contains_type<char, bool, char, int, long>::value);
	static_assert(traits::contains_type<int, bool, char, int, long>::value);
	static_assert(traits::contains_type<long, bool, char, int, long>::value);
	static_assert(!traits::contains_type<std::string, bool, char, int, long>::value);
	static_assert(!traits::contains_type<bool>::value);
}

TEST_CASE("Contains value trait can be used to check if a type .", "[.compile-time]")
{
	static_assert(traits::contains_value<enum_value1, enum_value2>{}(1));
	static_assert(traits::contains_value<enum_value1, enum_value2>{}(2));
	static_assert(!traits::contains_value<enum_value1, enum_value2>{}(3));
	static_assert(!traits::contains_value<enum_value1, enum_value2>{}(0));
	static_assert(!traits::contains_value<enum_value1, enum_value2>{}(-1));
}

TEST_CASE("Strong enum can only be constructed from its values.", "[.compile-time]")
{
	//test_enum{0}; //Does not compile.
	//test_enum{std::integral_constant<int, 1>{}}; //Does not compile.
	constexpr auto ev1 = test_enum{enum_value1{}};
	constexpr auto ev2 = test_enum{enum_value2{}};
	static_assert(ev1 == test_enum{enum_value1{}});
	static_assert(ev1 != ev2);
	static_assert(enum_value1{} == ev1);
	static_assert(ev1 != enum_value2{});
	static_assert(enum_value2{} != ev1);
	//static_assert(ev1 == 1); // does not compile.
	//static_assert(ev1 == (std::integral_constant<int, 1>{})); // does not compile.
}


TEST_CASE("Strong enum can be constructed from value type if it contains the given value.", "[.compile-time]")
{
	constexpr auto efv1 = test_enum::from_value(1);
	static_assert(efv1 == enum_value1{});
	//constexpr auto efv3 = test_enum::from_value(3); // does not compile.
}

TEST_CASE("Strong enum throws exception if constructed with a value outside of its enum values.")
{
	REQUIRE_THROWS_AS(test_enum::from_value(3), invalid_enum_value);
}