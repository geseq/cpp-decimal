# cpp-decimal

A fixed place signed/unsigned numeric library. Unsigned by default.

## Usage

### Basic Usage
```cpp
#include "decimal.h"

using namespace decimal;

int main() {
    U8 a(123.456);  // Unsigned Decimal with 8 decimal places
    U8 b("789.012");
    U8 c = a + b;

    std::cout << "Result: " << c.to_string() << std::endl;
    return 0;
}
```

### Arithmetic Operations
```cpp
U8 x(15.75);
U8 y(2.5);
U8 z = x * y; // z is now 39.375
```

### Conversion and Comparison
```cpp
I8 m("-123.456"); // Signed Decimal
double n = m.to_double(); // Convert to double

if (m < n) {
    std::cout << "m is less than n" << std::endl;
}
```

### More examplease
```cpp
// 8 decimal places, unsigned
auto f = decimal::U8("123.456"); // 123.456
auto f = decimal::U8(123456, 3); // 123.456
auto f = decimal::U8::FromExp(123456, -3); // 123.456
auto f = decimal::U8(123.456); // 123.456

// 9 decimal places, unsigned
auto f = decimal::U9("123.456"); // 123.456
auto f = decimal::U9(123456, 3); // 123.456
auto f = decimal::U9::FromExp(123456, -3); // 123.456
auto f = decimal::U9(123.456); // 123.456

// 9 decimal places, signed
auto f = decimal::I9("123.456"); // 123.456
auto f = decimal::I9(123456, 3); // 123.456
auto f = decimal::I9::FromExp(123456, -3); // 123.456
auto f = decimal::I9(123.456); // 123.456

// Convert from one precision level to another
auto g = f.convert_precision<2>(); // 123.45

// Alternatively, 8 decimal places, unsigned
auto f = decimal::Decimal<8, decimal::Unsigned>("123.456"); // 123.456

// Alternatively, 8 decimal places, signed
auto f = decimal::Decimal<8, decimal::Signed>("123.456"); // 123.456
```

## Constructors and Initialization
- **Default Constructor**: `Decimal()` - Initializes a new Decimal with a default value of 0.
- **From Integer**: `Decimal(IntType value)` - Initializes a Decimal with an integer value, scaling it to the decimal precision.
- **From Double**: `Decimal(double value)` - Initializes a Decimal from a floating-point number, rounding it to the specified precision.
- **From String**: `Decimal(const std::string& value)` - Parses and initializes a Decimal from a string representation.

## Static Constants
- `scale`: Represents the scaling factor based on the number of decimal places.
- `digits`: The number of digits in the base integral type.
- `MAX`: The maximum value that the Decimal can represent.
- `MIN`: The minimum value that the Decimal can represent (relevant for signed decimals).

## Methods
- `to_double()`: Converts the Decimal to a floating-point number.
- `to_string()`: Converts the Decimal to its string representation.
- `to_int()`: Converts the Decimal to an integer, truncating the decimal part.
- `is_zero()`: Checks if the Decimal value is zero.


