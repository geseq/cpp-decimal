# cpp-udecimal

A fixed place unsigned numeric library

## Usage

```
// 8 decimal places
auto f = Decimal<8>("123.456"); // 123.456
auto f = Decimal<>(123456, 3); // 123.456
auto f = Decimal<>::FromExp(123456, -3); // 123.456
auto f = Decimal<>(123.456); // 123.456

// Convert from one precision level to another
auto g = f.convert<2>(); // 123.45
```
