# Strong enum.

Strong enum is an attempt to create an enum like type that is easy to serialize/deserialize.

Strong enum instance underlying value can only be retrieved via its value method (no static_cast).
Strong enum instance can only be constructed from one of its value or from a raw value with a check to ensure it is a valid value.

##Typical enum class usage:

    enum class MyEnum
    {
        A,
        B,
        C,
    };

    // Accessing the underlying value of B.
    MyEnum e = MyEnum::B;
    std::cout << "Underlying value: " << static_cast<int>(e) << '\n';
    // Constructing MyEnum from raw value
    MyEnum fromRaw = static_cast<MyEnum>(2);
    std::cout << "Underlying value: " << static_cast<int>(fromRaw) << '\n';
    // Oops: construct MyEnum from invalid raw value. This should not be possible.
    MyEnum wrong = static_cast<MyEnum>(3);
    std::cout << "Underlying value: " << static_cast<int>(wrong) << '\n';

##Usage of strong_enum:

    namespace MyEnum {
        struct A : int_enum_value<0>{};
        struct B : int_enum_value<1>{};
        struct C : int_enum_value<2>{};
    }

    using MyEnum_t = strong_int_enum<A, B, C>;

    // Accessing the underlying value of B.
    MyEnum_t e = MyEnum::B{};
    std::cout << "Underlying value: " << e.value() << '\n';
    // Or, if enum value is known at compile type.
    std::cout << "Underlying value: " << B::value << '\n';
    // Construct MyEnum from raw value and print its value.
    MyEnum_t fromRaw = MyEnum_t::from_value(2);
    std::cout << "Underlying value: " << fromRaw.value() << '\n';
    // Try to construct MyEnum from invalid raw value.
    MyEnum wrong = MyEnum_t::from_value(3); // Fails if raw value is known at compile time or throws invalid_enum_value at runtime.

Note: The usage of ``namespace XXX`` for defining enum values avoid polluting parent namespace. The enum type can then be created as
``using XXX_t = strong_enum<...>``.