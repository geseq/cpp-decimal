#ifndef CPP_DECIMAL_H
#define CPP_DECIMAL_H

#include <sys/types.h>

#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cwchar>
#include <iostream>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <vector>

// The decimal namespace provides functionalities for handling decimal arithmetic with
// variable precision. It includes templates for signed and unsigned decimal types.
namespace decimal {

#if defined(__GNUC__) || defined(__clang__)
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

enum Type {
    Signed,
    Unsigned
};

namespace detail {
template <typename T, typename = void>
struct has_int128_impl : std::false_type {};

template <typename T>
struct has_int128_impl<T, std::void_t<decltype(T(1) * T(1))>> : std::true_type {};

constexpr bool has_int128 = has_int128_impl<__int128>::value;

template <int base, int exponent>
constexpr uint64_t const_pow() {
    static_assert(base > 0, "Base must be positive");
    static_assert(exponent >= 0, "Exponent cannot be negative");
    uint64_t result = 1;
    for (int i = 0; i < exponent; ++i) {
        result *= base;
    }
    return result;
}

template <Type T>
struct IntTypeMap;

template <>
struct IntTypeMap<Signed> {
    using type = int64_t;
};

template <>
struct IntTypeMap<Unsigned> {
    using type = uint64_t;
};

template <typename T>
inline T precomputed_pow_10(unsigned int exponent);

template <>
inline uint64_t precomputed_pow_10<uint64_t>(unsigned int exponent) {
    constexpr std::array<uint64_t, 20> powers_of_10 = {1,
                                                       10,
                                                       100,
                                                       1000,
                                                       10000,
                                                       100000,
                                                       1000000,
                                                       10000000,
                                                       100000000,
                                                       1000000000,
                                                       10000000000,
                                                       100000000000,
                                                       1000000000000,
                                                       10000000000000,
                                                       100000000000000,
                                                       1000000000000000,
                                                       10000000000000000,
                                                       100000000000000000,
                                                       1000000000000000000,
                                                       10000000000000000000UL};

    constexpr unsigned int max_exponent = powers_of_10.size() - 1;
    if (unlikely(exponent > max_exponent)) {
        throw std::invalid_argument("invalid exponent for unsigned decimal");
    }

    return powers_of_10[exponent];
}

template <>
inline int64_t precomputed_pow_10<int64_t>(unsigned int exponent) {
    constexpr std::array<int64_t, 19> powers_of_10 = {1,
                                                      10,
                                                      100,
                                                      1000,
                                                      10000,
                                                      100000,
                                                      1000000,
                                                      10000000,
                                                      100000000,
                                                      1000000000,
                                                      10000000000,
                                                      100000000000,
                                                      1000000000000,
                                                      10000000000000,
                                                      100000000000000,
                                                      1000000000000000,
                                                      10000000000000000,
                                                      100000000000000000,
                                                      1000000000000000000};

    constexpr unsigned int max_exponent = powers_of_10.size() - 1;
    if (unlikely(exponent > max_exponent)) {
        throw std::invalid_argument("invalid exponent for signed decimal");
    }

    return powers_of_10[exponent];
}

}  // namespace detail

// Decimal is a decimal precision for signed and unsigned numbers (defaults to 11.8 digits unsigned).
template <int nPlaces = 8, Type S = Unsigned>
class Decimal {
   private:
    static constexpr double computeMax() {
        return static_cast<double>(detail::const_pow<10, (digits - nPlaces)>() - 1) + (static_cast<double>(scale - 1) / static_cast<double>(scale));
    }

    static constexpr double computeMin() {
        if constexpr (S == Signed) {
            return -computeMax();
        }
        return 0;
    }

   public:
    using IntType = typename detail::IntTypeMap<S>::type;

    IntType fp = 0;

    explicit Decimal(int i) { fp = static_cast<IntType>(i) * scale; }

    Decimal(IntType fp = 0) : fp(fp) {}

    static constexpr IntType scale = detail::const_pow<10, nPlaces>();
    static constexpr int digits = std::numeric_limits<IntType>::digits10;
    static constexpr double MAX = computeMax();
    static constexpr double MIN = computeMin();

    static_assert(nPlaces < digits);
    static_assert(nPlaces > 0);

