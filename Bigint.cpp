//
//  Bigint.cpp
//  BigInts
//
//  Created by Kyle Hess on 1/13/17.
//  Copyright © 2017 Kyle Hess. All rights reserved.
//
//  NOTES
//
//  I adopt the convention of representing an arbitrary Bigint by LOWER CASE
//  (not upper case) b.
//
//  For binary operations on Bigints, I adopt the convention that m = length
//  of Bigint on the left and n = length of Bigint on the right and A is a
//  holder variable for the Bigint on the left and B is a holder variable
//  for the one on the right.
//

#include "Bigint.hpp"

///////////////////////////////////////////////////////////////////////////
// Constructors/Destructors
///////////////////////////////////////////////////////////////////////////

Bigint::Bigint(intmax_t init) : outputType("bin"), m_digits(0)
{
    if (init >= 0) {
        m_sign = true;
    }
    else {
        m_sign = false;
        init = -init;
    }
    
    // I assume the conversion to a smaller integer size here is to cut
    // off the extra digits to the left
    for (; init > 0; init = init >> RADIX_SIZE)
        m_digits.push_back(init);
}

// Create a Bigint equal to base ^ exponent
Bigint::Bigint(intmax_t base, unsigned exponent) {
    if (exponent == 0)
        *this = Bigint(1);
    
    // Create the Bigint by making another bigint and setting it equal to
    // this one later
    Bigint b1(base);
    Bigint b2(exponent);
    Bigint b3(1);
    // Algorithm 4
    std::vector<Bigint> powersoftwo;
    
    // Caluclate furthest bit out so we don't have to calculate more powers
    // of two than needed
    int largestDigit = -1;
    for (int i = 0; i < RADIX_SIZE; ++i) {
        if ((b2.getDigit(b2.getLength() - 1) >> i) % 2 == 1) {
            largestDigit = i;
        }
    }
    
    for (int i = 0; i < (b2.getLength() - 1) * RADIX_SIZE + largestDigit + 1; ++i) {
        powersoftwo.push_back(b1);
        // Make sure we don't calculate this when we don't need to
        if (i != (b2.getLength() - 1) * RADIX_SIZE + largestDigit)
            b1 = b1 * b1;
    }
    b1 = 1;
    for (int i = 0; i < b2.getLength() * RADIX_SIZE; ++i) {
        if ((b2.getDigit(i / RADIX_SIZE) >> (i % RADIX_SIZE)) % 2 == 1) {
            b1 *= powersoftwo[i];
        }
    }
    
    
    *this = b1;
}

///////////////////////////////////////////////////////////////////////////
// Accessors
///////////////////////////////////////////////////////////////////////////

fint Bigint::getDigit(unsigned long int n) const {
    return m_digits.at(n);
}
unsigned long int Bigint::getLength() const {
    return m_digits.size();
}
bool Bigint::getSign() const {
    return m_sign;
}
std::string Bigint::getOutputType() const {
    return outputType;
}

///////////////////////////////////////////////////////////////////////////
// Mutators
///////////////////////////////////////////////////////////////////////////

void Bigint::pushDigit(fint p) {
    m_digits.push_back(p);
}

void Bigint::flipSign() {
    m_sign = !m_sign;
}

///////////////////////////////////////////////////////////////////////////
// Operators
///////////////////////////////////////////////////////////////////////////

// Declare a helper function to convert Bigints to ints
// This helper function can convert Bigint to any int type. If Bigint b is
// too big, then its last (most significant) digits are truncated.
template <class T> T convertBigint(Bigint b);

// Converts BigInt to int
/* Bigint::operator short int() const                {return convertBigint<short int>(*this);}
   Bigint::operator int() const                      {return convertBigint<int>(*this);}
   Bigint::operator long int() const                 {return convertBigint<long int>(*this);}
   Bigint::operator long long int() const            {return convertBigint<long long int>(*this);}
   Bigint::operator unsigned short int() const       {return convertBigint<unsigned short int>(*this);}
   Bigint::operator unsigned int() const             {return convertBigint<unsigned int>(*this);}
   Bigint::operator unsigned long int() const        {return convertBigint<unsigned long int>(*this);}
   Bigint::operator unsigned long long int() const   {return convertBigint<unsigned long long int>(*this);}
*/

