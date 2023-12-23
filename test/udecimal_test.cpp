#include "udecimal.hpp"

#include <gtest/gtest.h>

#include <cassert>
#include <cstdint>
#include <cwchar>
#include <iostream>

using udecimal::Decimal;

class DecimalTest : public ::testing::Test {
   protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(DecimalTest, BasicU8) {
    udecimal::U8 f0 = udecimal::U8("123.456");
    udecimal::U8 f1 = udecimal::U8("123.456");

    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f1, f0);
    ASSERT_EQ(f0.to_int(), 123);
    ASSERT_EQ(f1.to_int(), 123);
    ASSERT_EQ(f0.to_string(), "123.456");
    ASSERT_EQ(f1.to_string(), "123.456");

    f0 = udecimal::U8(1, 0);
    f1 = udecimal::U8(.5) + (udecimal::U8(.5));
    udecimal::U8 f2 = udecimal::U8(.3) + udecimal::U8(.3) + udecimal::U8(.4);

    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f0, f2);

    f0 = udecimal::U8(.999);
    ASSERT_EQ(f0.to_string(), "0.999");
}

TEST_F(DecimalTest, BasicI8) {
    udecimal::I8 f0 = udecimal::I8("-123.456");
    udecimal::I8 f1 = udecimal::I8("-123.456");

    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f1, f0);
    ASSERT_EQ(f0.to_int(), -123);
    ASSERT_EQ(f1.to_int(), -123);
    ASSERT_EQ(f0.to_string(), "-123.456");
    ASSERT_EQ(f1.to_string(), "-123.456");

    f0 = udecimal::I8("123.456");
    f1 = udecimal::I8("123.456");

    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f1, f0);
    ASSERT_EQ(f0.to_int(), 123);
    ASSERT_EQ(f1.to_int(), 123);
    ASSERT_EQ(f0.to_string(), "123.456");
    ASSERT_EQ(f1.to_string(), "123.456");

    f0 = udecimal::I8(-1, 0);
    f1 = udecimal::I8(-.5) + udecimal::I8(-.5);
    udecimal::I8 f2 = udecimal::I8(-.3) + udecimal::I8(-.3) + udecimal::I8(-.4);

    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f0, f2);

    f0 = udecimal::I8(-.999);
    ASSERT_EQ(f0.to_string(), "-0.999");

    f0 = udecimal::I8(1, 0);
    f1 = udecimal::I8(.5) + (udecimal::I8(.5));
    f2 = udecimal::I8(.3) + udecimal::I8(.3) + udecimal::I8(.4);

    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f0, f2);

    f0 = udecimal::I8(.999);
    ASSERT_EQ(f0.to_string(), "0.999");
}

TEST_F(DecimalTest, EqualU8) {
    udecimal::U8 f0{};
    udecimal::U8 f1("123.456");

    ASSERT_NE(f0, f1);
    ASSERT_NE(f1, f0);

    f1 = udecimal::U8(0, 0);
    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f0.to_int(), 0);
}

TEST_F(DecimalTest, EqualI8) {
    udecimal::I8 f0{};
    udecimal::I8 f1("123.456");
    udecimal::I8 f2("-123.456");

    ASSERT_NE(f0, f1);
    ASSERT_NE(f0, f2);
    ASSERT_NE(f1, f0);
    ASSERT_NE(f1, f2);
    ASSERT_NE(f2, f0);
    ASSERT_NE(f2, f1);

    f1 = udecimal::I8(0, 0);
    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f0.to_int(), 0);

    f1 = udecimal::I8(-123456, 3);
    ASSERT_EQ(f2, f1);
    ASSERT_EQ(f2.to_int(), -123);
}

TEST_F(DecimalTest, FromExpU8) {
    auto f = udecimal::U8::FromExp(123, 1);
    ASSERT_EQ(f.to_string(), "1230");

    f = udecimal::U8::FromExp(123, 0);
    ASSERT_EQ(f.to_string(), "123");

    f = udecimal::U8::FromExp(123, -1);
    ASSERT_EQ(f.to_string(), "12.3");

    f = udecimal::U8::FromExp(123456789001, -9);
    ASSERT_EQ(f.to_string(), "123.456789");

    f = udecimal::U8::FromExp(123456789012, -9);
    ASSERT_EQ(f.to_string(7), "123.4567890");

    f = udecimal::U8::FromExp(123456789012, -9);
    ASSERT_EQ(f.to_string(8), "123.45678901");
}

