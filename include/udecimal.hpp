#include <sys/types.h>

#include <array>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <cwchar>
#include <iostream>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

namespace udecimal {

#if defined(__GNUC__) || defined(__clang__)
#define likely(x) __builtin_expect(!!(x), 1)
#define unlikely(x) __builtin_expect(!!(x), 0)
#else
#define likely(x) (x)
#define unlikely(x) (x)
#endif

template <typename T, typename = void>
struct has_int128_impl : std::false_type {};

template <typename T>
struct has_int128_impl<T, decltype(sizeof(T(1) * T(1) == T(1)))> : std::true_type {};

constexpr bool has_int128 = has_int128_impl<__int128>::value;

template <int base, int exponent>
constexpr uint64_t const_pow() {
    uint64_t result = 1;
    for (int i = 0; i < exponent; ++i) {
        result *= base;
    }
    return result;
}

uint64_t precomputed_pow_10(unsigned int exponent) {
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

    if (unlikely(exponent >= powers_of_10.size())) {
        throw std::invalid_argument("invalid exponent");
    }

    return powers_of_10[exponent];
}

// Decimal is a decimal precision unsigned number (defaults to 11.8 digits).
// IMPORTANT: this is not designed to be used for signed decimals.
template <int nPlaces = 8>
class Decimal {
   public:
    uint64_t fp = 0;

    Decimal(uint64_t fp = 0) : fp(fp) {}

    static constexpr uint64_t scale = const_pow<10, nPlaces>();
    static constexpr int digits = std::numeric_limits<uint64_t>::digits10;
    static constexpr double MAX = static_cast<double>(const_pow<10, (digits - nPlaces)>() - 1) + (static_cast<double>(scale - 1) / static_cast<double>(scale));

    static_assert(nPlaces < digits);
    static_assert(nPlaces > 0);

    inline static std::string zeros() { return {std::string(nPlaces, '0')}; }

    static const std::runtime_error errDivByZero;
    static const std::overflow_error errTooLarge;
    static const std::overflow_error errOverflow;
    static const std::invalid_argument errInvalidInput;

    Decimal(const Decimal<nPlaces>& other) : fp(other.fp) {}

    // Creates a Decimal from a double, rounding at the 8th decimal place
    Decimal(double f) {
        if (unlikely(std::isnan(f))) {
            throw errInvalidInput;
        }
        if (unlikely(f >= MAX || f < 0)) {
            throw errInvalidInput;
        }
        double round = 0.5;
        if (f < 0) {
            round = -0.5;
        }
        fp = static_cast<uint64_t>(f * scale + round);
    }

    // Creates a Decimal for an integer, moving the decimal point n places to the left
    // For example, NewI(123,1) becomes 12.3. If n > 7, the value is truncated
    Decimal(uint64_t i, uint32_t n) { fp = newI(i, n); }

    Decimal(const std::string& s) {
        if (s.find_first_of("eE") != std::string::npos) {
            double f = std::stod(s);
            fp = static_cast<uint64_t>(f * scale);
            return;
        }

        std::size_t period = s.find('.');
        uint64_t i = 0, f = 0;
        if (period == std::string::npos) {
            i = std::stoull(s);
            if (unlikely(i > MAX)) {
                throw errTooLarge;
            }
            fp = i * scale;
        } else {
            if (period > 0) {
                i = std::stoull(s.substr(0, period));
                if (unlikely(i > MAX)) {
                    throw errTooLarge;
                }
            }
            std::string fs = s.substr(period + 1);
            fs += std::string(max(0, static_cast<int>(nPlaces - fs.length())), '0');
            f = std::stoull(fs.substr(0, nPlaces));
            fp = i * scale + f;
        }
    }

    // New returns a new fixed-point decimal, value * 10 ^ exp.
    static Decimal FromExp(uint64_t value, int exp) {
        if (exp >= 0) {
            uint64_t exp_mul = precomputed_pow_10(exp);
            return {mul(newI(value, 0), newI(exp_mul, 0))};
        }

        return {newI(value, static_cast<unsigned int>(-exp))};
    }

    [[nodiscard]] bool is_zero() const { return fp == 0; }

    // Float converts the Decimal to a float64
    [[nodiscard]] double to_double() const { return static_cast<double>(fp) / scale; }

    // Add adds f0 to f producing a Decimal.
    Decimal operator+(const Decimal& f0) const {
        if (unlikely(f0.fp > UINT64_MAX - fp)) {
            throw errOverflow;
        }
        return Decimal{fp + f0.fp};
    }

    // Sub subtracts f0 from f producing a Decimal.
    Decimal operator-(const Decimal& f0) const {
        if (unlikely(fp < f0.fp)) {
            throw errOverflow;
        }
        return Decimal{fp - f0.fp};
    }

    Decimal operator*(const Decimal& f0) { return Decimal{mul(fp, f0.fp)}; }

