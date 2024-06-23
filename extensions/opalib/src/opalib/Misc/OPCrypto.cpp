//
//  OPCrypto.cpp
//  Persistor
//
//  Created by Adilson Tavares on 29/10/19.
//  Copyright © 2019 Adilson Tavares. All rights reserved.
//

#include "OPCrypto.hpp"

#include <openssl/conf.h>
#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/crypto.h>

const unsigned char OPCrypto::key[] = {
    0xB2, 0x4B, 0x14, 0x20, 0xF2, 0xAC, 0x28, 0x4E,
    0x4E, 0x7E, 0x39, 0x4,  0x3F, 0xC0, 0xCE, 0x36,
    0x7E, 0x47, 0x25, 0xEE, 0xCB, 0x3F, 0x34, 0xB0,
    0xAF, 0x67, 0xE4, 0x16, 0xC2, 0x8A, 0x6,  0xEE
};

const unsigned char OPCrypto::inputVector[] = {
    0x4E, 0x7E, 0x39, 0x4,  0x3F, 0xC0, 0xCE, 0x36,
    0xB4, 0x60, 0x6C, 0x52, 0x4B, 0x6A, 0x29, 0x6B
};

int OPCrypto::encrypt(unsigned char *original, int originalLength, unsigned char *ciphered)
{
    EVP_CIPHER_CTX *ctx;
    int length, cipheredLength;

    if (!(ctx = EVP_CIPHER_CTX_new())) {
        return -1;
    }

    if (1 != EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, inputVector)) {
        return -1;
    }

    if (1 != EVP_EncryptUpdate(ctx, ciphered, &length, original, originalLength)) {
        return -1;
    }
    
    cipheredLength = length;

    if (1 != EVP_EncryptFinal_ex(ctx, ciphered + length, &length)) {
        return -1;
    }
    
    cipheredLength += length;

    EVP_CIPHER_CTX_free(ctx);

    return cipheredLength;
}

int OPCrypto::decrypt(unsigned char *ciphered, int cipheredLength, unsigned char *original)
{
    EVP_CIPHER_CTX *ctx;
    int length, originalLength;
    
    if (!(ctx = EVP_CIPHER_CTX_new())) {
        return -1;
    }
    
    if (1 != EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, inputVector)) {
        return -1;
    }
    
    if(1 != EVP_DecryptUpdate(ctx, original, &length, ciphered, cipheredLength)) {
        return -1;
    }
    
    originalLength = length;
    
    if (1 != EVP_DecryptFinal_ex(ctx, original + length, &length)) {
        return -1;
    }
    
    originalLength += length;
    original[originalLength] = 0;

    EVP_CIPHER_CTX_free(ctx);

    return originalLength;
}

bool OPCrypto::encrypt(const std::vector<unsigned char>& data, std::vector<unsigned char>& output)
{
    unsigned char raw[bufferLength];
    
    auto length = encrypt((unsigned char*)&data.front(), (int)data.size(), (unsigned char*)raw);
    if (length < 0) {
        return false;
    }
    
    output = std::vector<unsigned char>(raw, raw + length);
    return true;
}

bool OPCrypto::decrypt(const std::vector<unsigned char>& data, std::vector<unsigned char>& output)
{
    unsigned char raw[bufferLength];
    
    auto length = decrypt((unsigned char*)&data[0], (int)data.size(), (unsigned char*)raw);
    if (length < 0) {
        return false;
    }
    
    output = std::vector<unsigned char>(raw, raw + length);
    return true;
}

bool OPCrypto::encrypt(const std::string& str, std::string& output)
{
    if (str.empty())
        return false;
    
    unsigned char raw[bufferLength];
    
    auto length = encrypt((unsigned char*)str.data(), (int)str.size(), raw);
    if (length < 0) {
        return false;
    }
    
    output = std::string(raw, raw + length);
    
    return true;
    
//    std::vector<unsigned char> input(str.begin(), str.end());
//    std::vector<unsigned char> outputVector;
//
//    auto success = encrypt(input, outputVector);
//    output = std::string(outputVector.begin(), outputVector.end());
//
//    return success;
}

bool OPCrypto::decrypt(const std::string& str, std::string& output)
{
    std::vector<unsigned char> input(str.begin(), str.end());
    std::vector<unsigned char> outputVector;
    
    auto success = decrypt(input, outputVector);
    output = std::string(outputVector.begin(), outputVector.end());
    
    return success;
}
