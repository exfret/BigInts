//
//  multiplication.hpp
//  BigInts
//
//  Created by Kyle Hess on 3/15/18.
//  Copyright © 2018 Kyle Hess. All rights reserved.
//

#ifndef multiplication_hpp
#define multiplication_hpp

#include "Bigint.hpp"

// Constants
const unsigned KARATSUBATHRESHOLD = 10;
const unsigned KARATSUBADEPTH = 0;


// Defines more advanced multiplication algorithm on BigInts

// Currently BROKEN
Bigint karatsuba(Bigint a, Bigint b);
Bigint karatsuba(Bigint a, Bigint b, unsigned depth);

#endif /* multiplication_hpp */


























