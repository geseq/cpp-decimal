#include "udecimal.hpp"

#include <cassert>
#include <cstdint>
#include <iostream>

using udecimal::Decimal;

void testBasic() {
    Decimal<> f0 = Decimal("123.456");
    Decimal<> f1 = Decimal("123.456");

    assert(f0 == f1);
    assert(f1 == f0);
    assert(f0.toInt() == 123);
    assert(f1.toInt() == 123);
    assert(f0.toString() == "123.456");
    assert(f1.toString() == "123.456");

    f0 = Decimal<>(1, 0);
    f1 = Decimal<>(.5) + (Decimal(.5));
    Decimal<> f2 = Decimal(.3) + Decimal(.3) + Decimal(.4);

    assert(f0 == f1);
    assert(f0 == f2);

    f0 = Decimal<>(.999);
    assert(f0.toString() == "0.999");
}

void testEqual() {
    Decimal<> f0 = udecimal::Zero;
    Decimal<> f1 = Decimal("123.456");

    assert(f0 != f1);
    assert(f0 != f1);
    assert(f1 != f0);

    f1 = udecimal::Zero;
    assert(f0 == f1);

    f0 = udecimal::Zero;
    assert(f0 == f1);
    assert(f0.toInt() == 0);
}

void testFromExp() {
    Decimal<> f = Decimal<>::FromExp(123, 1);
    assert(f.toString() == "1230");

    f = Decimal<>::FromExp(123, 0);
    assert(f.toString() == "123");

    f = Decimal<>::FromExp(123, -1);
    assert(f.toString() == "12.3");

    f = Decimal<>::FromExp(123456789001, -9);
    assert(f.toString() == "123.456789");

    f = Decimal<>::FromExp(123456789012, -9);
    assert(f.toString(7) == "123.4567890");

    f = Decimal<>::FromExp(123456789012, -9);
    assert(f.toString(8) == "123.45678901");
}

void testParse() {
    try {
        Decimal<>("123");
    } catch (std::invalid_argument& e) {
        assert(false);
    }

    try {
        auto val = Decimal<>(",123456");
        assert(false);
    } catch (std::invalid_argument& e) {
        // Expected
    }

    try {
        Decimal<>("abc");
        assert(false);
    } catch (std::invalid_argument& e) {
        // Expected
    }
}

void testNewI() {
    Decimal<> f = Decimal(123, 1);
    assert(f.toString() == "12.3");

    f = Decimal<>(123, 0);
    assert(f.toString() == "123");

    f = Decimal<>(123456789001, 9);
    assert(f.toString() == "123.456789");

    f = Decimal<>(123456789012, 9);
    assert(f.toString(7) == "123.4567890");
    assert(f.toString() == "123.45678901");

    f = Decimal<>(123456789012, 9);
    assert(f.toString(8) == "123.45678901");
}

void testMaxValue() {
    Decimal<> f0 = Decimal("12345678901");
    assert(f0.MAX == 99999999999.99999999);
    assert(f0.toString() == "12345678901");

    try {
        Decimal<>("123456789012");
        assert(false);
    } catch (std::overflow_error& e) {
        std::cout << e.what() << std::endl;
    }

    try {
        Decimal<>("-12345678901");
        assert(false);
    } catch (std::overflow_error& e) {
        std::cout << e.what() << std::endl;
    }

    try {
        Decimal<>("-123456789012");
        assert(false);
    } catch (std::overflow_error& e) {
        std::cout << e.what() << std::endl;
    }

    f0 = Decimal<>("99999999999");
    assert(f0.toString() == "99999999999");

    f0 = Decimal<>("9.99999999");
    assert(f0.toString() == "9.99999999");

    f0 = Decimal<>("99999999999.99999999");
    assert(f0.toString() == "99999999999.99999999");

    f0 = Decimal<>("99999999999.12345678901234567890");
    assert(f0.toString() == "99999999999.12345678");
}

