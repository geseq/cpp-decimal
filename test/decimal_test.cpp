#include "decimal.hpp"

#include <gtest/gtest.h>

#include <cassert>
#include <cstdint>
#include <cwchar>
#include <iostream>

using decimal::Decimal;

class DecimalTest : public ::testing::Test {
   protected:
    void SetUp() override {}

    void TearDown() override {}
};

TEST_F(DecimalTest, BasicU8) {
    decimal::U8 f0 = decimal::U8("123.456");
    decimal::U8 f1 = decimal::U8("123.456");

    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f1, f0);
    ASSERT_EQ(f0.to_int(), 123);
    ASSERT_EQ(f1.to_int(), 123);
    ASSERT_EQ(f0.to_string(), "123.456");
    ASSERT_EQ(f1.to_string(), "123.456");

    f0 = decimal::U8(1, 0);
    f1 = decimal::U8(.5) + (decimal::U8(.5));
    decimal::U8 f2 = decimal::U8(.3) + decimal::U8(.3) + decimal::U8(.4);

    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f0, f2);

    f0 = decimal::U8(.999);
    ASSERT_EQ(f0.to_string(), "0.999");
}

TEST_F(DecimalTest, BasicI8) {
    decimal::I8 f0 = decimal::I8("-123.456");
    decimal::I8 f1 = decimal::I8("-123.456");

    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f1, f0);
    ASSERT_EQ(f0.to_int(), -123);
    ASSERT_EQ(f1.to_int(), -123);
    ASSERT_EQ(f0.to_string(), "-123.456");
    ASSERT_EQ(f1.to_string(), "-123.456");

    f0 = decimal::I8("123.456");
    f1 = decimal::I8("123.456");

    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f1, f0);
    ASSERT_EQ(f0.to_int(), 123);
    ASSERT_EQ(f1.to_int(), 123);
    ASSERT_EQ(f0.to_string(), "123.456");
    ASSERT_EQ(f1.to_string(), "123.456");

    f0 = decimal::I8(-1, 0);
    f1 = decimal::I8(-.5) + decimal::I8(-.5);
    decimal::I8 f2 = decimal::I8(-.3) + decimal::I8(-.3) + decimal::I8(-.4);

    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f0, f2);

    f0 = decimal::I8(-.999);
    ASSERT_EQ(f0.to_string(), "-0.999");

    f0 = decimal::I8(1, 0);
    f1 = decimal::I8(.5) + (decimal::I8(.5));
    f2 = decimal::I8(.3) + decimal::I8(.3) + decimal::I8(.4);

    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f0, f2);

    f0 = decimal::I8(.999);
    ASSERT_EQ(f0.to_string(), "0.999");
}

TEST_F(DecimalTest, IntConstructorU8) {
    decimal::U8 f0(123);
    ASSERT_EQ(f0.to_string(), "123");

    decimal::U8 f1(0);
    ASSERT_EQ(f1.to_string(), "0");

    decimal::U8 f2(99999999);
    ASSERT_EQ(f2.to_string(), "99999999");

    ASSERT_EQ(f0.to_double(), 123.0);
    ASSERT_EQ(f2.to_double(), 99999999.0);
}

TEST_F(DecimalTest, IntConstructorI8) {
    decimal::I8 f0(123);
    ASSERT_EQ(f0.to_string(), "123");

    decimal::I8 f1(-123);
    ASSERT_EQ(f1.to_string(), "-123");

    decimal::I8 f2(0);
    ASSERT_EQ(f2.to_string(), "0");

    decimal::I8 f3(9999999);
    ASSERT_EQ(f3.to_string(), "9999999");

    decimal::I8 f4(-9999999);
    ASSERT_EQ(f4.to_string(), "-9999999");

    ASSERT_EQ(f0.to_double(), 123.0);
    ASSERT_EQ(f1.to_double(), -123.0);
}

TEST_F(DecimalTest, EqualU8) {
    decimal::U8 f0{};
    decimal::U8 f1("123.456");

    ASSERT_NE(f0, f1);
    ASSERT_NE(f1, f0);

    f1 = decimal::U8(0, 0);
    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f0.to_int(), 0);
}

TEST_F(DecimalTest, EqualI8) {
    decimal::I8 f0{};
    decimal::I8 f1("123.456");
    decimal::I8 f2("-123.456");

    ASSERT_NE(f0, f1);
    ASSERT_NE(f0, f2);
    ASSERT_NE(f1, f0);
    ASSERT_NE(f1, f2);
    ASSERT_NE(f2, f0);
    ASSERT_NE(f2, f1);

    f1 = decimal::I8(0, 0);
    ASSERT_EQ(f0, f1);
    ASSERT_EQ(f0.to_int(), 0);

    f1 = decimal::I8(-123456, 3);
    ASSERT_EQ(f2, f1);
    ASSERT_EQ(f2.to_int(), -123);
}

TEST_F(DecimalTest, FromExpU8) {
    auto f = decimal::U8::FromExp(123, 1);
    ASSERT_EQ(f.to_string(), "1230");

    f = decimal::U8::FromExp(123, 0);
    ASSERT_EQ(f.to_string(), "123");

    f = decimal::U8::FromExp(123, -1);
    ASSERT_EQ(f.to_string(), "12.3");

    f = decimal::U8::FromExp(123456789001, -9);
    ASSERT_EQ(f.to_string(), "123.456789");

    f = decimal::U8::FromExp(123456789012, -9);
    ASSERT_EQ(f.to_string(7), "123.4567890");

    f = decimal::U8::FromExp(123456789012, -9);
    ASSERT_EQ(f.to_string(8), "123.45678901");
}

TEST_F(DecimalTest, FromExpI8) {
    auto f = decimal::I8::FromExp(123, 1);
    ASSERT_EQ(f.to_string(), "1230");

    f = decimal::I8::FromExp(123, 0);
    ASSERT_EQ(f.to_string(), "123");

    f = decimal::I8::FromExp(123, -1);
    ASSERT_EQ(f.to_string(), "12.3");

    f = decimal::I8::FromExp(123456789001, -9);
    ASSERT_EQ(f.to_string(), "123.456789");

    f = decimal::I8::FromExp(123456789012, -9);
    ASSERT_EQ(f.to_string(7), "123.4567890");

    f = decimal::I8::FromExp(123456789012, -9);
    ASSERT_EQ(f.to_string(8), "123.45678901");

    f = decimal::I8::FromExp(-123, 1);
    ASSERT_EQ(f.to_string(), "-1230");

    f = decimal::I8::FromExp(-123, 0);
    ASSERT_EQ(f.to_string(), "-123");

    f = decimal::I8::FromExp(-123, -1);
    ASSERT_EQ(f.to_string(), "-12.3");

    f = decimal::I8::FromExp(-123456789001, -9);
    ASSERT_EQ(f.to_string(), "-123.456789");

    f = decimal::I8::FromExp(-123456789012, -9);
    ASSERT_EQ(f.to_string(7), "-123.4567890");

    f = decimal::I8::FromExp(-123456789012, -9);
    ASSERT_EQ(f.to_string(8), "-123.45678901");
}

