// #include <iostream>
// #include <string>
#include "ulimitedint.h"

int abs(int n)
{
    return (n < 0) ? -n : n;
}

class LargeInteger
{
private:
    int *digits;
    int size;
    int base;
    bool isNegative;
    bool exactDivision;
public:
    // Add a default constructor to LargeInteger
    LargeInteger() : digits(nullptr), size(0), base(10), isNegative(false) {}

    LargeInteger(const std::string &str, int base = 10) : base(base), isNegative(false)
    {
        size = str.length();
        if (str[0] == '-')
        {
            isNegative = true;
            size--;
        }

        digits = new int[size];
        for (int i = 0; i < size; i++)
        {
            if (str[i + (isNegative ? 1 : 0)] >= '0' && str[i + (isNegative ? 1 : 0)] <= '9')
            {
                digits[i] = str[size - 1 - i + (isNegative ? 1 : 0)] - '0';
            }
            else
            {
                // Handle non-numeric characters (e.g., '-' or other invalid characters)
                delete[] digits;
                // throw std::invalid_argument("Invalid character in the input string.");
            }
        }
    }

    LargeInteger(const LargeInteger &other) : base(other.base), isNegative(other.isNegative)
    {
        size = other.size;
        digits = new int[size];
        for (int i = 0; i < size; i++)
        {
            digits[i] = other.digits[i];
        }
    }

    bool isZero() const
    {
        return (size == 1 && digits[0] == 0);
    }

    LargeInteger &operator=(const LargeInteger &other)
    {
        if (this != &other)
        {
            delete[] digits;
            size = other.size;
            base = other.base;
            isNegative = other.isNegative;
            digits = new int[size];
            for (int i = 0; i < size; i++)
            {
                digits[i] = other.digits[i];
            }
        }
        return *this;
    }

    // Comparison operator for equality
    bool operator==(const LargeInteger &other) const
    {
        if (size != other.size || isNegative != other.isNegative)
        {
            return false;
        }
        for (int i = 0; i < size; i++)
        {
            if (digits[i] != other.digits[i])
            {
                return false;
            }
        }
        return true;
    }

    bool operator!=(const LargeInteger &other) const
    {
        if (size != other.size || isNegative != other.isNegative)
        {
            return true;
        }
        for (int i = 0; i < size; i++)
        {
            if (digits[i] != other.digits[i])
            {
                return true;
            }
        }
        return false;
    }

    ~LargeInteger()
    {
        delete[] digits;
    }

    LargeInteger operator+(const LargeInteger &other) const
{
    // Check for negative numbers
    if (isNegative && !other.isNegative)
    {
        LargeInteger temp = other;
        temp.isNegative = true;
        return *this - temp; // Perform subtraction when this number is greater
    }
    else if (!isNegative && other.isNegative)
    {
        LargeInteger temp = other;
        temp.isNegative = false;
        return *this - temp; // Perform subtraction when other number is greater
    }

    // Calculate the absolute values of both numbers
    LargeInteger absThis(*this);
    absThis.isNegative = false;
    LargeInteger absOther(other);
    absOther.isNegative = false;

    // Check which number has a greater absolute value
    if (absThis < absOther)
    {
        LargeInteger result = other;
        result += *this; // Perform addition when other number is greater
        return result;
    }
    else
    {
        LargeInteger result = *this;
        result += other; // Perform addition when this number is greater or they are equal
        return result;
    }
}


    LargeInteger operator-(const LargeInteger &other) const
    {
        if (isNegative && !other.isNegative)
        {
            LargeInteger temp = other;
            temp.isNegative = true;
            return *this + temp;
        }
        else if (!isNegative && other.isNegative)
        {
            LargeInteger temp = other;
            temp.isNegative = false;
            return *this + temp;
        }

        // Handle subtraction of negative numbers
        if (*this < other)
        {
            LargeInteger result = other - *this;
            result.isNegative = !result.isNegative;
            return result;
        }

        LargeInteger result(*this);
        result -= other;
        return result;
    }

    // Helper function to remove leading zeros from a LargeInteger
    void removeLeadingZeros()
    {
        while (size > 1 && digits[size - 1] == 0)
        {
            size--;
        }
    }

    LargeInteger operator*(const LargeInteger &other) const
    {
        // Check if either operand is 0 and return 0
        if (isZero() || other.isZero())
        {
            return LargeInteger("0");
        }

        // Initialize a result LargeInteger with zeros
        LargeInteger result("0");

        // Set the sign of the result based on the signs of the operands
        result.isNegative = (isNegative != other.isNegative);

        // Create an array to store intermediate products
        LargeInteger *products = new LargeInteger[other.size];

        // Perform long multiplication
        for (int i = 0; i < other.size; i++)
        {
            products[i].size = size + i + 1;
            products[i].digits = new int[products[i].size]();
            int carry = 0;

            for (int j = 0; j < size; j++)
            {
                int product = digits[j] * other.digits[i] + carry;
                products[i].digits[j + i] = product % base;
                carry = product / base;
            }

            if (carry > 0)
            {
                products[i].digits[size + i] = carry;
            }
        }

        // Add all intermediate products to get the final result
        for (int i = 0; i < other.size; i++)
        {
            result += products[i];
        }

        // Clean up intermediate products
        delete[] products;

        // Remove leading zeros from the result
        result.removeLeadingZeros();

        return result;
    }

