//
//  globals.hpp
//  BigInts
//
//  Created by Kyle Hess on 1/20/17.
//  Copyright © 2017 Kyle Hess. All rights reserved.
//

#ifndef globals_hpp
#define globals_hpp

#include <stdint.h>
#include <limits>

// Computing 10^10000 on different int types:
// 17 seconds on 8 bit
// 10 seconds on 16 bit
// 5 seconds on 32 bit
// NA seconds on 64 bit (requires special case checking that I don't care to do)

// fint stands for "fast integer"
typedef uint32_t fint;
const size_t RADIX_SIZE = sizeof(fint) * 8;

inline
fint getCarry(fint p, fint q, fint r) {
    
    // We need to calculate these separately else the compiler optimizes incorrectly
    fint pplusq = p + q;
    fint pplusqplusr = p + q + r;
    
    return static_cast<fint>(pplusq < p || pplusqplusr < p);
}

#endif /* globals_hpp */