TEST_F(DecimalTest, Parse) {
    ASSERT_NO_THROW(decimal::U8("123"));
    ASSERT_NO_THROW(decimal::I8("123"));

    ASSERT_THROW(decimal::U8(",123456"), std::invalid_argument);
    ASSERT_THROW(decimal::I8(",123456"), std::invalid_argument);
    ASSERT_THROW(decimal::U8("abc"), std::invalid_argument);
    ASSERT_THROW(decimal::I8("abc"), std::invalid_argument);
}

TEST_F(DecimalTest, NewIU8) {
    decimal::U8 f = decimal::U8(123, 1);
    ASSERT_EQ(f.to_string(), "12.3");

    f = decimal::U8(123, 0);
    ASSERT_EQ(f.to_string(), "123");

    f = decimal::U8(123, 5);
    ASSERT_EQ(f.to_string(), "0.00123");

    f = decimal::U8(123456789001, 9);
    ASSERT_EQ(f.to_string(), "123.456789");

    f = decimal::U8(123456789012, 9);
    ASSERT_EQ(f.to_string(7), "123.4567890");
    ASSERT_EQ(f.to_string(), "123.45678901");

    f = decimal::U8(123456789012, 9);
    ASSERT_EQ(f.to_string(8), "123.45678901");

    auto g = Decimal<3>(123, 5);
    ASSERT_EQ(g.to_string(), "0.001");
}

TEST_F(DecimalTest, NewII8) {
    decimal::I8 f = decimal::I8(123, 1);
    ASSERT_EQ(f.to_string(), "12.3");

    f = decimal::I8(123, 0);
    ASSERT_EQ(f.to_string(), "123");

    f = decimal::I8(123, 5);
    ASSERT_EQ(f.to_string(), "0.00123");

    f = decimal::I8(123456789001, 9);
    ASSERT_EQ(f.to_string(), "123.456789");

    f = decimal::I8(123456789012, 9);
    ASSERT_EQ(f.to_string(7), "123.4567890");
    ASSERT_EQ(f.to_string(), "123.45678901");

    f = decimal::I8(123456789012, 9);
    ASSERT_EQ(f.to_string(8), "123.45678901");

    f = decimal::I8(-123, 1);
    ASSERT_EQ(f.to_string(), "-12.3");

    f = decimal::I8(-123, 0);
    ASSERT_EQ(f.to_string(), "-123");

    f = decimal::I8(-123, 5);
    ASSERT_EQ(f.to_string(), "-0.00123");

    f = decimal::I8(-123456789001, 9);
    ASSERT_EQ(f.to_string(), "-123.456789");

    f = decimal::I8(-123456789012, 9);
    ASSERT_EQ(f.to_string(7), "-123.4567890");
    ASSERT_EQ(f.to_string(), "-123.45678901");

    f = decimal::I8(-123456789012, 9);
    ASSERT_EQ(f.to_string(8), "-123.45678901");

    auto g = Decimal<3, decimal::Signed>(123, 5);
    ASSERT_EQ(g.to_string(), "0.001");

    g = Decimal<3, decimal::Signed>(-123, 5);
    ASSERT_EQ(g.to_string(), "-0.001");
}

TEST_F(DecimalTest, MaxValueU8) {
    decimal::U8 f0 = decimal::U8("12345678901");
    ASSERT_EQ(f0.MAX, 99999999999.99999999);
    ASSERT_EQ(f0.to_string(), "12345678901");

    ASSERT_THROW(decimal::U8("123456789012"), std::overflow_error);
    ASSERT_THROW(decimal::U8("-12345678901"), std::overflow_error);
    ASSERT_THROW(decimal::U8("-123456789012"), std::overflow_error);

    f0 = decimal::U8("99999999999");
    ASSERT_EQ(f0.to_string(), "99999999999");

    f0 = decimal::U8("9.99999999");
    ASSERT_EQ(f0.to_string(), "9.99999999");

    f0 = decimal::U8("99999999999.99999999");
    ASSERT_EQ(f0.to_string(), "99999999999.99999999");

    f0 = decimal::U8("99999999999.12345678901234567890");
    ASSERT_EQ(f0.to_string(), "99999999999.12345678");
}

TEST_F(DecimalTest, MaxValueI8) {
    decimal::I8 f0 = decimal::I8("1234567890");
    ASSERT_EQ(f0.MAX, 9999999999.99999999);
    ASSERT_EQ(f0.to_string(), "1234567890");

    ASSERT_THROW(decimal::I8("12345678901"), std::overflow_error);
    ASSERT_THROW(decimal::I8("123456789012"), std::overflow_error);

    f0 = decimal::I8("9999999999");
    ASSERT_EQ(f0.to_string(), "9999999999");

    f0 = decimal::I8("9.99999999");
    ASSERT_EQ(f0.to_string(), "9.99999999");

    f0 = decimal::I8("9999999999.99999999");
    ASSERT_EQ(f0.to_string(), "9999999999.99999999");

    f0 = decimal::I8("9999999999.12345678901234567890");
    ASSERT_EQ(f0.to_string(), "9999999999.12345678");

    f0 = decimal::I8("-1234567890");
    ASSERT_EQ(f0.MAX, 9999999999.99999999);
    ASSERT_EQ(f0.to_string(), "-1234567890");

    ASSERT_THROW(decimal::I8("-12345678901"), std::overflow_error);
    ASSERT_THROW(decimal::I8("-123456789012"), std::overflow_error);

    f0 = decimal::I8("-9999999999");
    ASSERT_EQ(f0.to_string(), "-9999999999");

    f0 = decimal::I8("-9.99999999");
    ASSERT_EQ(f0.to_string(), "-9.99999999");

    f0 = decimal::I8("-9999999999.99999999");
    ASSERT_EQ(f0.to_string(), "-9999999999.99999999");

    f0 = decimal::I8("-9999999999.12345678901234567890");
    ASSERT_EQ(f0.to_string(), "-9999999999.12345678");
}

TEST_F(DecimalTest, ToDoubleU8) {
    auto f0 = decimal::U8("123.456");
    auto f1 = decimal::U8(123.456);

    ASSERT_EQ(f0, f1);

    f1 = decimal::U8(0.0001);

    ASSERT_EQ(f1.to_string(), "0.0001");

    f1 = decimal::U8(".1");
    ASSERT_EQ(f1.to_string(), "0.1");

    auto f2 = decimal::U8(decimal::U8(f1.to_double()).to_string());
    ASSERT_EQ(f1, f2);
}

