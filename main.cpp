//
//  main.cpp
//  BigInts
//
//  Created by Kyle Hess on 1/13/17.
//  Copyright © 2017 Kyle Hess. All rights reserved.
//
//  Include rules:
//  Standard library headers needed for a file go in the header
//  Custom files needed for a file not needed in the header go in the .cpp,
//  unless said file is the globals.hpp, in which case it goes in header
//
//  Current functions untested
//  Currently implementing/converting old functions
//
///////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <string>
#include "Bigint.hpp"
#include "multiplication.hpp"

//std::string Bigint::OUTPUT_NUMBER_SYSTEM = "";

int main() {
    
    Bigint b(9,20000);
    
    std::cout << b << "\n";
    //std::cout << karatsuba(b, c) << "\n";
    
    
    
}



