void testDouble() {
    auto f0 = Decimal<>("123.456");
    auto f1 = Decimal<>(123.456);

    assert(f0 == f1);

    f1 = Decimal<>(0.0001);

    assert(f1.toString() == "0.0001");

    f1 = Decimal<>(".1");
    auto f2 = Decimal<>(Decimal(f1.Double()).toString());
    assert(f1 == f2);
}

void testInfinite() {
    auto f0 = Decimal<>("0.10");
    auto f1 = Decimal<>(0.10);

    assert(f0 == f1);

    auto f2 = Decimal<>(0.0);
    for (int i = 0; i < 3; i++) {
        f2 = f2 + Decimal<>(0.10);
    }
    assert(f2.toString() == "0.3");

    f2 = Decimal<>(0.0);
    for (int i = 0; i < 10; i++) {
        f2 = f2 + Decimal<>(0.10);
    }
    assert(f2.toString() == "1");
}

void testAddSub() {
    Decimal<> f0 = Decimal("1");
    Decimal<> f1 = Decimal("0.3333333");

    Decimal<> f2 = f0 - f1;
    f2 = f2 - f1;
    f2 = f2 - f1;

    assert(f2.toString() == "0.0000001");

    f2 = f2 - Decimal<>("0.0000001");
    assert(f2.toString() == "0");

    f0 = Decimal<>("0");
    for (int i = 0; i < 10; i++) {
        f0 = f0 + Decimal<>("0.1");
    }
    assert(f0.toString() == "1");
}

void testMulDiv() {
    Decimal<> f0("123.456");
    Decimal<> f1("1000");

    Decimal<> f2 = f0 * f1;
    assert(f2.toString() == "123456");

    f0 = Decimal<>("123456");
    f1 = Decimal<>("0.0001");

    f2 = f0 * f1;
    assert(f2.toString() == "12.3456");

    f0 = Decimal<>("10000.1");
    f1 = Decimal<>("10000");

    f2 = f0 * f1;
    assert(f2.toString() == "100001000");

    f2 = f2 / f1;
    assert(f2 == f0);

    f0 = Decimal<>("2");
    f1 = Decimal<>("3");

    f2 = f0 / f1;
    assert(f2.toString() == "0.66666667");

    f0 = Decimal<>("1000");
    f1 = Decimal<>("10");

    f2 = f0 / f1;
    assert(f2.toString() == "100");

    f0 = Decimal<>("1000");
    f1 = Decimal<>("0.1");

    f2 = f0 / f1;
    assert(f2.toString() == "10000");

    f0 = Decimal<>("1");
    f1 = Decimal<>("0.1");

    f2 = f0 * f1;
    assert(f2.toString() == "0.1");
}

void testNegatives() {
    try {
        Decimal<>("-1");
        assert(false);
    } catch (std::overflow_error& e) {
    }

    Decimal<> f0("99");
    Decimal<> f1("100");

    try {
        f0 - f1;
        assert(false);
    } catch (std::overflow_error& e) {
    }

    f0 = Decimal<>(".001");
    f1 = Decimal<>(".002");

    try {
        f0 - f1;
        assert(false);
    } catch (std::overflow_error& e) {
    }
}

void testOverflow() {
    auto f0 = Decimal<>(1.12345678);
    assert(f0.toString() == "1.12345678");

    f0 = Decimal<>(1.123456789123);
    assert(f0.toString() == "1.12345679");

    f0 = Decimal<>(1.0 / 3.0);
    assert(f0.toString() == "0.33333333");

    f0 = Decimal<>(2.0 / 3.0);
    assert(f0.toString() == "0.66666667");
}

void testNaN() {
    try {
        Decimal<>(std::nan(""));
        assert(false);
    } catch (std::invalid_argument& e) {
    }

    try {
        Decimal<>("NaN");
        assert(false);
    } catch (std::invalid_argument& e) {
    }

    Decimal<> f0("0.0004096");
    assert(f0.toString() == "0.0004096");
}

void testIntFrac() {
    auto f0 = Decimal<>(1234.5678);
    assert(f0.toInt() == 1234);
    assert(f0.toFrac() == .5678);
}

