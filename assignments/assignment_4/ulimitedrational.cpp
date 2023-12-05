/* Do NOT add/remove any includes statements from this header file */
/* unless EXPLICTLY clarified on Piazza. */
#include "ulimitedrational.h"
// #include "ulimitedint.cpp"
// #include <iostream>
// #include <string>

int abs_(int n)
{
    return (n < 0) ? -n : n;
}

class LargeInteger_
{
private:
    int *digits;
    int size;
    int base;
    bool isNegative;
    bool exactDivision;

public:
    // Add a default constructor to LargeInteger_
    LargeInteger_() : digits(nullptr), size(0), base(10), isNegative(false) {}

    LargeInteger_(const std::string &str, int base = 10) : base(base), isNegative(false)
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

    LargeInteger_(const LargeInteger_ &other) : base(other.base), isNegative(other.isNegative)
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

    LargeInteger_ &operator=(const LargeInteger_ &other)
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
    bool operator==(const LargeInteger_ &other) const
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

    bool operator!=(const LargeInteger_ &other) const
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

    ~LargeInteger_()
    {
        delete[] digits;
    }

    LargeInteger_ operator+(const LargeInteger_ &other) const
    {
        // Check for negative numbers
        if (isNegative && !other.isNegative)
        {
            LargeInteger_ temp = other;
            temp.isNegative = true;
            return *this - temp; // Perform subtraction when this number is greater
        }
        else if (!isNegative && other.isNegative)
        {
            LargeInteger_ temp = other;
            temp.isNegative = false;
            return *this - temp; // Perform subtraction when other number is greater
        }

        // Calculate the absolute values of both numbers
        LargeInteger_ absThis(*this);
        absThis.isNegative = false;
        LargeInteger_ absOther(other);
        absOther.isNegative = false;

        // Check which number has a greater absolute value
        if (absThis < absOther)
        {
            LargeInteger_ result = other;
            result += *this; // Perform addition when other number is greater
            return result;
        }
        else
        {
            LargeInteger_ result = *this;
            result += other; // Perform addition when this number is greater or they are equal
            return result;
        }
    }

    LargeInteger_ operator-(const LargeInteger_ &other) const
    {
        if (isNegative && !other.isNegative)
        {
            LargeInteger_ temp = other;
            temp.isNegative = true;
            return *this + temp;
        }
        else if (!isNegative && other.isNegative)
        {
            LargeInteger_ temp = other;
            temp.isNegative = false;
            return *this + temp;
        }

        // Handle subtraction of negative numbers
        if (*this < other)
        {
            LargeInteger_ result = other - *this;
            result.isNegative = !result.isNegative;
            return result;
        }

        LargeInteger_ result(*this);
        result -= other;
        return result;
    }

    // Helper function to remove leading zeros from a LargeInteger_
    void removeLeadingZeros()
    {
        while (size > 1 && digits[size - 1] == 0)
        {
            size--;
        }
    }