    LargeInteger operator<<(int shift) const
    {
        LargeInteger result(*this);
        result.ShiftLeft(shift);
        return result;
    }

    // Helper method to shift the digits left by a specified number of positions
    void ShiftLeft(int shift)
    {
        if (shift <= 0)
        {
            return;
        }
        int newSize = size + shift;
        int *newDigits = new int[newSize];
        for (int i = 0; i < shift; i++)
        {
            newDigits[i] = 0;
        }
        for (int i = shift; i < newSize; i++)
        {
            newDigits[i] = digits[i - shift];
        }
        delete[] digits;
        digits = newDigits;
        size = newSize;
    }

    LargeInteger operator/(const LargeInteger &other) const {
    // Check if dividend is zero and return 0
    if (isZero()) {
        return LargeInteger("0");
    }

    // Initialize the result LargeInteger and set the sign
    LargeInteger result("0");
    result.isNegative = (isNegative != other.isNegative);

    LargeInteger dividend(*this);
    LargeInteger divisor(other);

    // Ensure both dividend and divisor are positive
    dividend.isNegative = false;
    divisor.isNegative = false;

    // Initialize the exactDivision flag to true
    bool exactDivision = true;

    while (dividend >= divisor) {
        LargeInteger tempDivisor(divisor);
        LargeInteger quotient("1");

        while (dividend >= tempDivisor) {
            dividend -= tempDivisor;
            result += quotient;
            tempDivisor <<= 1;
            quotient <<= 1;
        }
    }

    // Check if there's a remainder
    if (dividend != LargeInteger("0")) {
        exactDivision = false;
    }

    // If exact division didn't occur, adjust the result
    if (!exactDivision && result.isNegative) {
        result += LargeInteger("1");
    }

    return result;
}

    bool operator>=(const LargeInteger &other) const
    {
        if (isNegative && !other.isNegative)
        {
            return false; // Negative is always less than non-negative
        }
        else if (!isNegative && other.isNegative)
        {
            return true; // Non-negative is always greater than negative
        }

        // Compare the absolute values of LargeIntegers
        if (size > other.size)
            return true;
        if (size < other.size)
            return false;

        for (int i = size - 1; i >= 0; i--)
        {
            if (digits[i] > other.digits[i])
                return true;
            if (digits[i] < other.digits[i])
                return false;
        }

        return true; // Equal
    }

    void operator+=(const LargeInteger &other)
    {
        int carry = 0;
        for (int i = 0; i < std::max(size, other.size) || carry; i++)
        {
            if (i == size)
            {
                int *newDigits = new int[i + 1];
                for (int j = 0; j < size; j++)
                {
                    newDigits[j] = digits[j];
                }
                delete[] digits;
                digits = newDigits;
                size++;
                digits[i] = 0;
            }
            digits[i] += carry + (i < other.size ? other.digits[i] : 0);
            carry = digits[i] >= base;
            if (carry)
                digits[i] -= base;
        }

        // Remove leading zeros
        removeLeadingZeros();
    }

    void operator-=(const LargeInteger &other)
    {
        int borrow = 0;
        for (int i = 0; i < size; i++)
        {
            digits[i] -= borrow + (i < other.size ? other.digits[i] : 0);
            borrow = digits[i] < 0;
            if (borrow)
                digits[i] += base;
        }

        // Remove leading zeros
        removeLeadingZeros();
    }

    void operator<<=(int shift)
    {
        if (shift == 0)
            return;
        int newSize = size + shift;
        int *newDigits = new int[newSize];
        for (int i = 0; i < shift; i++)
        {
            newDigits[i] = 0;
        }
        for (int i = shift; i < newSize; i++)
        {
            newDigits[i] = digits[i - shift];
        }
        delete[] digits;
        digits = newDigits;
        size = newSize;
    }

    bool operator<(const LargeInteger &other) const
    {
        if (isNegative && !other.isNegative)
        {
            return true; // Negative is always less than non-negative
        }
        else if (!isNegative && other.isNegative)
        {
            return false; // Non-negative is always greater than negative
        }

        // Compare the absolute values of LargeIntegers
        if (size < other.size)
            return true;
        if (size > other.size)
            return false;

        for (int i = size - 1; i >= 0; i--)
        {
            if (digits[i] < other.digits[i])
                return true;
            if (digits[i] > other.digits[i])
                return false;
        }

        return false; // Equal
    }

    // Convert to string for output
    std::string toString() const
    {
        std::string result;
        if (isNegative)
        {
            result += '-';
        }
        for (int i = size - 1; i >= 0; i--)
        {
            result += std::to_string(digits[i]);
        }
        return result;
    }
    // Overload the % operator for modulo operation
    LargeInteger operator%(const LargeInteger &other) const
    {
        // if (other == LargeInteger("0")) {
        //     throw std::runtime_error("Modulo by zero is not allowed.");
        // }

        // Calculate the floor division
        LargeInteger floorDiv = *this / other;

        // Calculate the product of other and floorDiv
        LargeInteger product = other * floorDiv;

        // Calculate the result as a - product
        LargeInteger result = *this - product;

        return result;
    }