    inline static std::string zeros() { return {std::string(nPlaces, '0')}; }

    static const std::runtime_error errDivByZero;
    static const std::overflow_error errTooLarge;
    static const std::overflow_error errOverflow;
    static const std::invalid_argument errInvalidInput;

    Decimal(const Decimal<nPlaces>& other) : fp(other.fp) {}

    // Creates a Decimal from a double, rounding at the nth place
    Decimal(double f) {
        if (unlikely(std::isnan(f))) {
            throw errInvalidInput;
        }

        double round = 0.5;

        if (S == Signed) {
            if (unlikely(f >= MAX || f <= MIN)) {
                throw errOverflow;
            }

            if (f < 0) {
                round = -0.5;
            }
        } else {
            if (unlikely(f >= MAX || f < 0)) {
                throw errOverflow;
            }
        }

        fp = static_cast<IntType>(f * scale + round);
    }

    // Creates a Decimal for an integer, moving the decimal point n places to the left
    // For example, Decimal(123,1) becomes 12.3. If n > 7, the value is truncated
    Decimal(IntType i, uint32_t n) { fp = newI(i, n); }

    Decimal(const std::string& s) {
        // TODO: optimize string parsing algorithm
        if (s.find_first_of("eE") != std::string::npos) {
            double f = std::stod(s);
            fp = static_cast<IntType>(f * scale);
            return;
        }

        std::size_t period = s.find('.');
        IntType i = 0, f = 0;
        if (period == std::string::npos) {
            i = parseInteger(s);
            if constexpr (S == Signed) {
                if (i > 0 && unlikely(i > MAX)) {
                    throw errTooLarge;
                } else if (i < 0 && unlikely(-i > MAX)) {
                    throw errTooLarge;
                }
            } else {
                if (unlikely(i > MAX)) {
                    throw errTooLarge;
                }
            }
            fp = i * scale;
        } else {
            bool negative = false;
            if (period > 0) {
                if constexpr (S == Signed) {
                    if (s[0] == '-') {
                        negative = true;
                    }

                    if (negative && period == 1) {
                        i = 0;
                    } else {
                        i = parseInteger(s.substr(0, period));
                        if (i > 0 && unlikely(i > MAX)) {
                            throw errTooLarge;
                        } else if (i < 0 && unlikely(-i > MAX)) {
                            throw errTooLarge;
                        }
                    }
                } else {
                    i = parseInteger(s.substr(0, period));
                    if (unlikely(i > MAX)) {
                        throw errTooLarge;
                    }
                }
            }
            std::string fs = s.substr(period + 1);
            fs += std::string(max(0, static_cast<int>(nPlaces - fs.length())), '0');
            f = parseInteger(fs.substr(0, nPlaces));
            if (i < 0 || negative) {
                fp = i * scale - f;
            } else {
                fp = i * scale + f;
            }
        }
    }

    // New returns a new fixed-point decimal, value * 10 ^ exp.
    static Decimal FromExp(IntType value, int exp) {
        if (exp >= 0) {
            auto exp_mul = detail::precomputed_pow_10<IntType>(exp);
            return {mul(newI(value, 0), newI(exp_mul, 0))};
        }

        return {newI(value, static_cast<unsigned int>(-exp))};
    }

    [[nodiscard]] bool is_zero() const { return fp == 0; }

    // Float converts the Decimal to a float64
    [[nodiscard]] double to_double() const { return static_cast<double>(fp) / scale; }

    // Add adds f0 to f producing a Decimal.
    Decimal operator+(const Decimal& f0) const {
        if constexpr (S == Signed) {
            if (unlikely(fp > 0 && f0.fp > 0 && fp > MAX - f0.fp)) {
                throw errOverflow;
            }
            if (unlikely(fp < 0 && f0.fp < 0 && fp < MIN - f0.fp)) {
                throw errOverflow;
            }
            IntType sum = fp + f0.fp;
            return {sum};
        } else {
            if (unlikely(f0.fp > MAX - fp)) {
                throw errOverflow;
            }
            return {fp + f0.fp};
        }
    }

    // Adds f0 to the current Decimal object.
    Decimal& operator+=(const Decimal& f0) {
        if (unlikely(f0.fp > UINT64_MAX - fp)) {
            throw errOverflow;
        }
        fp += f0.fp;
        return *this;
    }