    LargeInteger_ operator*(const LargeInteger_ &other) const
    {
        // Check if either operand is 0 and return 0
        if (isZero() || other.isZero())
        {
            return LargeInteger_("0");
        }

        // Initialize a result LargeInteger_ with zeros
        LargeInteger_ result("0");

        // Set the sign of the result based on the signs of the operands
        result.isNegative = (isNegative != other.isNegative);

        // Create an array to store intermediate products
        LargeInteger_ *products = new LargeInteger_[other.size];

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

    LargeInteger_ operator<<(int shift) const
    {
        LargeInteger_ result(*this);
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

    LargeInteger_ operator/(const LargeInteger_ &other) const
    {
        // Check if dividend is zero and return 0
        if (isZero())
        {
            return LargeInteger_("0");
        }

        // Initialize the result LargeInteger_ and set the sign
        LargeInteger_ result("0");
        result.isNegative = (isNegative != other.isNegative);

        LargeInteger_ dividend(*this);
        LargeInteger_ divisor(other);

        // Ensure both dividend and divisor are positive
        dividend.isNegative = false;
        divisor.isNegative = false;

        // Initialize the exactDivision flag to true
        bool exactDivision = true;

        while (dividend >= divisor)
        {
            LargeInteger_ tempDivisor(divisor);
            LargeInteger_ quotient("1");

            while (dividend >= tempDivisor)
            {
                dividend -= tempDivisor;
                result += quotient;
                tempDivisor <<= 1;
                quotient <<= 1;
            }
        }

        // Check if there's a remainder
        if (dividend != LargeInteger_("0"))
        {
            exactDivision = false;
        }

        // If exact division didn't occur, adjust the result
        if (!exactDivision && result.isNegative)
        {
            result += LargeInteger_("1");
        }

        return result;
    }

    bool operator>=(const LargeInteger_ &other) const
    {
        if (isNegative && !other.isNegative)
        {
            return false; // Negative is always less than non-negative
        }
        else if (!isNegative && other.isNegative)
        {
            return true; // Non-negative is always greater than negative
        }

        // Compare the absolute values of LargeInteger_s
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

    void operator+=(const LargeInteger_ &other)
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

    void operator-=(const LargeInteger_ &other)
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

    bool operator<(const LargeInteger_ &other) const
    {
        if (isNegative && !other.isNegative)
        {
            return true; // Negative is always less than non-negative
        }
        else if (!isNegative && other.isNegative)
        {
            return false; // Non-negative is always greater than negative
        }

        // Compare the absolute values of LargeInteger_s
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
    LargeInteger_ operator%(const LargeInteger_ &other) const
    {
        // if (other == LargeInteger_("0")) {
        //     throw std::runtime_error("Modulo by zero is not allowed.");
        // }

        // Calculate the floor division
        LargeInteger_ floorDiv = *this / other;

        // Calculate the product of other and floorDiv
        LargeInteger_ product = other * floorDiv;

        // Calculate the result as a - product
        LargeInteger_ result = *this - product;

        return result;
    }

    LargeInteger_ operator^(const LargeInteger_ &other) const
    {
        // Ensure both numbers are positive for HCF calculation
        LargeInteger_ num1 = *this;
        num1.isNegative = false;
        LargeInteger_ num2 = other;
        num2.isNegative = false;

        // Use the Euclidean algorithm to find the HCF
        while (num2 != LargeInteger_("0"))
        {
            LargeInteger_ temp = num2;
            num2 = num1 % num2;
            num1 = temp;
        }

        return num1;
    }

    // Friend function to print LargeInteger_
    friend std::ostream &operator<<(std::ostream &os, const LargeInteger_ &num)
    {
        os << num.toString();
        return os;
    }
};

// Auxillary fuction to find hcf of two UnlimitedInt numbers
UnlimitedInt *HCF(UnlimitedInt *i1, UnlimitedInt *i2)
{
    LargeInteger_ *num1 = new LargeInteger_(i1->to_string());
    LargeInteger_ *num2 = new LargeInteger_(i2->to_string());
    LargeInteger_ *large_integer_hcf = new LargeInteger_(*num1 ^ *num2);
    UnlimitedInt *hcf = new UnlimitedInt(large_integer_hcf->toString());
    delete large_integer_hcf;
    delete num1;
    delete num2;
    return hcf;
}

UnlimitedRational::UnlimitedRational(UnlimitedInt *num, UnlimitedInt *den)
{
    if (den->to_string() == "0")
    {
        p = new UnlimitedInt("0");
        q = new UnlimitedInt("0");
    }
    else if (num->to_string() == "0" && den->to_string() != "0")
    {
        p = new UnlimitedInt("0");
        q = new UnlimitedInt("1");
    }
    else
    {
        UnlimitedInt *hcf = HCF(num, den);
        UnlimitedInt *x = UnlimitedInt::div(num, hcf);
        UnlimitedInt *y = UnlimitedInt::div(den, hcf);
        int x_sign = x->get_sign();
        int y_sign = y->get_sign();
        if (y_sign == -1)
        {
            y_sign = 1;
            x_sign = -x_sign;
        }
        p = new UnlimitedInt(x->get_array(), 0, x_sign, x->get_size());
        q = new UnlimitedInt(y->get_array(), 0, y_sign, y->get_size());
    }
}

UnlimitedInt *UnlimitedRational::get_p()
{
    return p;
}

UnlimitedInt *UnlimitedRational::get_q()
{
    return q;
}

string UnlimitedRational::get_p_str()
{
    return p->to_string();
}

string UnlimitedRational::get_q_str()
{
    return q->to_string();
}

string UnlimitedRational::get_frac_str()
{
    return this->get_p_str() + '/' + this->get_q_str();
}

UnlimitedRational *UnlimitedRational::add(UnlimitedRational *i1, UnlimitedRational *i2)
{
    UnlimitedInt *num1 = i1->get_p();
    UnlimitedInt *num2 = i2->get_p();
    UnlimitedInt *den1 = i1->get_q();
    UnlimitedInt *den2 = i2->get_q();

    if (den1->to_string() == "0" || den2->to_string() == "0")
    {
        UnlimitedInt *trivial = new UnlimitedInt("0");
        UnlimitedRational *strange = new UnlimitedRational(trivial, trivial);
        delete trivial;
        return strange;
    }

    UnlimitedInt *initial_den = UnlimitedInt::mul(den1, den2);
    UnlimitedInt *x = UnlimitedInt::mul(num1, den2);
    UnlimitedInt *y = UnlimitedInt::mul(num2, den1);
    UnlimitedInt *initial_num = UnlimitedInt::add(x, y);

    UnlimitedRational *sum = new UnlimitedRational(initial_num, initial_den);
    delete x;
    delete y;
    delete initial_den;
    delete initial_num;
    return sum;
}

UnlimitedRational *UnlimitedRational::sub(UnlimitedRational *i1, UnlimitedRational *i2)
{
    UnlimitedInt *num1 = i1->get_p();
    UnlimitedInt *num2 = i2->get_p();
    UnlimitedInt *den1 = i1->get_q();
    UnlimitedInt *den2 = i2->get_q();

    if (den1->to_string() == "0" || den2->to_string() == "0")
    {
        UnlimitedInt *trivial = new UnlimitedInt("0");
        UnlimitedRational *strange = new UnlimitedRational(trivial, trivial);
        delete trivial;
        return strange;
    }
    UnlimitedInt* x = UnlimitedInt::mul(num1, den2);
    UnlimitedInt* y = UnlimitedInt::mul(num2, den1);
    UnlimitedInt *initial_den = UnlimitedInt::mul(den1, den2);
    UnlimitedInt *initial_num = UnlimitedInt::sub(x,y);

    UnlimitedRational *diff = new UnlimitedRational(initial_num, initial_den);
    delete x;
    delete y;
    delete initial_den;
    delete initial_num;
    return diff;
}

UnlimitedRational *UnlimitedRational::mul(UnlimitedRational *i1, UnlimitedRational *i2)
{
    UnlimitedInt *num1 = i1->get_p();
    UnlimitedInt *num2 = i2->get_p();
    UnlimitedInt *den1 = i1->get_q();
    UnlimitedInt *den2 = i2->get_q();

    if (den1->to_string() == "0" || den2->to_string() == "0")
    {
        UnlimitedInt *trivial = new UnlimitedInt("0");
        UnlimitedRational *strange = new UnlimitedRational(trivial, trivial);
        delete trivial;
        return strange;
    }

    UnlimitedInt *initial_den = UnlimitedInt::mul(den1, den2);
    UnlimitedInt *initial_num = UnlimitedInt::mul(num1, num2);

    UnlimitedRational *prod = new UnlimitedRational(initial_num, initial_den);
    delete initial_den;
    delete initial_num;
    return prod;
}

UnlimitedRational *UnlimitedRational::div(UnlimitedRational *i1, UnlimitedRational *i2)
{
    UnlimitedInt *num1 = i1->get_p();
    UnlimitedInt *num2 = i2->get_p();
    UnlimitedInt *den1 = i1->get_q();
    UnlimitedInt *den2 = i2->get_q();

    if (den1->to_string() == "0" || den2->to_string() == "0" || num2->to_string() == "0")
    {
        UnlimitedInt *trivial = new UnlimitedInt("0");
        UnlimitedRational *strange = new UnlimitedRational(trivial, trivial);
        delete trivial;
        return strange;
    }

    UnlimitedInt *initial_den = UnlimitedInt::mul(den1, num2);
    UnlimitedInt *initial_num = UnlimitedInt::mul(num1, den2);

    UnlimitedRational *prod = new UnlimitedRational(initial_num, initial_den);
    delete initial_den;
    delete initial_num;
    return prod;
}

UnlimitedRational::~UnlimitedRational()
{
    delete p;
    delete q;
}
// int main()
// {
//     UnlimitedInt *num1 = new UnlimitedInt("5");
//     UnlimitedInt *den1 = new UnlimitedInt("2");
//     UnlimitedRational *x = new UnlimitedRational(num1, den1);

//     UnlimitedInt *num2 = new UnlimitedInt("-1");
//     UnlimitedInt *den2 = new UnlimitedInt("2");
//     UnlimitedRational *y = new UnlimitedRational(num2, den2);

//     UnlimitedRational *ans = UnlimitedRational::add(x, y);

//     // std::cout<<x->get_frac_str();
//     std::cout << ans->get_frac_str();
// }