TEST_F(DecimalTest, FromExpI8) {
    auto f = udecimal::I8::FromExp(123, 1);
    ASSERT_EQ(f.to_string(), "1230");

    f = udecimal::I8::FromExp(123, 0);
    ASSERT_EQ(f.to_string(), "123");

    f = udecimal::I8::FromExp(123, -1);
    ASSERT_EQ(f.to_string(), "12.3");

    f = udecimal::I8::FromExp(123456789001, -9);
    ASSERT_EQ(f.to_string(), "123.456789");

    f = udecimal::I8::FromExp(123456789012, -9);
    ASSERT_EQ(f.to_string(7), "123.4567890");

    f = udecimal::I8::FromExp(123456789012, -9);
    ASSERT_EQ(f.to_string(8), "123.45678901");

    f = udecimal::I8::FromExp(-123, 1);
    ASSERT_EQ(f.to_string(), "-1230");

    f = udecimal::I8::FromExp(-123, 0);
    ASSERT_EQ(f.to_string(), "-123");

    f = udecimal::I8::FromExp(-123, -1);
    ASSERT_EQ(f.to_string(), "-12.3");

    f = udecimal::I8::FromExp(-123456789001, -9);
    ASSERT_EQ(f.to_string(), "-123.456789");

    f = udecimal::I8::FromExp(-123456789012, -9);
    ASSERT_EQ(f.to_string(7), "-123.4567890");

    f = udecimal::I8::FromExp(-123456789012, -9);
    ASSERT_EQ(f.to_string(8), "-123.45678901");
}

TEST_F(DecimalTest, Parse) {
    ASSERT_NO_THROW(udecimal::U8("123"));
    ASSERT_NO_THROW(udecimal::I8("123"));

    ASSERT_THROW(udecimal::U8(",123456"), std::invalid_argument);
    ASSERT_THROW(udecimal::I8(",123456"), std::invalid_argument);
    ASSERT_THROW(udecimal::U8("abc"), std::invalid_argument);
    ASSERT_THROW(udecimal::I8("abc"), std::invalid_argument);
}

TEST_F(DecimalTest, NewIU8) {
    udecimal::U8 f = udecimal::U8(123, 1);
    ASSERT_EQ(f.to_string(), "12.3");

    f = udecimal::U8(123, 0);
    ASSERT_EQ(f.to_string(), "123");

    f = udecimal::U8(123, 5);
    ASSERT_EQ(f.to_string(), "0.00123");

    f = udecimal::U8(123456789001, 9);
    ASSERT_EQ(f.to_string(), "123.456789");

    f = udecimal::U8(123456789012, 9);
    ASSERT_EQ(f.to_string(7), "123.4567890");
    ASSERT_EQ(f.to_string(), "123.45678901");

    f = udecimal::U8(123456789012, 9);
    ASSERT_EQ(f.to_string(8), "123.45678901");

    auto g = Decimal<3>(123, 5);
    ASSERT_EQ(g.to_string(), "0.001");
}

