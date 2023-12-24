# cpp-udecimal

A fixed place signed/unsigned numeric library. Unsigned by default.

## Usage

```
// 8 decimal places, unsigned
auto f = udecimal::U8("123.456"); // 123.456
auto f = udecimal::U8(123456, 3); // 123.456
auto f = udecimal::U8::FromExp(123456, -3); // 123.456
auto f = udecimal::U8(123.456); // 123.456

// 9 decimal places, unsigned
auto f = udecimal::U9("123.456"); // 123.456
auto f = udecimal::U9(123456, 3); // 123.456
auto f = udecimal::U9::FromExp(123456, -3); // 123.456
auto f = udecimal::U9(123.456); // 123.456

// 9 decimal places, signed
auto f = udecimal::I9("123.456"); // 123.456
auto f = udecimal::I9(123456, 3); // 123.456
auto f = udecimal::I9::FromExp(123456, -3); // 123.456
auto f = udecimal::I9(123.456); // 123.456

// Convert from one precision level to another
auto g = f.convert_precision<2>(); // 123.45

// Alternatively, 8 decimal places, unsigned
auto f = udecimal::Decimal<8, udecimal::Unsigned>("123.456"); // 123.456

// Alternatively, 8 decimal places, signed
auto f = udecimal::Decimal<8, udecimal::Signed>("123.456"); // 123.456
```