TEST_F(DecimalTest, ToDoubleI8) {
    auto f0 = decimal::I8("123.456");
    auto f1 = decimal::I8(123.456);

    ASSERT_EQ(f0, f1);

    f1 = decimal::I8(0.0001);
    ASSERT_EQ(f1.to_string(), "0.0001");

    f1 = decimal::I8(".1");
    ASSERT_EQ(f1.to_string(), "0.1");

    auto f2 = decimal::I8(decimal::I8(f1.to_double()).to_string());
    ASSERT_EQ(f1, f2);

    f0 = decimal::I8("-123.456");
    f1 = decimal::I8(-123.456);

    ASSERT_EQ(f0, f1);

    f1 = decimal::I8(-0.0001);
    ASSERT_EQ(f1.to_string(), "-0.0001");

    f1 = decimal::I8("-.1");
    ASSERT_EQ(f1.to_string(), "-0.1");

    auto b = decimal::I8(f1.to_double()).to_string();
    f2 = decimal::I8(decimal::I8(f1.to_double()).to_string());
    ASSERT_EQ(f1, f2);
}

TEST_F(DecimalTest, InfiniteU8) {
    auto f0 = decimal::U8("0.10");
    auto f1 = decimal::U8(0.10);

    ASSERT_EQ(f0, f1);

    auto f2 = decimal::U8(0.0);
    for (int i = 0; i < 3; i++) {
        f2 = f2 + decimal::U8(0.10);
    }
    ASSERT_EQ(f2.to_string(), "0.3");

    f2 = decimal::U8(0.0);
    for (int i = 0; i < 10; i++) {
        f2 = f2 + decimal::U8(0.10);
    }
    ASSERT_EQ(f2.to_string(), "1");
}

TEST_F(DecimalTest, InfiniteI8) {
    auto f0 = decimal::I8("0.10");
    auto f1 = decimal::I8(0.10);

    ASSERT_EQ(f0, f1);

    auto f2 = decimal::I8(0.0);
    for (int i = 0; i < 3; i++) {
        f2 = f2 + decimal::I8(0.10);
    }
    ASSERT_EQ(f2.to_string(), "0.3");

    f2 = decimal::I8(0.0);
    for (int i = 0; i < 10; i++) {
        f2 = f2 + decimal::I8(0.10);
    }
    ASSERT_EQ(f2.to_string(), "1");

    f0 = decimal::I8("-0.10");
    f1 = decimal::I8(-0.10);

    ASSERT_EQ(f0, f1);

    f2 = decimal::I8(0.0);
    for (int i = 0; i < 3; i++) {
        f2 = f2 - decimal::I8(0.10);
    }
    ASSERT_EQ(f2.to_string(), "-0.3");

    f2 = decimal::I8(0.0);
    for (int i = 0; i < 10; i++) {
        f2 = f2 - decimal::I8(0.10);
    }
    ASSERT_EQ(f2.to_string(), "-1");
}

TEST_F(DecimalTest, AddSubU8) {
    decimal::U8 f0 = decimal::U8("1");
    decimal::U8 f1 = decimal::U8("0.3333333");

    decimal::U8 f2 = f0 - f1;
    f2 = f2 - f1;
    f2 -= f1;

    ASSERT_EQ(f2.to_string(), "0.0000001");

    f2 = f2 - decimal::U8("0.0000001");
    ASSERT_EQ(f2.to_string(), "0");

    f0 = decimal::U8("0");
    for (int i = 0; i < 5; i++) {
        f0 = f0 + decimal::U8("0.1");
        f0 += decimal::U8("0.1");
    }
    ASSERT_EQ(f0.to_string(), "1");
}

TEST_F(DecimalTest, AddSubI8) {
    decimal::I8 f0 = decimal::I8("1");
    decimal::I8 f1 = decimal::I8("0.3333333");

    decimal::I8 f2 = f0 - f1;
    f2 = f2 - f1;
    f2 = f2 - f1;

    ASSERT_EQ(f2.to_string(), "0.0000001");

    f2 = f2 - decimal::I8("0.0000001");
    ASSERT_EQ(f2.to_string(), "0");

    f0 = decimal::I8("0");
    for (int i = 0; i < 10; i++) {
        f0 = f0 + decimal::I8("0.1");
    }
    ASSERT_EQ(f0.to_string(), "1");

    f0 = decimal::I8("-1");
    f1 = decimal::I8("-0.3333333");

    f2 = f0 - f1;
    f2 = f2 - f1;
    f2 = f2 - f1;

    ASSERT_EQ(f2.to_string(), "-0.0000001");

    f2 = f2 - decimal::I8("-0.0000001");
    ASSERT_EQ(f2.to_string(), "0");

    f0 = decimal::I8("0");
    for (int i = 0; i < 10; i++) {
        f0 = f0 + decimal::I8("0.1");  // Note: Adding to a negative
    }
    ASSERT_EQ(f0.to_string(), "1");
}

TEST_F(DecimalTest, MulDivU8) {
    decimal::U8 f0("123.456");
    decimal::U8 f1("1000");

    decimal::U8 f2 = f0 * f1;
    ASSERT_EQ(f2.to_string(), "123456");

    f0 = decimal::U8("123456");
    f1 = decimal::U8("0.0001");

    f2 = f0 * f1;
    ASSERT_EQ(f2.to_string(), "12.3456");

    f0 = decimal::U8("10000.1");
    f1 = decimal::U8("10000");

    f2 = f0 * f1;
    ASSERT_EQ(f2.to_string(), "100001000");

    f2 = f2 / f1;
    ASSERT_EQ(f2, f0);

    f2 *= f1;
    ASSERT_EQ(f2.to_string(), "100001000");

    f2 /= f1;
    ASSERT_EQ(f2, f0);

    f0 = decimal::U8("2");
    f1 = decimal::U8("3");

    f2 = f0 / f1;
    ASSERT_EQ(f2.to_string(), "0.66666667");

    f0 = decimal::U8("1000");
    f1 = decimal::U8("10");

    f2 = f0 / f1;
    ASSERT_EQ(f2.to_string(), "100");

    f0 = decimal::U8("1000");
    f1 = decimal::U8("0.1");

    f2 = f0 / f1;
    ASSERT_EQ(f2.to_string(), "10000");

    f0 = decimal::U8("1");
    f1 = decimal::U8("0.1");

    f2 = f0 * f1;
    ASSERT_EQ(f2.to_string(), "0.1");
}