    Decimal operator/(const Decimal& f0) const {
        if constexpr (has_int128) {
            if (f0.fp == 0) {
                throw errDivByZero;
            }

            __int128 temp = static_cast<__int128>(fp) * scale;
            __int128 remainder = temp % f0.fp;
            temp /= f0.fp;

            if (temp > std::numeric_limits<uint64_t>::max()) {
                throw errOverflow;
            }

            auto quotient = static_cast<uint64_t>(temp);

            if (remainder * 2 >= f0.fp) {
                quotient++;
            }

            return Decimal(quotient);
        } else {
            double quotient = to_double() / f0.to_double();
            return Decimal(quotient);
        }
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

    [[nodiscard]] uint64_t to_int() const { return fp / scale; }

    [[nodiscard]] double to_frac() const { return static_cast<double>(fp % scale) / scale; }

    [[nodiscard]] Decimal round(int n) const {
        double round = 0.5;
        if (fp < 0) {
            round = -0.5;
        }

        double f0 = to_frac();
        f0 = f0 * pow(10, n) + round;
        f0 = double(int(f0)) / pow(10, n);

        return {double(to_int()) + f0};
    }

    // convert_precision allows converting a Decimal from one precision to another.
    // A conversion  moving the number of places right, will just be a lossy conversion
    // A conversion moving the number of places left will throw an overflow error
    // if it is not possible
    template <int toPlaces>
    Decimal<toPlaces> convert_precision() const {
        if constexpr (toPlaces == nPlaces) {
            return Decimal<toPlaces>(*this);
        } else if constexpr (toPlaces < nPlaces) {
            static constexpr uint64_t factor = scale / const_pow<10, toPlaces>();
            return Decimal<toPlaces>(fp / factor);
        } else {
            static constexpr uint64_t factor = const_pow<10, toPlaces>() / scale;
            if (unlikely(fp > std::numeric_limits<uint64_t>::max() / factor)) {
                throw errOverflow;
            }
            return Decimal<toPlaces>(fp * factor);
        }
    }

    // decode_binary reads from a byte vector and sets the Decimal value
    void decode_binary(const std::vector<uint8_t>& data) {
        uint64_t value = 0;
        int shift = 0;
        size_t index = 0;
        for (; index < data.size() - 1; ++index) {
            uint8_t byte = data[index];
            value |= static_cast<uint64_t>(byte & 0x7F) << shift;
            if ((byte & 0x80) == 0) {
                break;
            }
            shift += 7;
        }

        int extracted_nPlaces = data[index + 1];
        if (extracted_nPlaces != nPlaces) {
            if (extracted_nPlaces > nPlaces) {
                value /= pow(10, extracted_nPlaces - nPlaces);
            } else {
                value *= pow(10, nPlaces - extracted_nPlaces);
            }
        }

        fp = value;
    }

    // decode_binary_data reads from a byte vector, sets the Decimal value, and returns the remaining data
    std::vector<uint8_t> decode_binary_data(const std::vector<uint8_t>& data) {
        decode_binary(data);
        size_t index = 0;
        for (; index < data.size() - 1; ++index) {
            if ((data[index] & 0x80) == 0) {
                break;
            }
        }
        return {data.begin() + index + 2, data.end()};
    }

    // encode_binary serializes the Decimal value into a byte vector
    [[nodiscard]] std::vector<uint8_t> encode_binary() const {
        std::vector<uint8_t> data;
        uint64_t value = fp;
        do {
            uint8_t byte = value & 0x7F;
            value >>= 7;
            if (value != 0) {
                byte |= 0x80;
            }
            data.push_back(byte);
        } while (value != 0);

        // Add nPlaces as an additional byte at the end
        data.push_back(static_cast<uint8_t>(nPlaces));

        return data;
    }

   private:
    static uint64_t mul(uint64_t fp, uint64_t f0) {
        uint64_t fp_a = fp / scale;
        uint64_t fp_b = fp % scale;

        uint64_t fp0_a = f0 / scale;
        uint64_t fp0_b = f0 % scale;

        uint64_t result = 0;

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

    static uint64_t newI(uint64_t i, uint32_t n) {
        if (n > nPlaces) {
            i /= precomputed_pow_10(n - nPlaces);
            n = nPlaces;
        }
        i *= precomputed_pow_10(nPlaces - n);
        return i;
    }

    [[nodiscard]] std::string to_str() const {
        if (fp == 0) {
            static const std::string zero_str = "0." + zeros();
            return zero_str;
        }

        std::array<char, 24> buf;
        int i = sizeof(buf) - 1;
        int idec = i - nPlaces;
        uint64_t val = fp;

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
        return {std::next(buf.begin(), i), buf.end()};
    }

    static int max(int a, int b) { return (a > b) ? a : b; }
};

template <int nPlaces>
const std::runtime_error Decimal<nPlaces>::errDivByZero("division by zero");
template <int nPlaces>
const std::overflow_error Decimal<nPlaces>::errTooLarge("number is too large");
template <int nPlaces>
const std::overflow_error Decimal<nPlaces>::errOverflow("decimal overflow");
template <int nPlaces>
const std::invalid_argument Decimal<nPlaces>::errInvalidInput("invalid input");

}  // namespace udecimal