void testString() {
    auto f0 = Decimal<>(1234.5678);
    assert(f0.toString() == "1234.5678");

    f0 = Decimal<>(1234.0);
    assert(f0.toString() == "1234");

    f0 = Decimal<>("1.1");
    std::string s = f0.toString(2);

    assert(s == "1.10");

    f0 = Decimal<>("1");
    s = f0.toString(2);

    assert(s == "1.00");

    f0 = Decimal<>("1.123");
    s = f0.toString(2);

    assert(s == "1.12");

    f0 = Decimal<>("1.123");
    s = f0.toString(2);

    assert(s == "1.12");

    f0 = Decimal<>("1.123");
    s = f0.toString(0);

    assert(s == "1");

    s = f0.toString(10);
    assert(s == "1.12300000");

    f0 = Decimal<>(0.0);
    s = f0.toString(10);
    assert(s == "0.00000000");
}

void testRound() {
    Decimal<> f0 = Decimal("1.12345");
    Decimal<> f1 = f0.round(2);

    assert(f1.toString() == "1.12");

    f1 = f0.round(5);

    assert(f1.toString() == "1.12345");

    f1 = f0.round(4);

    assert(f1.toString() == "1.1235");
}

void testGeneralixedPlaces() {
    Decimal<9> f0 = Decimal<9>("9999999999.12345678901234567890");
    assert(f0.scale == 1000000000);
    assert(f0.MAX == 9999999999.999999999);
    assert(f0.zeros() == "000000000");
    assert(f0.toString() == "9999999999.123456789");

    Decimal<10> f1 = Decimal<10>("999999999.1234567891234567890");
    assert(f1.scale == 10000000000);
    assert(f1.MAX == 999999999.9999999999);
    assert(f1.zeros() == "0000000000");
    assert(f1.toString() == "999999999.1234567891");

    Decimal<11> f2 = Decimal<11>("99999999.12345678901234567890");
    assert(f2.scale == 100000000000);
    assert(f2.MAX == 99999999.99999999999);
    assert(f2.zeros() == "00000000000");
    assert(f2.toString() == "99999999.12345678901");

    Decimal<18> f3 = Decimal<18>("9.12345678901234567890");
    assert(f3.scale == 1000000000000000000);
    assert(f3.MAX == 9.999999999999999999);
    assert(f3.zeros() == "000000000000000000");
    assert(f3.toString() == "9.123456789012345678");

    Decimal<1> f4 = Decimal<1>("999999999999999999.12345678901234567890");
    assert(f4.scale == 10);
    assert(f4.MAX == 999999999999999999.9);
    assert(f4.zeros() == "0");
    assert(f4.toString() == "999999999999999999.1");
}

template <int nPlaces>
void testEncodeDecode() {
    udecimal::Decimal<nPlaces> original(123.456789);
    std::vector<uint8_t> binary_data = original.EncodeBinary();

    udecimal::Decimal<nPlaces> result;
    result.DecodeBinary(binary_data);

    assert(original == result);
}

template <int nPlaces>
void testDecodeBinaryData() {
    udecimal::Decimal<nPlaces> original(987.654321);
    std::vector<uint8_t> binary_data = original.EncodeBinary();
    std::vector<uint8_t> extra_data = {42, 24, 0};

    binary_data.insert(binary_data.end(), extra_data.begin(), extra_data.end());

    udecimal::Decimal<nPlaces> result;
    std::vector<uint8_t> remaining_data = result.DecodeBinaryData(binary_data);

    assert(original == result);
    assert(extra_data == remaining_data);
}

int main() {
    testBasic();
    testEqual();
    testFromExp();
    testParse();
    testNewI();
    testMaxValue();
    testDouble();
    testInfinite();
    testAddSub();
    testMulDiv();
    testNegatives();
    testOverflow();
    testNaN();
    testIntFrac();
    testString();
    testRound();
    testGeneralixedPlaces();

    testEncodeDecode<3>();
    testEncodeDecode<6>();
    testEncodeDecode<8>();
    testEncodeDecode<11>();

    testDecodeBinaryData<3>();
    testDecodeBinaryData<6>();
    testDecodeBinaryData<8>();
    testDecodeBinaryData<11>();

    return 0;
}