TEST_F(DecimalTest, MulDivI8) {
    decimal::I8 f0("123.456");
    decimal::I8 f1("1000");

    decimal::I8 f2 = f0 * f1;
    ASSERT_EQ(f2.to_string(), "123456");

    f0 = decimal::I8("123456");
    f1 = decimal::I8("0.0001");

    f2 = f0 * f1;
    ASSERT_EQ(f2.to_string(), "12.3456");

    f0 = decimal::I8("10000.1");
    f1 = decimal::I8("10000");

    f2 = f0 * f1;
    ASSERT_EQ(f2.to_string(), "100001000");

    f2 = f2 / f1;
    ASSERT_EQ(f2, f0);

    f2 *= f1;
    ASSERT_EQ(f2.to_string(), "100001000");

    f2 /= f1;
    ASSERT_EQ(f2, f0);

    f0 = decimal::I8("2");
    f1 = decimal::I8("3");

    f2 = f0 / f1;
    ASSERT_EQ(f2.to_string(), "0.66666667");

    f0 = decimal::I8("1000");
    f1 = decimal::I8("10");

    f2 = f0 / f1;
    ASSERT_EQ(f2.to_string(), "100");

    f0 = decimal::I8("1000");
    f1 = decimal::I8("0.1");

    f2 = f0 / f1;
    ASSERT_EQ(f2.to_string(), "10000");

    f0 = decimal::I8("1");
    f1 = decimal::I8("0.1");

    f2 = f0 * f1;
    ASSERT_EQ(f2.to_string(), "0.1");

    // Negative-Positive cases (resulting in negative)
    f0 = decimal::I8("-123.456");
    f1 = decimal::I8("1000");

    f2 = f0 * f1;
    ASSERT_EQ(f2.to_string(), "-123456");

    f0 = decimal::I8("-123456");
    f1 = decimal::I8("0.0001");

    f2 = f0 * f1;
    ASSERT_EQ(f2.to_string(), "-12.3456");

    f0 = decimal::I8("-10000.1");
    f1 = decimal::I8("10000");

    f2 = f0 * f1;
    ASSERT_EQ(f2.to_string(), "-100001000");

    f2 = f2 / f1;
    ASSERT_EQ(f2, f0);

    f2 *= f1;
    ASSERT_EQ(f2.to_string(), "-100001000");

    f2 /= f1;
    ASSERT_EQ(f2, f0);

    f0 = decimal::I8("-2");
    f1 = decimal::I8("3");

    f2 = f0 / f1;
    ASSERT_EQ(f2.to_string(), "-0.66666667");

    f0 = decimal::I8("-1000");
    f1 = decimal::I8("10");

    f2 = f0 / f1;
    ASSERT_EQ(f2.to_string(), "-100");

    f0 = decimal::I8("-1000");
    f1 = decimal::I8("0.1");

    f2 = f0 / f1;
    ASSERT_EQ(f2.to_string(), "-10000");

    f0 = decimal::I8("-1");
    f1 = decimal::I8("0.1");

    f2 = f0 * f1;
    ASSERT_EQ(f2.to_string(), "-0.1");

    // Positive-Negative cases (resulting in negative)
    f0 = decimal::I8("123.456");
    f1 = decimal::I8("-1000");

    f2 = f0 * f1;
    ASSERT_EQ(f2.to_string(), "-123456");

    f0 = decimal::I8("123456");
    f1 = decimal::I8("-0.0001");

    f2 = f0 * f1;
    ASSERT_EQ(f2.to_string(), "-12.3456");

    f0 = decimal::I8("10000.1");
    f1 = decimal::I8("-10000");

    f2 = f0 * f1;
    ASSERT_EQ(f2.to_string(), "-100001000");

    f2 = f2 / f1;
    ASSERT_EQ(f2, f0);

    f2 *= f1;
    ASSERT_EQ(f2.to_string(), "-100001000");

    f2 /= f1;
    ASSERT_EQ(f2, f0);

    f0 = decimal::I8("2");
    f1 = decimal::I8("-3");

    f2 = f0 / f1;
    ASSERT_EQ(f2.to_string(), "-0.66666667");

    f0 = decimal::I8("1000");
    f1 = decimal::I8("-10");

    f2 = f0 / f1;
    ASSERT_EQ(f2.to_string(), "-100");

    f0 = decimal::I8("1000");
    f1 = decimal::I8("-0.1");

    f2 = f0 / f1;
    ASSERT_EQ(f2.to_string(), "-10000");

    f0 = decimal::I8("1");
    f1 = decimal::I8("-0.1");

    f2 = f0 * f1;
    ASSERT_EQ(f2.to_string(), "-0.1");

    // Negative-Negative cases (resulting in positive)
    f0 = decimal::I8("-123.456");
    f1 = decimal::I8("-1000");

    f2 = f0 * f1;
    ASSERT_EQ(f2.to_string(), "123456");

    f0 = decimal::I8("-123456");
    f1 = decimal::I8("-0.0001");

    f2 = f0 * f1;
    ASSERT_EQ(f2.to_string(), "12.3456");

    f0 = decimal::I8("-10000.1");
    f1 = decimal::I8("-10000");

    f2 = f0 * f1;
    ASSERT_EQ(f2.to_string(), "100001000");

    f2 = f2 / f1;
    ASSERT_EQ(f2, f0);

    f2 *= f1;
    ASSERT_EQ(f2.to_string(), "100001000");

    f2 /= f1;
    ASSERT_EQ(f2, f0);

    f0 = decimal::I8("-2");
    f1 = decimal::I8("-3");

    f2 = f0 / f1;
    ASSERT_EQ(f2.to_string(), "0.66666667");

    f0 = decimal::I8("-1000");
    f1 = decimal::I8("-10");

    f2 = f0 / f1;
    ASSERT_EQ(f2.to_string(), "100");

    f0 = decimal::I8("-1000");
    f1 = decimal::I8("-0.1");

    f2 = f0 / f1;
    ASSERT_EQ(f2.to_string(), "10000");

    f0 = decimal::I8("-1");
    f1 = decimal::I8("-0.1");

    f2 = f0 * f1;
    ASSERT_EQ(f2.to_string(), "0.1");
}

TEST_F(DecimalTest, NegativesU8) {
    ASSERT_THROW(decimal::U8("-1"), std::overflow_error);
    ASSERT_THROW(decimal::U8(-1.0), std::overflow_error);

    decimal::U8 f0("99");
    decimal::U8 f1("100");

    ASSERT_THROW(f0 - f1, std::overflow_error);

    f0 = decimal::U8(".001");
    f1 = decimal::U8(".002");

    ASSERT_THROW(f0 - f1, std::overflow_error);
}

