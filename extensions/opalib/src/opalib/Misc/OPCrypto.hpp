//
//  OPCrypto.hpp
//  Persistor
//
//  Created by Adilson Tavares on 29/10/19.
//  Copyright © 2019 Adilson Tavares. All rights reserved.
//

#ifndef OPCrypto_hpp
#define OPCrypto_hpp

#include <iostream>
#include <vector>

class OPCrypto
{
private:
    
    static const unsigned char key[32];
    static const unsigned char inputVector[16];
    
    static const unsigned int bufferLength {1024 * 50}; // 50KB
    
    static int encrypt(unsigned char *original, int originalLength, unsigned char *ciphered);
    static int decrypt(unsigned char *ciphered, int cipheredLength, unsigned char *original);
    
public:
    
    static bool encrypt(const std::vector<unsigned char>& data, std::vector<unsigned char>& output);
    static bool decrypt(const std::vector<unsigned char>& data, std::vector<unsigned char>& output);
    
    static bool encrypt(const std::string& data, std::string& output);
    static bool decrypt(const std::string& data, std::string& output);
};

#endif /* OPCrypto_hpp */
