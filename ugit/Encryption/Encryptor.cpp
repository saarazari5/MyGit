//
//  Encryptor.cpp
//  ugit
//
//  Created by Saar Azari on 08/06/2024.
//

#include "Encryptor.hpp"
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstring>
#include <cstdint>
#include <stdio.h>
#include <iostream>

using namespace std;

string SHA1::encrypt(const std::string& text) {
    SHA1 sha1;
    sha1.update(text);
    return sha1.final();
}

SHA1::SHA1() {
    reset();
}

void SHA1::reset() {
    digest[0] = 0x67452301;
    digest[1] = 0xEFCDAB89;
    digest[2] = 0x98BADCFE;
    digest[3] = 0x10325476;
    digest[4] = 0xC3D2E1F0;
    transforms = 0;
    buffer = "";
}

void SHA1::update(const std::string &s) {
    std::istringstream is(s);
    update(is);
}

void SHA1::update(std::istream &is) {
    std::string rest_of_buffer;
    read(is, rest_of_buffer, BLOCK_BYTES - buffer.size());
    buffer += rest_of_buffer;

    while (buffer.size() >= BLOCK_BYTES) {
        uint32 block[BLOCK_INTS];
        buffer_to_block(buffer, block);
        transform(block);
        buffer.erase(0, BLOCK_BYTES);
    }
}

std::string SHA1::final() {
    uint64 total_bits = (transforms * BLOCK_BYTES + buffer.size()) * 8;

    buffer += static_cast<char>(0x80);
    unsigned int orig_size = buffer.size();
    while (buffer.size() < BLOCK_BYTES) {
        buffer += static_cast<char>(0x00);
    }

    uint32 block[BLOCK_INTS];
    buffer_to_block(buffer, block);

    if (orig_size > BLOCK_BYTES - 8) {
        transform(block);
        for (unsigned int i = 0; i < BLOCK_INTS - 2; i++) {
            block[i] = 0;
        }
    }

    block[BLOCK_INTS - 1] = total_bits;
    block[BLOCK_INTS - 2] = (total_bits >> 32);
    transform(block);

    std::ostringstream result;
    for (unsigned int i = 0; i < 5; i++) {
        result << std::hex << std::setw(8) << std::setfill('0') << digest[i];
    }

    reset();

    return result.str();
}

void SHA1::transform(uint32 block[BLOCK_BYTES]) {
    uint32 a = digest[0];
    uint32 b = digest[1];
    uint32 c = digest[2];
    uint32 d = digest[3];
    uint32 e = digest[4];

    for (unsigned int i = 0; i < 80; i++) {
        uint32 f, k;

        if (i < 20) {
            f = (b & c) | ((~b) & d);
            k = 0x5A827999;
        } else if (i < 40) {
            f = b ^ c ^ d;
            k = 0x6ED9EBA1;
        } else if (i < 60) {
            f = (b & c) | (b & d) | (c & d);
            k = 0x8F1BBCDC;
        } else {
            f = b ^ c ^ d;
            k = 0xCA62C1D6;
        }

        uint32 temp = (a << 5 | a >> 27) + f + e + k + block[i % BLOCK_INTS];
        e = d;
        d = c;
        c = b << 30 | b >> 2;
        b = a;
        a = temp;
    }

    digest[0] += a;
    digest[1] += b;
    digest[2] += c;
    digest[3] += d;
    digest[4] += e;
    transforms++;
}

void SHA1::buffer_to_block(const std::string &buffer, uint32 block[BLOCK_BYTES]) {
    for (unsigned int i = 0; i < BLOCK_INTS; i++) {
        block[i] = (buffer[4 * i + 3] & 0xff)
                 | (buffer[4 * i + 2] & 0xff) << 8
                 | (buffer[4 * i + 1] & 0xff) << 16
                 | (buffer[4 * i + 0] & 0xff) << 24;
    }
}

void SHA1::read(std::istream &is, std::string &s, std::size_t max) {
    char sbuf[max];
    is.read(sbuf, max);
    s.assign(sbuf, is.gcount());
}
