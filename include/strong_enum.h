//
// Created by simon on 9/11/17.
//

#ifndef STRONG_ENUM_STRONG_ENUM_H
#define STRONG_ENUM_STRONG_ENUM_H

#include <cstdint>
#include <type_traits>
#include <exception>

namespace se
{

namespace traits
{

/**
 * Trait to check if a type is contained in a variadic pack.
 * Usage:
 * 	contains_type<T, Ts...> is true_type if T is in Ts...
 * 	contains_type<bool, int, char, std::string, bool>::value == true // bool is in Ts.
 * 	contains_type<bool, int, char, std::string, long>::value == false. // bool is not in Ts.
 * @tparam Tp The type to search.
 * @tparam List The type list.
 */
template<typename Tp, typename... List>
struct contains_type;

template<typename Tp, typename Head, typename... Rest>
struct contains_type<Tp, Head, Rest...>
	: std::conditional<
		std::is_same<Tp, Head>::value,
		std::true_type,
		contains_type<Tp, Rest...>
	>::type
{
};

template<typename Tp>
struct contains_type<Tp> : std::false_type{};

/**
 * Functor trait used to check if a variadic pack of integral values contains a type with a given value.
 * Usage:
 * 	contains_value<Values...>{}(v) is true if v is a value of Values... types.
 * 	contains_value<std::integral_constant<int, 5>>{}(5) is true because integral contant has value 5.
 * 	contains_value<std::integral_constant<char, 's'>>{}('a') is false because integral constant does not have value 'a'.
 * @tparam Values
 */
template <typename... Values>
struct contains_value;

template <>
struct contains_value<>
{
	template <typename ValueType>
	constexpr bool operator()(ValueType v) const noexcept
	{
		return false;
	}
};

template <typename V, typename... Vs>
struct contains_value<V, Vs...>
{
	template <typename ValueType>
	constexpr bool operator()(ValueType v) const noexcept
	{
		return v == V::value || contains_value<Vs...>{}(v);
	}
};

} // traits

/**
 * Exception thrown when constructing a strong_enum from a value (i.e. using strong_enum::from_value) that is not in the defined strong_enum Values...
 */
struct invalid_enum_value : std::exception
{
	const char* what() const noexcept override
	{
		return "invalid enum value";
	}
};

template <typename T, T V>
using enum_value = std::integral_constant<T, V>;
template <int V>
using int_enum_value = std::integral_constant<int, V>;
template <uint8_t V>
using byte_enum_value = std::integral_constant<uint8_t, V>;

/**
 * Strong enum is an attempt to create an enum like type that is easy to serialize/deserialize.
 *
 * Strong enum instance underlying value can only be retrieved via its value method (no static_cast).
 * Strong enum instance can only be constructed from one of its value or from a raw value with a check to ensure it is a valid value.
 *
 * Usage:
 * namespace MyEnum {
 *    struct A : int_enum_value<0>{};
 *    struct B : int_enum_value<1>{};
 *    struct C : int_enum_value<2>{};
 *  }
 *
 *  using MyEnum_t = strong_int_enum<A, B, C>;
 *
 * @tparam ValueType The type used to store the underlying values.
 * @tparam Values The available values.
 */
template<typename ValueType, typename... Values>
class strong_enum
{
public:
	using value_type = ValueType;

	template<typename T>
	constexpr strong_enum(T, typename std::enable_if<traits::contains_type<T, Values...>::value>::type* = nullptr):
		m_value{T::value} {}

	static constexpr strong_enum from_value(ValueType v)
	{
		if (!traits::contains_value<Values...>{}(v))
		{
			throw invalid_enum_value{};
		}
		return strong_enum{v};
	}

	constexpr ValueType value() const noexcept { return m_value; }

	constexpr bool operator==(strong_enum other) const noexcept { return m_value == other.value(); }
	constexpr bool operator!=(strong_enum other) const noexcept { return m_value != other.value(); }

private:
	constexpr explicit strong_enum(ValueType v):
		m_value{v}
	{}

	ValueType m_value;
};

template <typename... Values>
using strong_int_enum = strong_enum<int, Values...>;
template <typename... Values>
using strong_byte_enum = strong_enum<uint8_t, Values...>;

template<typename T, typename ValueType, typename... Values>
constexpr inline bool operator==(const strong_enum<ValueType, Values...>& enumValue, T)
{
	static_assert(traits::contains_type<T, Values...>::value, "Cannot compare enum with values outside of its type.");
	return enumValue.value() == T::value;
};

template<typename T, typename ValueType, typename... Values>
constexpr inline bool operator==(T, const strong_enum<ValueType, Values...>& enumValue)
{
	return enumValue == T{};
};

template<typename T, typename ValueType, typename... Values>
constexpr inline bool operator!=(const strong_enum<ValueType, Values...>& enumValue, T)
{
	return !(enumValue == T{});
};

template<typename T, typename ValueType, typename... Values>
constexpr inline bool operator!=(T, const strong_enum<ValueType, Values...>& enumValue)
{
	return !(enumValue == T{});
};

}


#endif //STRONG_ENUM_STRONG_ENUM_H