    // Sub subtracts f0 from f producing a Decimal.
    Decimal operator-(const Decimal& f0) const {
        if constexpr (S == Signed) {
            if (unlikely(fp > 0 && f0.fp < 0 && fp - f0.fp > MAX)) {
                throw errOverflow;
            }
            if (unlikely(fp < 0 && f0.fp > 0 && fp - f0.fp < MIN)) {
                throw errOverflow;
            }
            return {fp - f0.fp};
        } else {
            if (unlikely(fp < f0.fp)) {
                throw errOverflow;
            }
            return {fp - f0.fp};
        }
    }

    // Subtracts f0 from the current Decimal object.
    Decimal& operator-=(const Decimal& f0) {
        if (unlikely(fp < f0.fp)) {
            throw errOverflow;
        }
        fp -= f0.fp;
        return *this;
    }

    Decimal operator*(const Decimal& f0) { return {mul(fp, f0.fp)}; }

    // multiplies the current Decimal object by f0.
    Decimal& operator*=(const Decimal& f0) {
        fp = mul(fp, f0.fp);
        return *this;
    }

    Decimal operator/(const Decimal& f0) const { return {div(fp, f0.fp)}; }

    // divides the current Decimal object by f0.
    Decimal& operator/=(const Decimal& f0) {
        fp = div(fp, f0.fp);
        return *this;
    }

    bool operator==(const Decimal& rhs) const { return fp == rhs.fp; }
    bool operator!=(const Decimal& rhs) const { return fp != rhs.fp; }
    bool operator<(const Decimal& rhs) const { return fp < rhs.fp; }
    bool operator<=(const Decimal& rhs) const { return fp <= rhs.fp; }
    bool operator>(const Decimal& rhs) const { return fp > rhs.fp; }
    bool operator>=(const Decimal& rhs) const { return fp >= rhs.fp; }

    [[nodiscard]] std::string to_string() const {
        std::string s = to_str();

        std::size_t point = s.find('.');
        if (point == std::string::npos) {
            return s;
        }

        std::size_t index = s.length() - 1;
        while (index != point) {
            if (s[index] != '0') {
                return s.substr(0, index + 1);
            }
            index--;
        }

        return s.substr(0, point);
    }

    [[nodiscard]] std::string to_string(int decimals) const {
        std::string s = to_str();

        std::size_t point = s.find('.');
        if (point == std::string::npos) {
            return s;
        }

        if (decimals == 0) {
            return s.substr(0, point);
        } else {
            return s.substr(0, point + decimals + 1);
        }
    }

    [[nodiscard]] IntType to_int() const { return fp / scale; }

    [[nodiscard]] double to_frac() const { return static_cast<double>(fp % scale) / scale; }

    [[nodiscard]] Decimal round(int n) const {
        if (n >= nPlaces) {
            return *this;
        }

        if (n == 0) {
            return fp - (fp % scale);
        }

        IntType frac = fp % scale;
        IntType f0 = fp - frac;

        auto pow = detail::precomputed_pow_10<IntType>(nPlaces - n);
        if constexpr (S == Signed) {
            if (fp < 0) {
                frac -= pow / 2;  // rounding factor
            } else {
                frac += pow / 2;  // rounding factor
            }
        } else {
            frac += pow / 2;  // rounding factor
        }
        frac /= pow;
        frac *= pow;

        IntType new_fp = f0 + frac;

        return {new_fp};
    }

    // convert_precision allows converting a Decimal from one precision to another.
    // A conversion  moving the number of places right, will just be a lossy conversion
    // A conversion moving the number of places left will throw an overflow error
    // if it is not possible
    template <int toPlaces>
    Decimal<toPlaces, S> convert_precision() const {
        if constexpr (toPlaces == nPlaces) {
            return Decimal<toPlaces, S>(*this);
        } else if constexpr (toPlaces < nPlaces) {
            if constexpr (detail::has_int128) {
                return Decimal<toPlaces, S>(div(fp, scale), nPlaces);
            } else {
                static constexpr IntType factor = scale / detail::const_pow<10, toPlaces>();
                return Decimal<toPlaces, S>(fp / factor);
            }
        } else {
            static constexpr IntType factor = detail::const_pow<10, toPlaces>() / scale;
            if (unlikely(fp > std::numeric_limits<IntType>::max() / factor)) {
                throw errOverflow;
            }
            if constexpr (S == Signed) {
                if (unlikely(fp < std::numeric_limits<IntType>::min() / factor)) {
                    throw errOverflow;
                }
            }
            return Decimal<toPlaces, S>(fp * factor);
        }
    }