bool Bigint::operator<(Bigint b) const {
    // Check signs
    if (this->getSign() < b.getSign())
        return true;
    else if (this->getSign() > b.getSign())
        return false;
    else if (!this->getSign() && !b.getSign()) {
        Bigint c = *this;
        return !(abs(c) < abs(b));
    }
    
    unsigned long int m = this->getLength();
    unsigned long int n = b.getLength();
    
    if(m != n)
        return m < n;
    
    // If we get here, we should have m == n
    for (int i = 0; i < m; i++) {
        if(this->getDigit(m - i - 1) < b.getDigit(n - i - 1))
            return true;
        else if(this->getDigit(m - i - 1) > b.getDigit(n - i - 1))
            return false;
    }
    
    // At this point, if we haven't returned, then we must have b1 == b2
    return false;
}
bool Bigint::operator==(Bigint b) const {
    return !(*this < b || b < *this);
}
bool Bigint::operator<=(Bigint b) const {
    return (*this < b) || (*this == b);
}
bool Bigint::operator>(Bigint b) const {
    return !(*this <= b);
}
bool Bigint::operator>=(Bigint b) const {
    return !(*this < b);
}

Bigint Bigint::operator+(Bigint b) const {
    // Sign logic so that we can assume *this and b are both positive
    if (getSign() && !b.getSign())
        return *this - -b;
    else if (!getSign() && b.getSign())
        return b - -*this;
    else if (!getSign() && !b.getSign())
        return -(-b + -*this);
    
    // We can now assume both *this and b are positive
    
    Bigint result(0);
    fint carry = 0;
    unsigned long int m = this->getLength();
    unsigned long int n = b.getLength();
    fint A;
    fint B;
    
    for (unsigned long int i = 0; i < std::max(m, n); ++i) {
        if (i < m)
            A = this->getDigit(i);
        else
            A = 0;
        
        if (i < n)
            B = b.getDigit(i);
        else
            B = 0;
        
        result.pushDigit(A + B + carry);
        carry = getCarry(A, B, carry);
    }
    
    if (carry > 0) {
        result.m_digits.push_back(carry);
    }
    
    return result;
}

Bigint& Bigint::operator+=(Bigint b) {
    *this = *this + b;
    return *this;
}

// This needs testing and seems to not be working when carries are needed
Bigint Bigint::operator-(Bigint b) const {
    if (getSign() && !b.getSign())
        return *this + -b;
    else if (!getSign() && b.getSign())
        return -(b + -*this);
    else if (!getSign() && !b.getSign())
        return -(-*this - -b);
    // We can now assume both *this and b are positive
    
    if (*this < b)
        return -(b - *this);
    // Now we can assume *this > b
    
    /*Bigint result(0);
    bool carry = false;
    unsigned long int m = this->getLength();
    unsigned long int n = b.getLength();
    bool A;
    bool B;
    
    for(unsigned long int i = 0; i < m; i++) {
        A = this->getDigit(i);
        if (i < n)
            B = b.getDigit(i);
        else
            B = false;
        
        //result.pushDigit(subtract(A,B,carry));
        carry = (A < (B + carry));
    }*/
    
    
    Bigint result(0);
    fint carry = 0;
    unsigned long int m = this->getLength();
    unsigned long int n = b.getLength();
    fint A;
    fint B;
    
    for (unsigned long int i = 0; i < std::max(m, n); ++i) {
        if (i < m)
            A = this->getDigit(i);
        else
            A = 0;
        
        if (i < n)
            B = b.getDigit(i);
        else
            B = 0;
        
        result.pushDigit(A - B + carry);
        if (A + carry >= B) {
            carry = 0;
        }
        else {
            carry = -1;
        }
        
        carry = getCarry(A, B, carry);
    }
    
    if (carry > 0) {
        result.m_digits.push_back(carry);
    }
    
    // This shouldn't work because removeLeadingZeroes is a private function
    // ... But it does work ...
    result.removeLeadingZeroes();
    
    return result;
}