TEST_F(DecimalTest, NewII8) {
    udecimal::I8 f = udecimal::I8(123, 1);
    ASSERT_EQ(f.to_string(), "12.3");

    f = udecimal::I8(123, 0);
    ASSERT_EQ(f.to_string(), "123");

    f = udecimal::I8(123, 5);
    ASSERT_EQ(f.to_string(), "0.00123");

    f = udecimal::I8(123456789001, 9);
    ASSERT_EQ(f.to_string(), "123.456789");

    f = udecimal::I8(123456789012, 9);
    ASSERT_EQ(f.to_string(7), "123.4567890");
    ASSERT_EQ(f.to_string(), "123.45678901");

    f = udecimal::I8(123456789012, 9);
    ASSERT_EQ(f.to_string(8), "123.45678901");

    f = udecimal::I8(-123, 1);
    ASSERT_EQ(f.to_string(), "-12.3");

    f = udecimal::I8(-123, 0);
    ASSERT_EQ(f.to_string(), "-123");

    f = udecimal::I8(-123, 5);
    ASSERT_EQ(f.to_string(), "-0.00123");

    f = udecimal::I8(-123456789001, 9);
    ASSERT_EQ(f.to_string(), "-123.456789");

    f = udecimal::I8(-123456789012, 9);
    ASSERT_EQ(f.to_string(7), "-123.4567890");
    ASSERT_EQ(f.to_string(), "-123.45678901");

    f = udecimal::I8(-123456789012, 9);
    ASSERT_EQ(f.to_string(8), "-123.45678901");

    auto g = Decimal<3, udecimal::Signed>(123, 5);
    ASSERT_EQ(g.to_string(), "0.001");

    g = Decimal<3, udecimal::Signed>(-123, 5);
    ASSERT_EQ(g.to_string(), "-0.001");
}

TEST_F(DecimalTest, MaxValueU8) {
    udecimal::U8 f0 = udecimal::U8("12345678901");
    ASSERT_EQ(f0.MAX, 99999999999.99999999);
    ASSERT_EQ(f0.to_string(), "12345678901");

    ASSERT_THROW(udecimal::U8("123456789012"), std::overflow_error);
    ASSERT_THROW(udecimal::U8("-12345678901"), std::overflow_error);
    ASSERT_THROW(udecimal::U8("-123456789012"), std::overflow_error);

    f0 = udecimal::U8("99999999999");
    ASSERT_EQ(f0.to_string(), "99999999999");

    f0 = udecimal::U8("9.99999999");
    ASSERT_EQ(f0.to_string(), "9.99999999");

    f0 = udecimal::U8("99999999999.99999999");
    ASSERT_EQ(f0.to_string(), "99999999999.99999999");

    f0 = udecimal::U8("99999999999.12345678901234567890");
    ASSERT_EQ(f0.to_string(), "99999999999.12345678");
}

TEST_F(DecimalTest, MaxValueI8) {
    udecimal::I8 f0 = udecimal::I8("1234567890");
    ASSERT_EQ(f0.MAX, 9999999999.99999999);
    ASSERT_EQ(f0.to_string(), "1234567890");

    ASSERT_THROW(udecimal::I8("12345678901"), std::overflow_error);
    ASSERT_THROW(udecimal::I8("123456789012"), std::overflow_error);

    f0 = udecimal::I8("9999999999");
    ASSERT_EQ(f0.to_string(), "9999999999");

    f0 = udecimal::I8("9.99999999");
    ASSERT_EQ(f0.to_string(), "9.99999999");

    f0 = udecimal::I8("9999999999.99999999");
    ASSERT_EQ(f0.to_string(), "9999999999.99999999");

    f0 = udecimal::I8("9999999999.12345678901234567890");
    ASSERT_EQ(f0.to_string(), "9999999999.12345678");

    f0 = udecimal::I8("-1234567890");
    ASSERT_EQ(f0.MAX, 9999999999.99999999);
    ASSERT_EQ(f0.to_string(), "-1234567890");

    ASSERT_THROW(udecimal::I8("-12345678901"), std::overflow_error);
    ASSERT_THROW(udecimal::I8("-123456789012"), std::overflow_error);

    f0 = udecimal::I8("-9999999999");
    ASSERT_EQ(f0.to_string(), "-9999999999");

    f0 = udecimal::I8("-9.99999999");
    ASSERT_EQ(f0.to_string(), "-9.99999999");

    f0 = udecimal::I8("-9999999999.99999999");
    ASSERT_EQ(f0.to_string(), "-9999999999.99999999");

    f0 = udecimal::I8("-9999999999.12345678901234567890");
    ASSERT_EQ(f0.to_string(), "-9999999999.12345678");
}

void testToDouble() {
    auto f0 = udecimal::U8("123.456");
    auto f1 = udecimal::U8(123.456);

    assert(f0 == f1);

    f1 = udecimal::U8(0.0001);

    assert(f1.to_string() == "0.0001");

    f1 = udecimal::U8(".1");
    assert(f1.to_string() == "0.1");

    auto f2 = udecimal::U8(udecimal::U8(f1.to_double()).to_string());
    assert(f1 == f2);
}