    // decode_binary reads from a byte vector and sets the Decimal value
    // It also updates the offset.
    void decode_binary(const std::vector<uint8_t>& data, size_t& offset) {
        IntType value = 0;
        int shift = 0;
        for (; offset < data.size() - 1; ++offset) {
            uint8_t byte = data[offset];
            value |= static_cast<IntType>(byte & 0x7F) << shift;
            if ((byte & 0x80) == 0) {
                break;
            }
            shift += 7;
        }

        int extracted_nPlaces = data[offset + 1];
        if (extracted_nPlaces != nPlaces) {
            if (extracted_nPlaces > nPlaces) {
                value /= detail::precomputed_pow_10<IntType>(extracted_nPlaces - nPlaces);
            } else {
                value *= detail::precomputed_pow_10<IntType>(nPlaces - extracted_nPlaces);
            }
        }

        fp = value;
        offset += 2;  // We also read one extra byte for the 'nPlaces' information
    }

    // decode_binary_data reads from a byte vector, sets the Decimal value, and returns the new offset
    size_t decode_binary_data(const std::vector<uint8_t>& data, size_t offset = 0) {
        decode_binary(data, offset);
        return offset;  // offset has been updated in decode_binary
    }

    // encode_binary serializes the Decimal value into a byte vector and updates the offset.
    void encode_binary(std::vector<uint8_t>& data, size_t& offset) const {
        IntType value = fp;
        do {
            uint8_t byte = value & 0x7F;
            value >>= 7;
            if (value != 0) {
                byte |= 0x80;
            }

            // Ensure that the vector can accommodate the new byte
            if (offset >= data.size()) {
                data.resize(offset + 1);
            }
            data[offset] = byte;
            ++offset;
        } while (value != 0);

        // Add nPlaces as an additional byte at the end
        if (offset >= data.size()) {
            data.resize(offset + 1);
        }
        data[offset] = static_cast<uint8_t>(nPlaces);
        ++offset;
    }

    // Overloaded version of encode_binary that creates a new vector
    [[nodiscard]] std::vector<uint8_t> encode_binary() const {
        std::vector<uint8_t> data;
        size_t offset = 0;
        encode_binary(data, offset);
        return data;
    }

   private:
    IntType parseInteger(const std::string& s) {
        if constexpr (S == Signed) {
            return std::stoll(s);
        } else {
            return std::stoull(s);
        }
    }

    static IntType mul(IntType fp, IntType f0) {
        IntType fp_a = fp / scale;
        IntType fp_b = fp % scale;

        IntType fp0_a = f0 / scale;
        IntType fp0_b = f0 % scale;

        IntType result = 0;

        if (fp0_a != 0) {
            result = fp_a * fp0_a;
            if (unlikely(static_cast<double>(result) > MAX)) {
                throw errOverflow;
            }
            result = (result * scale) + (fp_b * fp0_a);
        }

        if (fp0_b != 0) {
            result = result + (fp_a * fp0_b) + (fp_b * fp0_b) / scale;
        }

        return result;
    }

    using DivT = typename std::conditional<detail::has_int128, IntType, double>::type;

    static __int128 abs128(__int128 x) { return x < 0 ? -x : x; }

    static DivT div(IntType fp, IntType f0) {
        if (unlikely(f0 == 0)) {
            throw errDivByZero;
        }

        if constexpr (detail::has_int128) {
            __int128 temp = static_cast<__int128>(fp) * scale;
            __int128 remainder = temp % f0;
            temp /= f0;

            if (unlikely(temp > std::numeric_limits<IntType>::max())) {
                throw errOverflow;
            }

            auto quotient = static_cast<IntType>(temp);

            if constexpr (S == Signed) {
                if (abs128(remainder) * 2 >= abs128(f0)) {
                    // Adjust quotient based on the sign of fp and f0
                    quotient += (fp ^ f0) < 0 ? -1 : 1;
                }
            } else {
                if (remainder * 2 >= f0) {
                    ++quotient;  // rounding factor
                }
            }
            return quotient;
        } else {
            return double(fp) / double(f0);
        }
    }