TEST_F(DecimalTest, NegativesI8) {
    decimal::I8 f0("-1");
    decimal::I8 f1(-1.0);

    f0 = decimal::I8("99");
    f1 = decimal::I8("100");

    ASSERT_EQ(f0 - f1, decimal::I8("-1"));

    f0 = decimal::I8(".001");
    f1 = decimal::I8(".002");

    ASSERT_EQ(f0 - f1, decimal::I8("-.001"));
}

TEST_F(DecimalTest, DecimalPrecisionTooLargeU8) {
    auto f0 = decimal::U8(1.12345678);
    ASSERT_EQ(f0.to_string(), "1.12345678");

    f0 = decimal::U8(1.123456789123);
    ASSERT_EQ(f0.to_string(), "1.12345679");

    f0 = decimal::U8(1.0 / 3.0);
    ASSERT_EQ(f0.to_string(), "0.33333333");

    f0 = decimal::U8(2.0 / 3.0);
    ASSERT_EQ(f0.to_string(), "0.66666667");
}

TEST_F(DecimalTest, DecimalPrecisionTooLargeI8) {
    auto f0 = decimal::I8(1.12345678);
    ASSERT_EQ(f0.to_string(), "1.12345678");

    f0 = decimal::I8(1.123456789123);
    ASSERT_EQ(f0.to_string(), "1.12345679");

    f0 = decimal::I8(1.0 / 3.0);
    ASSERT_EQ(f0.to_string(), "0.33333333");

    f0 = decimal::I8(2.0 / 3.0);
    ASSERT_EQ(f0.to_string(), "0.66666667");

    f0 = decimal::I8(-1.12345678);
    ASSERT_EQ(f0.to_string(), "-1.12345678");

    f0 = decimal::I8(-1.123456789123);
    ASSERT_EQ(f0.to_string(), "-1.12345679");

    f0 = decimal::I8(-1.0 / 3.0);
    ASSERT_EQ(f0.to_string(), "-0.33333333");

    f0 = decimal::I8(-2.0 / 3.0);
    ASSERT_EQ(f0.to_string(), "-0.66666667");
}

TEST_F(DecimalTest, NaN) {
    ASSERT_THROW(decimal::U8(std::nan("")), std::invalid_argument);
    ASSERT_THROW(decimal::I8(std::nan("")), std::invalid_argument);

    ASSERT_THROW(decimal::U8("NaN"), std::invalid_argument);
    ASSERT_THROW(decimal::I8("NaN"), std::invalid_argument);

    decimal::U8 f0("0.0004096");
    ASSERT_EQ(f0.to_string(), "0.0004096");

    decimal::I8 f1("0.0004096");
    ASSERT_EQ(f1.to_string(), "0.0004096");

    decimal::I8 f2("-0.0004096");
    ASSERT_EQ(f2.to_string(), "-0.0004096");
}

TEST_F(DecimalTest, IntFrac) {
    auto f0 = decimal::U8(1234.5678);
    ASSERT_EQ(f0.to_int(), 1234);
    ASSERT_DOUBLE_EQ(f0.to_frac(), .5678);  // Use ASSERT_DOUBLE_EQ for comparing floating point numbers

    auto f1 = decimal::I8(1234.5678);
    ASSERT_EQ(f1.to_int(), 1234);
    ASSERT_DOUBLE_EQ(f1.to_frac(), .5678);  // Use ASSERT_DOUBLE_EQ for comparing floating point numbers

    f1 = decimal::I8(-1234.5678);
    ASSERT_EQ(f1.to_int(), -1234);
    ASSERT_DOUBLE_EQ(f1.to_frac(), -.5678);  // Use ASSERT_DOUBLE_EQ for comparing floating point numbers
}

TEST_F(DecimalTest, StringU8) {
    auto f0 = decimal::U8(1234.5678);
    ASSERT_EQ(f0.to_string(), "1234.5678");

    f0 = decimal::U8(1234.0);
    ASSERT_EQ(f0.to_string(), "1234");

    f0 = decimal::U8("1.1");
    std::string s = f0.to_string(2);
    ASSERT_EQ(s, "1.10");

    f0 = decimal::U8("1");
    s = f0.to_string(2);
    ASSERT_EQ(s, "1.00");

    f0 = decimal::U8("1.123");
    s = f0.to_string(2);
    ASSERT_EQ(s, "1.12");

    f0 = decimal::U8("1.123");
    s = f0.to_string(0);
    ASSERT_EQ(s, "1");

    s = f0.to_string(10);
    ASSERT_EQ(s, "1.12300000");

    f0 = decimal::U8(0.0);
    s = f0.to_string(10);
    ASSERT_EQ(s, "0.00000000");
}

TEST_F(DecimalTest, StringI8) {
    auto f0 = decimal::I8(1234.5678);
    ASSERT_EQ(f0.to_string(), "1234.5678");

    f0 = decimal::I8(1234.0);
    ASSERT_EQ(f0.to_string(), "1234");

    f0 = decimal::I8("1.1");
    std::string s = f0.to_string(2);
    ASSERT_EQ(s, "1.10");

    f0 = decimal::I8("1");
    s = f0.to_string(2);
    ASSERT_EQ(s, "1.00");

    f0 = decimal::I8("1.123");
    s = f0.to_string(2);
    ASSERT_EQ(s, "1.12");

    f0 = decimal::I8("1.123");
    s = f0.to_string(0);
    ASSERT_EQ(s, "1");

    f0 = decimal::I8("1.123");
    s = f0.to_string(10);
    ASSERT_EQ(s, "1.12300000");

    f0 = decimal::I8(0.0);
    s = f0.to_string(10);
    ASSERT_EQ(s, "0.00000000");

    f0 = decimal::I8(-1234.5678);
    ASSERT_EQ(f0.to_string(), "-1234.5678");

    f0 = decimal::I8(-1234.0);
    ASSERT_EQ(f0.to_string(), "-1234");

    f0 = decimal::I8("-1.1");
    s = f0.to_string(2);
    ASSERT_EQ(s, "-1.10");

    f0 = decimal::I8("-1");
    s = f0.to_string(2);
    ASSERT_EQ(s, "-1.00");

    f0 = decimal::I8("-1.123");
    s = f0.to_string(2);
    ASSERT_EQ(s, "-1.12");

    f0 = decimal::I8("-1.123");
    s = f0.to_string(0);
    ASSERT_EQ(s, "-1");

    f0 = decimal::I8("-1.123");
    s = f0.to_string(10);
    ASSERT_EQ(s, "-1.12300000");
}