void testInfinite() {
    auto f0 = udecimal::U8("0.10");
    auto f1 = udecimal::U8(0.10);

    assert(f0 == f1);

    auto f2 = udecimal::U8(0.0);
    for (int i = 0; i < 3; i++) {
        f2 = f2 + udecimal::U8(0.10);
    }
    assert(f2.to_string() == "0.3");

    f2 = udecimal::U8(0.0);
    for (int i = 0; i < 10; i++) {
        f2 = f2 + udecimal::U8(0.10);
    }
    assert(f2.to_string() == "1");
}

void testAddSub() {
    udecimal::U8 f0 = udecimal::U8("1");
    udecimal::U8 f1 = udecimal::U8("0.3333333");

    udecimal::U8 f2 = f0 - f1;
    f2 = f2 - f1;
    f2 -= f1;

    assert(f2.to_string() == "0.0000001");

    f2 = f2 - udecimal::U8("0.0000001");
    assert(f2.to_string() == "0");

    f0 = udecimal::U8("0");
    for (int i = 0; i < 5; i++) {
        f0 = f0 + udecimal::U8("0.1");
        f0 += udecimal::U8("0.1");
    }

    assert(f0.to_string() == "1");
}

void testMulDiv() {
    udecimal::U8 f0("123.456");
    udecimal::U8 f1("1000");

    udecimal::U8 f2 = f0 * f1;
    assert(f2.to_string() == "123456");

    f0 = udecimal::U8("123456");
    f1 = udecimal::U8("0.0001");

    f2 = f0 * f1;
    assert(f2.to_string() == "12.3456");

    f0 = udecimal::U8("10000.1");
    f1 = udecimal::U8("10000");

    f2 = f0 * f1;
    assert(f2.to_string() == "100001000");

    f2 = f2 / f1;
    assert(f2 == f0);

    f2 *= f1;
    assert(f2.to_string() == "100001000");

    f2 /= f1;
    assert(f2 == f0);

    f0 = udecimal::U8("2");
    f1 = udecimal::U8("3");

    f2 = f0 / f1;
    assert(f2.to_string() == "0.66666667");

    f0 = udecimal::U8("1000");
    f1 = udecimal::U8("10");

    f2 = f0 / f1;
    assert(f2.to_string() == "100");

    f0 = udecimal::U8("1000");
    f1 = udecimal::U8("0.1");

    f2 = f0 / f1;
    assert(f2.to_string() == "10000");

    f0 = udecimal::U8("1");
    f1 = udecimal::U8("0.1");

    f2 = f0 * f1;
    assert(f2.to_string() == "0.1");
}

void testNegatives() {
    try {
        udecimal::U8("-1");
        assert(false);
    } catch (std::overflow_error& e) {
    }

    udecimal::U8 f0("99");
    udecimal::U8 f1("100");

    try {
        f0 - f1;
        assert(false);
    } catch (std::overflow_error& e) {
    }

    f0 = udecimal::U8(".001");
    f1 = udecimal::U8(".002");

    try {
        f0 - f1;
        assert(false);
    } catch (std::overflow_error& e) {
    }
}

void testOverflow() {
    auto f0 = udecimal::U8(1.12345678);
    assert(f0.to_string() == "1.12345678");

    f0 = udecimal::U8(1.123456789123);
    assert(f0.to_string() == "1.12345679");

    f0 = udecimal::U8(1.0 / 3.0);
    assert(f0.to_string() == "0.33333333");

    f0 = udecimal::U8(2.0 / 3.0);
    assert(f0.to_string() == "0.66666667");
}

void testNaN() {
    try {
        udecimal::U8(std::nan(""));
        assert(false);
    } catch (std::invalid_argument& e) {
    }

    try {
        udecimal::U8("NaN");
        assert(false);
    } catch (std::invalid_argument& e) {
    }

    udecimal::U8 f0("0.0004096");
    assert(f0.to_string() == "0.0004096");
}