    LargeInteger findHCF(const LargeInteger &a, const LargeInteger &b)
    {
        // Ensure both numbers are positive for HCF calculation
        LargeInteger num1 = a;
        num1.isNegative = false;
        LargeInteger num2 = b;
        num2.isNegative = false;

        // Use the Euclidean algorithm to find the HCF
        while (num2 != LargeInteger("0"))
        {
            LargeInteger temp = num2;
            num2 = num1 % num2;
            num1 = temp;
        }
        return num1;
    }

    // Friend function to print LargeInteger
    friend std::ostream &operator<<(std::ostream &os, const LargeInteger &num)
    {
        os << num.toString();
        return os;
    }
};

UnlimitedInt::UnlimitedInt(string s)
{
    if (s[0] == '-')
    {
        sign = -1;
    }
    else
        sign = 1;

    if (sign == -1)
    {
        s = s.substr(1);
    }

    size = s.length();

    unlimited_int = new int[size];
    for (int i = 0; i < size; i++)
    {
        if (std::isdigit(s[i]))
        {
            int digit = s[i] - '0';
            unlimited_int[i] = digit;
        }
    }
}

UnlimitedInt::UnlimitedInt(int *arr, int cap, int sgn, int sz)
{
    unlimited_int = arr;
    // capacity = cap;
    sign = sgn;
    size = sz;
}

int UnlimitedInt::get_size()
{
    return size;
}

int *UnlimitedInt::get_array()
{
    return unlimited_int;
}

int UnlimitedInt::get_sign()
{
    return sign;
}

// Actually only this is important
string UnlimitedInt::to_string()
{
    string s;
    if (sign == -1)
    {
        s.resize(size + 1);
        s[0] = '-';
        for (int i = 1; i <= size; i++)
        {
            s[i] = '0' + unlimited_int[i - 1];
        }
    }
    else
    {
        s.resize(size);
        for (int i = 0; i < size; i++)
        {
            s[i] = '0' + unlimited_int[i];
        }
    }
    return s;
}

UnlimitedInt::~UnlimitedInt()
{
    delete[] unlimited_int;
}

UnlimitedInt *UnlimitedInt::add(UnlimitedInt *i1, UnlimitedInt *i2)
{
    LargeInteger *num1 = new LargeInteger(i1->to_string());
    LargeInteger *num2 = new LargeInteger(i2->to_string());
    LargeInteger *sum = new LargeInteger(*num1 + *num2);
    UnlimitedInt *ans = new UnlimitedInt(sum->toString());
    delete num1;
    delete num2;
    delete sum;
    return ans;
}

UnlimitedInt *UnlimitedInt::sub(UnlimitedInt *i1, UnlimitedInt *i2)
{
    LargeInteger *num1 = new LargeInteger(i1->to_string());
    LargeInteger *num2 = new LargeInteger(i2->to_string());
    LargeInteger *diff = new LargeInteger(*num1 - *num2);
    UnlimitedInt *ans = new UnlimitedInt(diff->toString());
    delete num1;
    delete num2;
    delete diff;
    return ans;
}

UnlimitedInt *UnlimitedInt::mul(UnlimitedInt *i1, UnlimitedInt *i2)
{
    LargeInteger *num1 = new LargeInteger(i1->to_string());
    LargeInteger *num2 = new LargeInteger(i2->to_string());
    LargeInteger *prod = new LargeInteger((*num1) * (*num2));
    UnlimitedInt *ans = new UnlimitedInt(prod->toString());
    delete num1;
    delete num2;
    delete prod;
    return ans;
}

UnlimitedInt *UnlimitedInt::div(UnlimitedInt *i1, UnlimitedInt *i2)
{
    LargeInteger *num1 = new LargeInteger(i1->to_string());
    LargeInteger *num2 = new LargeInteger(i2->to_string());
    LargeInteger *quot = new LargeInteger((*num1) / (*num2));
    UnlimitedInt *ans = new UnlimitedInt(quot->toString());
    delete num1;
    delete num2;
    delete quot;
    return ans;
}

UnlimitedInt *UnlimitedInt::mod(UnlimitedInt *i1, UnlimitedInt *i2)
{
    LargeInteger *num1 = new LargeInteger(i1->to_string());
    LargeInteger *num2 = new LargeInteger(i2->to_string());
    LargeInteger *rem = new LargeInteger((*num1) % (*num2));
    UnlimitedInt *ans = new UnlimitedInt(rem->toString());
    delete num1;
    delete num2;
    delete rem;
    return ans;
}

// int main()
// {
//     UnlimitedInt *x = new UnlimitedInt("-1");
//     UnlimitedInt *y = new UnlimitedInt("4");
//     UnlimitedInt *z = UnlimitedInt::mul(x, y);
//     std::cout << z->to_string();
// }