TEST_F(DecimalTest, RoundU8) {
    decimal::U8 f0 = decimal::U8("1.12345");
    decimal::U8 f1 = f0.round(2);
    ASSERT_EQ(f1.to_string(), "1.12");

    f1 = f0.round(5);
    ASSERT_EQ(f1.to_string(), "1.12345");

    f1 = f0.round(4);
    ASSERT_EQ(f1.to_string(), "1.1235");

    f1 = f0.round(0);
    ASSERT_EQ(f1.to_string(), "1");

    f0 = decimal::U8("1.12345");
    f1 = f0.round(7);
    ASSERT_EQ(f1.to_string(), "1.12345");

    f0 = decimal::U8("0");
    f1 = f0.round(4);
    ASSERT_EQ(f1.to_string(), "0");

    f0 = decimal::U8("0.0001234");
    f1 = f0.round(2);
    ASSERT_EQ(f1.to_string(), "0");

    f0 = decimal::U8("0.6789");
    f1 = f0.round(2);
    ASSERT_EQ(f1.to_string(), "0.68");

    f0 = decimal::U8("0.0000");
    f1 = f0.round(2);
    ASSERT_EQ(f1.to_string(), "0");

    f0 = decimal::U8("123456789.987654321");
    f1 = f0.round(3);
    ASSERT_EQ(f1.to_string(), "123456789.988");

    f0 = decimal::U8("123456789.987654321");
    f1 = f0.round(0);
    ASSERT_EQ(f1.to_string(), "123456789");

    Decimal<18> f2 = Decimal<18>("0.0000000123456789");
    Decimal<18> f3 = f2.round(10);
    ASSERT_EQ(f3.to_string(), "0.0000000123");
}

TEST_F(DecimalTest, RoundI8) {
    // Test cases for positive values
    decimal::I8 f0 = decimal::I8("1.12345");
    decimal::I8 f1 = f0.round(2);
    ASSERT_EQ(f1.to_string(), "1.12");

    f1 = f0.round(5);
    ASSERT_EQ(f1.to_string(), "1.12345");

    f1 = f0.round(4);
    ASSERT_EQ(f1.to_string(), "1.1235");

    f1 = f0.round(0);
    ASSERT_EQ(f1.to_string(), "1");

    f0 = decimal::I8("1.12345");
    f1 = f0.round(7);
    ASSERT_EQ(f1.to_string(), "1.12345");

    f0 = decimal::I8("0");
    f1 = f0.round(4);
    ASSERT_EQ(f1.to_string(), "0");

    f0 = decimal::I8("0.0001234");
    f1 = f0.round(2);
    ASSERT_EQ(f1.to_string(), "0");

    f0 = decimal::I8("0.6789");
    f1 = f0.round(2);
    ASSERT_EQ(f1.to_string(), "0.68");

    f0 = decimal::I8("0.0000");
    f1 = f0.round(2);
    ASSERT_EQ(f1.to_string(), "0");

    f0 = decimal::I8("123456789.987654321");
    f1 = f0.round(3);
    ASSERT_EQ(f1.to_string(), "123456789.988");

    f0 = decimal::I8("123456789.987654321");
    f1 = f0.round(0);
    ASSERT_EQ(f1.to_string(), "123456789");

    Decimal<17, decimal::Signed> f2 = Decimal<17, decimal::Signed>("0.000000012345678");
    Decimal<17, decimal::Signed> f3 = f2.round(10);
    ASSERT_EQ(f3.to_string(), "0.0000000123");

    // Additional test cases for negative values
    f0 = decimal::I8("-1.12345");
    f1 = f0.round(2);
    ASSERT_EQ(f1.to_string(), "-1.12");

    f1 = f0.round(5);
    ASSERT_EQ(f1.to_string(), "-1.12345");

    f1 = f0.round(4);
    ASSERT_EQ(f1.to_string(), "-1.1235");

    f1 = f0.round(0);
    ASSERT_EQ(f1.to_string(), "-1");

    f0 = decimal::I8("-1.12345");
    f1 = f0.round(7);
    ASSERT_EQ(f1.to_string(), "-1.12345");

    f0 = decimal::I8("-0.0001234");
    f1 = f0.round(2);
    ASSERT_EQ(f1.to_string(), "0");

    f0 = decimal::I8("-0.6789");
    f1 = f0.round(2);
    ASSERT_EQ(f1.to_string(), "-0.68");

    f0 = decimal::I8("-123456789.987654321");
    f1 = f0.round(3);
    ASSERT_EQ(f1.to_string(), "-123456789.988");

    f0 = decimal::I8("-123456789.987654321");
    f1 = f0.round(0);
    ASSERT_EQ(f1.to_string(), "-123456789");
}

TEST_F(DecimalTest, GeneralizedPlaces_Unsigned) {
    decimal::U9 f0 = decimal::U9("9999999999.12345678901234567890");
    ASSERT_EQ(f0.scale, 1000000000);
    ASSERT_EQ(f0.MAX, 9999999999.999999999);
    ASSERT_EQ(f0.MIN, 0);
    ASSERT_EQ(f0.zeros(), "000000000");
    ASSERT_EQ(f0.to_string(), "9999999999.123456789");

    decimal::U10 f1 = decimal::U10("999999999.1234567891234567890");
    ASSERT_EQ(f1.scale, 10000000000);
    ASSERT_EQ(f1.MAX, 999999999.9999999999);
    ASSERT_EQ(f1.zeros(), "0000000000");
    ASSERT_EQ(f1.to_string(), "999999999.1234567891");

    decimal::U11 f2 = decimal::U11("99999999.12345678901234567890");
    ASSERT_EQ(f2.scale, 100000000000);
    ASSERT_EQ(f2.MAX, 99999999.99999999999);
    ASSERT_EQ(f2.zeros(), "00000000000");
    ASSERT_EQ(f2.to_string(), "99999999.12345678901");

    decimal::U18 f3 = decimal::U18("9.12345678901234567890");
    ASSERT_EQ(f3.scale, 1000000000000000000);
    ASSERT_EQ(f3.MAX, 9.999999999999999999);
    ASSERT_EQ(f3.zeros(), "000000000000000000");
    ASSERT_EQ(f3.to_string(), "9.123456789012345678");

    decimal::U1 f4 = decimal::U1("999999999999999999.12345678901234567890");
    ASSERT_EQ(f4.scale, 10);
    ASSERT_EQ(f4.MAX, 999999999999999999.9);
    ASSERT_EQ(f4.zeros(), "0");
    ASSERT_EQ(f4.to_string(), "999999999999999999.1");
}