    static IntType newI(IntType i, uint32_t n) {
        if (n > nPlaces) {
            auto pow = detail::precomputed_pow_10<IntType>(n - nPlaces);
            if constexpr (S == Signed) {
                if (i < 0) {
                    i -= pow / 2;  // rounding factor
                } else {
                    i += pow / 2;  // rounding factor
                }
            } else {
                i += pow / 2;  // rounding factor
            }
            i /= pow;
            n = nPlaces;
        }
        i *= detail::precomputed_pow_10<IntType>(nPlaces - n);
        return i;
    }

    [[nodiscard]] std::string to_str() const {
        if (fp == 0) {
            static const std::string zero_str = "0." + zeros();
            return zero_str;
        }

        IntType val = fp;
        if constexpr (S == Signed) {
            if (fp < 0) {
                val = -fp;
            }
        }

        std::array<char, 25> buf;
        int i = sizeof(buf) - 1;
        int idec = i - nPlaces;

        while (val >= 10 || i >= idec) {
            buf[i] = static_cast<char>(val % 10 + '0');
            i--;
            if (i == idec) {
                buf[i] = '.';
                i--;
            }
            val /= 10;
        }

        buf[i] = static_cast<char>(val + '0');

        if constexpr (S == Signed) {
            if (fp < 0) {
                i--;
                buf[i] = '-';
            }
        }

        return {std::next(buf.begin(), i), buf.end()};
    }

    static int max(int a, int b) { return (a > b) ? a : b; }
};

template <int nPlaces, Type S>
const std::runtime_error Decimal<nPlaces, S>::errDivByZero("division by zero");
template <int nPlaces, Type S>
const std::overflow_error Decimal<nPlaces, S>::errTooLarge("number is too large");
template <int nPlaces, Type S>
const std::overflow_error Decimal<nPlaces, S>::errOverflow("decimal overflow");
template <int nPlaces, Type S>
const std::invalid_argument Decimal<nPlaces, S>::errInvalidInput("invalid input");

template <int nPlaces, Type S>
std::ostream& operator<<(std::ostream& os, const Decimal<nPlaces, S>& d) {
    os << d.to_string();
    return os;
}

// Unsigned
using U1 = Decimal<1, Unsigned>;
using U2 = Decimal<2, Unsigned>;
using U3 = Decimal<3, Unsigned>;
using U4 = Decimal<4, Unsigned>;
using U5 = Decimal<5, Unsigned>;
using U6 = Decimal<6, Unsigned>;
using U7 = Decimal<7, Unsigned>;
using U8 = Decimal<8, Unsigned>;
using U9 = Decimal<9, Unsigned>;
using U10 = Decimal<10, Unsigned>;
using U11 = Decimal<11, Unsigned>;
using U12 = Decimal<12, Unsigned>;
using U13 = Decimal<13, Unsigned>;
using U14 = Decimal<14, Unsigned>;
using U15 = Decimal<15, Unsigned>;
using U16 = Decimal<16, Unsigned>;
using U17 = Decimal<17, Unsigned>;
using U18 = Decimal<18, Unsigned>;

// Signed
using I1 = Decimal<1, Signed>;
using I2 = Decimal<2, Signed>;
using I3 = Decimal<3, Signed>;
using I4 = Decimal<4, Signed>;
using I5 = Decimal<5, Signed>;
using I6 = Decimal<6, Signed>;
using I7 = Decimal<7, Signed>;
using I8 = Decimal<8, Signed>;
using I9 = Decimal<9, Signed>;
using I10 = Decimal<10, Signed>;
using I11 = Decimal<11, Signed>;
using I12 = Decimal<12, Signed>;
using I13 = Decimal<13, Signed>;
using I14 = Decimal<14, Signed>;
using I15 = Decimal<15, Signed>;
using I16 = Decimal<16, Signed>;
using I17 = Decimal<17, Signed>;

}  // namespace decimal

#endif  // CPP_DECIMAL_H
