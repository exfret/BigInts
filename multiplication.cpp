//
//  multiplication.cpp
//  BigInts
//
//  Created by Kyle Hess on 3/15/18.
//  Copyright © 2018 Kyle Hess. All rights reserved.
//

#include "multiplication.hpp"


Bigint karatsuba(Bigint a, Bigint b) {
    return karatsuba(a, b, KARATSUBADEPTH);
}

Bigint karatsuba(Bigint a, Bigint b, unsigned depth) {
    if (depth == 0) {
        return a * b;
    } else if (a.getLength() < KARATSUBATHRESHOLD || b.getLength() < KARATSUBATHRESHOLD) {
        return a * b;
    }
    unsigned long m = std::max(a.getLength(), b.getLength());
    
    Bigint x_1 = a << (m / 2);
    Bigint x_0 = a - (x_1 >> m / 2);
    Bigint y_1 = b << (m / 2);
    Bigint y_0 = b - (y_1 >> m / 2);
    
    Bigint z_0 = karatsuba(x_0, y_0, depth - 1);
    Bigint z_2 = karatsuba(x_1, y_1, depth - 1);
    Bigint z_1 = karatsuba(x_1 + x_0, y_1 + y_0, depth - 1) - z_0 - z_2;
    
    return z_0 + (z_1 >> m/2) + (z_2 >> (2 * m/2));
}


