void testIntFrac() {
    auto f0 = udecimal::U8(1234.5678);
    assert(f0.to_int() == 1234);
    assert(f0.to_frac() == .5678);
}

void testString() {
    auto f0 = udecimal::U8(1234.5678);
    assert(f0.to_string() == "1234.5678");

    f0 = udecimal::U8(1234.0);
    assert(f0.to_string() == "1234");

    f0 = udecimal::U8("1.1");
    std::string s = f0.to_string(2);

    assert(s == "1.10");

    f0 = udecimal::U8("1");
    s = f0.to_string(2);

    assert(s == "1.00");

    f0 = udecimal::U8("1.123");
    s = f0.to_string(2);

    assert(s == "1.12");

    f0 = udecimal::U8("1.123");
    s = f0.to_string(2);

    assert(s == "1.12");

    f0 = udecimal::U8("1.123");
    s = f0.to_string(0);

    assert(s == "1");

    s = f0.to_string(10);
    assert(s == "1.12300000");

    f0 = udecimal::U8(0.0);
    s = f0.to_string(10);
    assert(s == "0.00000000");
}

void testRound() {
    udecimal::U8 f0 = udecimal::U8("1.12345");
    udecimal::U8 f1 = f0.round(2);

    assert(f1.to_string() == "1.12");

    f1 = f0.round(5);
    assert(f1.to_string() == "1.12345");

    f1 = f0.round(4);
    assert(f1.to_string() == "1.1235");

    f1 = f0.round(0);
    assert(f1.to_string() == "1");

    f0 = udecimal::U8("1.12345");
    f1 = f0.round(7);
    assert(f1.to_string() == "1.12345");

    f0 = udecimal::U8("0");
    f1 = f0.round(4);
    assert(f1.to_string() == "0");

    f0 = udecimal::U8("0.0001234");
    f1 = f0.round(2);
    assert(f1.to_string() == "0");

    f0 = udecimal::U8("0.6789");
    f1 = f0.round(2);
    assert(f1.to_string() == "0.68");

    f0 = udecimal::U8("0.0000");
    f1 = f0.round(2);
    assert(f1.to_string() == "0");

    f0 = udecimal::U8("123456789.987654321");
    f1 = f0.round(3);
    assert(f1.to_string() == "123456789.988");

    f0 = udecimal::U8("123456789.987654321");
    f1 = f0.round(0);
    assert(f1.to_string() == "123456789");

    Decimal<18> f2 = Decimal<18>("0.0000000123456789");
    Decimal<18> f3 = f2.round(10);
    assert(f3.to_string() == "0.0000000123");
}

void testGeneralizedPlaces() {
    Decimal<9> f0 = Decimal<9>("9999999999.12345678901234567890");
    assert(f0.scale == 1000000000);
    assert(f0.MAX == 9999999999.999999999);
    assert(f0.zeros() == "000000000");
    assert(f0.to_string() == "9999999999.123456789");

    Decimal<10> f1 = Decimal<10>("999999999.1234567891234567890");
    assert(f1.scale == 10000000000);
    assert(f1.MAX == 999999999.9999999999);
    assert(f1.zeros() == "0000000000");
    assert(f1.to_string() == "999999999.1234567891");

    Decimal<11> f2 = Decimal<11>("99999999.12345678901234567890");
    assert(f2.scale == 100000000000);
    assert(f2.MAX == 99999999.99999999999);
    assert(f2.zeros() == "00000000000");
    assert(f2.to_string() == "99999999.12345678901");

    Decimal<18> f3 = Decimal<18>("9.12345678901234567890");
    assert(f3.scale == 1000000000000000000);
    assert(f3.MAX == 9.999999999999999999);
    assert(f3.zeros() == "000000000000000000");
    assert(f3.to_string() == "9.123456789012345678");

    Decimal<1> f4 = Decimal<1>("999999999999999999.12345678901234567890");
    assert(f4.scale == 10);
    assert(f4.MAX == 999999999999999999.9);
    assert(f4.zeros() == "0");
    assert(f4.to_string() == "999999999999999999.1");
}