TEST_F(DecimalTest, GeneralizedPlaces_Signed_Positive) {
    decimal::I9 f0 = decimal::I9("999999999.12345678901234567890");
    ASSERT_EQ(f0.scale, 1000000000);
    ASSERT_EQ(f0.MAX, 999999999.999999999);
    ASSERT_EQ(f0.MIN, -999999999.999999999);
    ASSERT_EQ(f0.zeros(), "000000000");
    ASSERT_EQ(f0.to_string(), "999999999.123456789");

    decimal::I10 f1 = decimal::I10("99999999.1234567891234567890");
    ASSERT_EQ(f1.scale, 10000000000);
    ASSERT_EQ(f1.MAX, 99999999.9999999999);
    ASSERT_EQ(f1.MIN, -99999999.9999999999);
    ASSERT_EQ(f1.zeros(), "0000000000");
    ASSERT_EQ(f1.to_string(), "99999999.1234567891");

    decimal::I11 f2 = decimal::I11("9999999.12345678901234567890");
    ASSERT_EQ(f2.scale, 100000000000);
    ASSERT_EQ(f2.MAX, 9999999.99999999999);
    ASSERT_EQ(f2.MIN, -9999999.99999999999);
    ASSERT_EQ(f2.zeros(), "00000000000");
    ASSERT_EQ(f2.to_string(), "9999999.12345678901");

    decimal::I17 f3 = decimal::I17("9.12345678901234567890");
    ASSERT_EQ(f3.scale, 100000000000000000);
    ASSERT_EQ(f3.MAX, 9.999999999999999999);
    ASSERT_EQ(f3.MIN, -9.999999999999999999);
    ASSERT_EQ(f3.zeros(), "00000000000000000");
    ASSERT_EQ(f3.to_string(), "9.12345678901234567");

    decimal::I1 f4 = decimal::I1("99999999999999999.12345678901234567890");
    ASSERT_EQ(f4.scale, 10);
    ASSERT_EQ(f4.MAX, 99999999999999999.9);
    ASSERT_EQ(f4.MIN, -99999999999999999.9);
    ASSERT_EQ(f4.zeros(), "0");
    ASSERT_EQ(f4.to_string(), "99999999999999999.1");
}

TEST_F(DecimalTest, GeneralizedPlaces_Signed_Negative) {
    decimal::I9 f0 = decimal::I9("-999999999.12345678901234567890");
    ASSERT_EQ(f0.scale, 1000000000);
    ASSERT_EQ(f0.MAX, 999999999.999999999);
    ASSERT_EQ(f0.MIN, -999999999.999999999);
    ASSERT_EQ(f0.zeros(), "000000000");
    ASSERT_EQ(f0.to_string(), "-999999999.123456789");

    decimal::I10 f1 = decimal::I10("-99999999.1234567891234567890");
    ASSERT_EQ(f1.scale, 10000000000);
    ASSERT_EQ(f1.MAX, 99999999.9999999999);
    ASSERT_EQ(f1.MIN, -99999999.9999999999);
    ASSERT_EQ(f1.zeros(), "0000000000");
    ASSERT_EQ(f1.to_string(), "-99999999.1234567891");

    decimal::I11 f2 = decimal::I11("-9999999.12345678901234567890");
    ASSERT_EQ(f2.scale, 100000000000);
    ASSERT_EQ(f2.MAX, 9999999.99999999999);
    ASSERT_EQ(f2.MIN, -9999999.99999999999);
    ASSERT_EQ(f2.zeros(), "00000000000");
    ASSERT_EQ(f2.to_string(), "-9999999.12345678901");

    decimal::I17 f3 = decimal::I17("-9.12345678901234567890");
    ASSERT_EQ(f3.scale, 100000000000000000);
    ASSERT_EQ(f3.MAX, 9.999999999999999999);
    ASSERT_EQ(f3.MIN, -9.999999999999999999);
    ASSERT_EQ(f3.zeros(), "00000000000000000");
    ASSERT_EQ(f3.to_string(), "-9.12345678901234567");

    decimal::I1 f4 = decimal::I1("-99999999999999999.12345678901234567890");
    ASSERT_EQ(f4.scale, 10);
    ASSERT_EQ(f4.MAX, 99999999999999999.9);
    ASSERT_EQ(f4.MIN, -99999999999999999.9);
    ASSERT_EQ(f4.zeros(), "0");
    ASSERT_EQ(f4.to_string(), "-99999999999999999.1");
}

TEST_F(DecimalTest, ConvertPrecision_Unsigned) {
    decimal::U8 f0("1.12345678");
    decimal::U2 f1 = f0.convert_precision<2>();
    ASSERT_EQ(f1.to_string(), "1.12");

    decimal::U8 f2("100");
    decimal::U2 f3 = f2.convert_precision<2>();
    ASSERT_EQ(f3.to_string(), "100");

    decimal::U2 f4("1.12");
    decimal::U8 f5 = f4.convert_precision<8>();
    ASSERT_EQ(f5.to_string(), "1.12");

    decimal::U8 f6("1.12345678");
    decimal::U8 f7 = f6.convert_precision<8>();
    ASSERT_EQ(f7.to_string(), "1.12345678");

    decimal::U8 f8("1e8");
    decimal::U2 f9 = f8.convert_precision<2>();
    ASSERT_EQ(f9.to_string(), "100000000");

    decimal::U8 f10("0.000000012345678");
    decimal::U4 f11 = f10.convert_precision<4>();
    ASSERT_EQ(f11.to_string(), "0");

    decimal::U8 f12("1.126");
    decimal::U2 f13 = f12.convert_precision<2>();
    ASSERT_EQ(f13.to_string(), "1.13");

    decimal::U8 f14("1.124");
    decimal::U2 f15 = f14.convert_precision<2>();
    ASSERT_EQ(f15.to_string(), "1.12");

    decimal::U1 f16("12345678901234567");
    decimal::U2 f17 = f16.convert_precision<2>();
    ASSERT_EQ(f16.to_string(), f17.to_string());

    decimal::U18 f18("0.0000000000000000001");
    decimal::U1 f19 = f18.convert_precision<1>();
    ASSERT_EQ(f19.to_string(), "0");

    decimal::U1 f20("123456789012345678");
    ASSERT_THROW(f20.convert_precision<18>(), std::overflow_error);

    decimal::U1 f22("123456789012345678");
    ASSERT_THROW(f22.convert_precision<3>(), std::overflow_error);

    decimal::U6 f24("12345.6789");
    ASSERT_THROW(f24.convert_precision<16>(), std::overflow_error);
}

