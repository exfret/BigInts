//
//  Bigint.hpp
//  BigInts
//
//  Created by Kyle Hess on 1/13/17.
//  Copyright © 2017 Kyle Hess. All rights reserved.
//

#ifndef Bigint_hpp
#define Bigint_hpp

#include <iostream>
#include <vector>
#include <string>
#include <bitset>
#include <iomanip>
#include "globals.hpp"

class Bigint {
public:
    // Constructors/Destructors
    Bigint(intmax_t = 0);
    // TODO: Change to repeated squaring method
    Bigint(intmax_t base, unsigned exponent);
    
    // Accessors
    fint                  getDigit(unsigned long int n) const;
    bool                  getBit(unsigned long int n) const;
    unsigned long int     getLength() const;
    // unsigned long int     getBitLength() const;
    bool                  getSign() const;
    std::string           getOutputType() const;
    
    // Mutators
    void                  pushDigit(fint p);
    void                  flipSign();
    
    // Operators
    
    // Returns integer rep. of BigInt truncated after 62nd binary place,
    // returns zero if BigInt has undefined valued (e.g. digits = {0} and
    // sign = false). Not yet converted to fast-style.
    /* explicit operator         short int() const;
       explicit operator         int() const;
       explicit operator         long int() const;
       explicit operator         long long int() const;
       explicit operator         unsigned short int() const;
       explicit operator         unsigned int() const;
       explicit operator         unsigned long int() const;
      explicit operator         unsigned long long int() const;
     */
    
    // Comparison operators
    bool operator<(Bigint b) const;
    bool operator==(Bigint b) const;
    bool operator<=(Bigint b) const;
    bool operator>(Bigint b) const;
    bool operator>=(Bigint b) const;
    
    // Arithmetic operators
    Bigint operator+(Bigint b) const;
    Bigint& operator+=(Bigint b);
    Bigint operator-(Bigint b) const;
    Bigint& operator-=(Bigint b);
    Bigint operator-() const;
    // TODO: Implement for negative numbers
    Bigint operator%(Bigint modulus) const;
    Bigint integerMult(fint A) const;
    Bigint operator*(Bigint b) const;
    Bigint& operator*=(Bigint b);
    //Bigint& operator*=(Bigint b);
    //Bigint& operator<<(Bigint b);
    Bigint operator<<(unsigned long n);
    Bigint operator>>(unsigned long n);
    
    
    
private:
    ///////////////////////////////////////////////////////////////////////////
    // m_digits is a vector holding the binary digits,
    // has size up to that of unsigned long int (up to 4,294,967,295 digits)
    // Sign = true when the number is nonnegative and false otherwise
    // We will return a Bigint with m_digits = 0 and m_sign = false when
    // something goes wrong (i.e. - division by zero)
    std::vector<fint>   m_digits;
    bool                m_sign;
    
    // This is the number system the number will be outputed in
    // Possible values: "bin"
    std::string         outputType;
    
    // Private Auxiliary Functions
    void removeLeadingZeroes();
};

// Printing
// TODO: Fix bug with printing leading zeroes
std::ostream& operator<<(std::ostream& s, Bigint b);

// Free functions
Bigint abs(Bigint b);

template <class T> T convertBigint(Bigint b) {
    T a = static_cast<T>(0);
    std::numeric_limits<T> n;
    
    for(int i = 0; i * RADIX_SIZE < sizeof(T) && i < b.getLength(); i++) {
        a |= static_cast<T>(b.getDigit(i)) << (i * RADIX_SIZE);
    }
    if(n.is_signed && !b.getSign()) a = -a;
    return a;
}

#endif /* Bigint_hpp */









