void testConvertPrecision() {
    udecimal::U8 f0("1.12345678");
    Decimal<2> f1 = f0.convert_precision<2>();
    assert(f1.to_string() == "1.12");

    udecimal::U8 f2("100");
    Decimal<2> f3 = f2.convert_precision<2>();
    assert(f3.to_string() == "100");

    Decimal<2> f4("1.12");
    udecimal::U8 f5 = f4.convert_precision<8>();
    assert(f5.to_string() == "1.12");

    udecimal::U8 f6("1.12345678");
    udecimal::U8 f7 = f6.convert_precision<8>();
    assert(f7.to_string() == "1.12345678");

    udecimal::U8 f8("1e8");
    Decimal<2> f9 = f8.convert_precision<2>();
    assert(f9.to_string() == "100000000");

    udecimal::U8 f10("0.000000012345678");
    Decimal<4> f11 = f10.convert_precision<4>();
    assert(f11.to_string() == "0");

    udecimal::U8 f12("1.126");
    Decimal<2> f13 = f12.convert_precision<2>();
    assert(f13.to_string() == "1.13");

    udecimal::U8 f14("1.124");
    Decimal<2> f15 = f14.convert_precision<2>();
    assert(f15.to_string() == "1.12");

    Decimal<1> f16("12345678901234567");
    Decimal<2> f17 = f16.convert_precision<2>();
    assert(f16.to_string() == f17.to_string());

    Decimal<18> f18("0.0000000000000000001");
    Decimal<1> f19 = f18.convert_precision<1>();
    assert(f19.to_string() == "0");

    Decimal<1> f20("123456789012345678");
    try {
        Decimal<18> f21 = f20.convert_precision<18>();
        assert(false);
    } catch (const std::overflow_error& e) {
        std::cout << "Caught expected overflow error" << std::endl;
    }

    Decimal<1> f22("123456789012345678");
    try {
        Decimal<3> f23 = f22.convert_precision<3>();
        assert(false);
    } catch (const std::overflow_error& e) {
        std::cout << "Caught expected overflow error" << std::endl;
    }

    Decimal<6> f24("12345.6789");
    Decimal<13> f25 = f24.convert_precision<13>();
    assert(f25.to_string() == "12345.6789");

    Decimal<6> f26("12345.6789");
    try {
        Decimal<16> f27 = f26.convert_precision<16>();
        assert(false);
    } catch (const std::overflow_error& e) {
        std::cout << "Caught expected overflow error" << std::endl;
    }
}

template <int nPlaces>
void testEncodeDecode() {
    udecimal::Decimal<nPlaces> original(123.456789);
    std::vector<uint8_t> binary_data = original.encode_binary();

    udecimal::Decimal<nPlaces> result;
    size_t offset = 0;
    result.decode_binary(binary_data, offset);

    assert(original == result);
    assert(offset > 0);

    std::vector<uint8_t> binary_vec{33};
    size_t vec_offset = 1;
    original.encode_binary(binary_vec, vec_offset);

    std::vector<uint8_t> extra_data = {42, 24, 0};
    binary_vec.insert(binary_vec.end(), extra_data.begin(), extra_data.end());

    std::vector<uint8_t> expected{33};
    expected.insert(expected.end(), binary_data.begin(), binary_data.end());
    expected.insert(expected.end(), extra_data.begin(), extra_data.end());

    assert(binary_vec == expected);
}

template <int nPlaces>
void testDecodeBinaryData() {
    udecimal::Decimal<nPlaces> original(987.654321);
    std::vector<uint8_t> binary_data = original.encode_binary();
    std::vector<uint8_t> extra_data = {42, 24, 0};

    binary_data.insert(binary_data.end(), extra_data.begin(), extra_data.end());

    udecimal::Decimal<nPlaces> result;
    size_t offset = result.decode_binary_data(binary_data);

    std::vector<uint8_t> remaining_data{binary_data.begin() + offset, binary_data.end()};

    assert(original == result);
    assert(extra_data == remaining_data);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();

    testToDouble();
    testInfinite();
    testAddSub();
    testMulDiv();
    testNegatives();
    testOverflow();
    testNaN();
    testIntFrac();
    testString();
    testRound();
    testGeneralizedPlaces();
    testConvertPrecision();

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