TEST_F(DecimalTest, ConvertPrecision_Signed) {
    decimal::I8 f0("1.12345678");
    decimal::I2 f1 = f0.convert_precision<2>();
    ASSERT_EQ(f1.to_string(), "1.12");

    decimal::I8 f2("100");
    decimal::I2 f3 = f2.convert_precision<2>();
    ASSERT_EQ(f3.to_string(), "100");

    decimal::I2 f4("1.12");
    decimal::I8 f5 = f4.convert_precision<8>();
    ASSERT_EQ(f5.to_string(), "1.12");

    decimal::I8 f6("1.12345678");
    decimal::I8 f7 = f6.convert_precision<8>();
    ASSERT_EQ(f7.to_string(), "1.12345678");

    decimal::I8 f8("1e8");
    decimal::I2 f9 = f8.convert_precision<2>();
    ASSERT_EQ(f9.to_string(), "100000000");

    decimal::I8 f10("0.000000012345678");
    decimal::I4 f11 = f10.convert_precision<4>();
    ASSERT_EQ(f11.to_string(), "0");

    decimal::I8 f12("1.126");
    decimal::I2 f13 = f12.convert_precision<2>();
    ASSERT_EQ(f13.to_string(), "1.13");

    decimal::I8 f14("1.124");
    decimal::I2 f15 = f14.convert_precision<2>();
    ASSERT_EQ(f15.to_string(), "1.12");

    decimal::I1 f16("12345678901234567");
    decimal::I2 f17 = f16.convert_precision<2>();
    ASSERT_EQ(f16.to_string(), f17.to_string());

    decimal::I17 f18("0.000000000000000001");
    decimal::I1 f19 = f18.convert_precision<1>();
    ASSERT_EQ(f19.to_string(), "0");

    decimal::I1 f20("12345678901234567");
    ASSERT_THROW(f20.convert_precision<17>(), std::overflow_error);

    decimal::I1 f22("12345678901234567");
    ASSERT_THROW(f22.convert_precision<3>(), std::overflow_error);

    decimal::I6 f24("12345.6789");
    ASSERT_THROW(f24.convert_precision<16>(), std::overflow_error);

    decimal::I8 nf0("-1.12345678");
    decimal::I2 nf1 = nf0.convert_precision<2>();
    ASSERT_EQ(nf1.to_string(), "-1.12");

    decimal::I8 nf2("-100");
    decimal::I2 nf3 = nf2.convert_precision<2>();
    ASSERT_EQ(nf3.to_string(), "-100");

    decimal::I2 nf4("-1.12");
    decimal::I8 nf5 = nf4.convert_precision<8>();
    ASSERT_EQ(nf5.to_string(), "-1.12");

    decimal::I8 nf6("-1.12345678");
    decimal::I8 nf7 = nf6.convert_precision<8>();
    ASSERT_EQ(nf7.to_string(), "-1.12345678");

    decimal::I8 nf8("-1e8");
    decimal::I2 nf9 = nf8.convert_precision<2>();
    ASSERT_EQ(nf9.to_string(), "-100000000");

    decimal::I8 nf10("-0.000000012345678");
    decimal::I4 nf11 = nf10.convert_precision<4>();
    ASSERT_EQ(nf11.to_string(), "0");

    decimal::I8 nf12("-1.126");
    decimal::I2 nf13 = nf12.convert_precision<2>();
    ASSERT_EQ(nf13.to_string(), "-1.13");

    decimal::I8 nf14("-1.124");
    decimal::I2 nf15 = nf14.convert_precision<2>();
    ASSERT_EQ(nf15.to_string(), "-1.12");

    decimal::I1 nf16("-12345678901234567");
    decimal::I2 nf17 = nf16.convert_precision<2>();
    ASSERT_EQ(nf16.to_string(), nf17.to_string());

    decimal::I17 nf18("-0.000000000000000001");
    decimal::I1 nf19 = nf18.convert_precision<1>();
    ASSERT_EQ(nf19.to_string(), "0");

    decimal::I1 nf20("-12345678901234567");
    ASSERT_THROW(nf20.convert_precision<17>(), std::overflow_error);

    decimal::I1 nf22("-12345678901234567");
    ASSERT_THROW(nf22.convert_precision<3>(), std::overflow_error);

    decimal::I6 nf24("-12345.6789");
    ASSERT_THROW(nf24.convert_precision<16>(), std::overflow_error);
}

/* ---- */

class DecimalEncodeDecodeTest : public ::testing::Test {
   protected:
    std::vector<uint8_t> extra_data{42, 24, 0};

    template <int nPlaces, decimal::Type S>
    void RunEncodeDecodeTest() {
        decimal::Decimal<nPlaces, S> original(123.456789);
        std::vector<uint8_t> binary_data = original.encode_binary();

        decimal::Decimal<nPlaces, S> result;
        size_t offset = 0;
        result.decode_binary(binary_data, offset);

        ASSERT_EQ(original, result);
        ASSERT_GT(offset, 0);

        std::vector<uint8_t> binary_vec{33};
        size_t vec_offset = 1;
        original.encode_binary(binary_vec, vec_offset);

        binary_vec.insert(binary_vec.end(), extra_data.begin(), extra_data.end());

        std::vector<uint8_t> expected{33};
        expected.insert(expected.end(), binary_data.begin(), binary_data.end());
        expected.insert(expected.end(), extra_data.begin(), extra_data.end());

        ASSERT_EQ(binary_vec, expected);
    }

    template <int nPlaces, decimal::Type S>
    void RunDecodeBinaryDataTest() {
        decimal::Decimal<nPlaces, S> original(987.654321);
        std::vector<uint8_t> binary_data = original.encode_binary();
        binary_data.insert(binary_data.end(), extra_data.begin(), extra_data.end());

        decimal::Decimal<nPlaces, S> result;
        size_t offset = result.decode_binary_data(binary_data);

        std::vector<uint8_t> remaining_data{binary_data.begin() + offset, binary_data.end()};

        ASSERT_EQ(original, result);
        ASSERT_EQ(extra_data, remaining_data);
    }
};

TEST_F(DecimalEncodeDecodeTest, EncodeDecode) {
    RunEncodeDecodeTest<3, decimal::Unsigned>();
    RunEncodeDecodeTest<6, decimal::Unsigned>();
    RunEncodeDecodeTest<8, decimal::Unsigned>();
    RunEncodeDecodeTest<11, decimal::Unsigned>();

    RunEncodeDecodeTest<3, decimal::Signed>();
    RunEncodeDecodeTest<6, decimal::Signed>();
    RunEncodeDecodeTest<8, decimal::Signed>();
    RunEncodeDecodeTest<11, decimal::Signed>();
}

TEST_F(DecimalEncodeDecodeTest, DecodeBinaryData) {
    RunDecodeBinaryDataTest<3, decimal::Unsigned>();
    RunDecodeBinaryDataTest<6, decimal::Unsigned>();
    RunDecodeBinaryDataTest<8, decimal::Unsigned>();
    RunDecodeBinaryDataTest<11, decimal::Unsigned>();

    RunDecodeBinaryDataTest<3, decimal::Signed>();
    RunDecodeBinaryDataTest<6, decimal::Signed>();
    RunDecodeBinaryDataTest<8, decimal::Signed>();
    RunDecodeBinaryDataTest<11, decimal::Signed>();
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