Bigint& Bigint::operator-=(Bigint b) {
    *this = *this - b;
    return *this;
}

// Creating a whole new Bigint to return the given Bigint negated isn't
// very fast, but it'll be clear and can be optimized later
Bigint Bigint::operator-() const {
    Bigint c = *this;
    c.flipSign();
    return c;
}


// NOTE: IMPLEMENT FOR NEGATIVE NUMBERS
// a%-b = a&b
// -a%b = b-(a%b)
Bigint Bigint::operator%(Bigint modulus) const {
    if (modulus < 0)
        modulus.flipSign();
    
    Bigint residue(0);
    unsigned long int m = this->getLength();
    
    for(int i = 0; i < getLength(); i++) {
        // Multiply by RADIX
        residue.pushDigit(0);
        residue = residue + this->getDigit(m - i);
        if(residue > modulus)
            residue = residue - modulus;
    }
    
    return residue;
}

Bigint Bigint::integerMult(fint A) const {
    Bigint b(0);
    for (int i = 0; i < RADIX_SIZE; ++i) {
        if ((A >> (RADIX_SIZE - i - 1)) % 2 == 1) {
            b = b + b + *this;
        }
        else {
            b = b + b;
        }
    }
    return b;
}

// Not pushing any digit except ones place when this is length 1
Bigint Bigint::operator*(Bigint b) const {
    if (this->getSign() xor b.getSign())
        return -((-*this) * b);
    
    Bigint result(0);
    unsigned long m = this->getLength();
    for(unsigned long int i = 0; i < m; i++) {
        
        result = result + b.integerMult(this->getDigit(i));
        b = b >> 1;
    }
    
    return result;
}

Bigint& Bigint::operator*=(Bigint b) {
    *this = *this * b;
    return *this;
}

Bigint Bigint::operator>>(unsigned long n) {
    Bigint b(0);
    for (int i = 0; i < n; ++i) {
        b.pushDigit(0);
    }
    for (int i = 0; i < this->getLength(); ++i) {
        b.pushDigit(getDigit(i));
    }
    return b;
}

Bigint Bigint::operator<<(unsigned long n) {
    Bigint b = 0;
    for (unsigned long i = n; i < this->getLength(); ++i) {
        b.pushDigit(getDigit(i));
    }
    return b;
}

std::ostream& operator<<(std::ostream& s, Bigint b) {
    if(b.getOutputType() == "bin") {
        
        // First make sure b's length is not zero
        if (b == 0) {
            s << 0;
            return s;
        }
        
        // Add negative sign if number is negative
        if (!b.getSign()) {
            s << "-";
        }
        
        // Write digits to s
        s << std::bitset<RADIX_SIZE>(b.getDigit(b.getLength() - 1));
        for(unsigned long int i = 2; i <= b.getLength(); ++i) {
            s << std::setfill('0') << std::setw(RADIX_SIZE) << std::bitset<RADIX_SIZE>(b.getDigit(b.getLength() - i));
        }
    }
    else if (b.getOutputType() == "dec") {
        s << "This output type not yet supported";
    }
    else if (b.getOutputType() == "hex") {
        s << "This output type not yet supported";
    }
    return s;
}

///////////////////////////////////////////////////////////////////////////
// Private Auxiliary functions
///////////////////////////////////////////////////////////////////////////

void Bigint::removeLeadingZeroes() {
    for (unsigned long i = getLength() - 1; i != 0; --i) {
        if (getDigit(i) != 0) {
            break;
        }
        m_digits.pop_back();
    }
}

///////////////////////////////////////////////////////////////////////////
// Free functions
///////////////////////////////////////////////////////////////////////////

Bigint abs(Bigint b) {
    if (!b.getSign())
        b.flipSign();
    return b;
}






































