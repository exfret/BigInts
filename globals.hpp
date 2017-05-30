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

// fint stands for "fast integer"
typedef uint_fast8_t fint;
const size_t RADIX_SIZE = sizeof(fint);

inline
fint getCarry(fint p, fint q, fint r) {
    return static_cast<fint>(p + q < p || p + q + r < p);
}

#endif /* globals_hpp */
