//
//  Encryptor.hpp
//  ugit
//
//  Created by Saar Azari on 08/06/2024.
//

#ifndef Encryptor_hpp
#define Encryptor_hpp


#include <string>


class Encryptor {
public:
    virtual ~Encryptor() = default;
    virtual std::string encrypt(const std::string& text) = 0;
};

class SHA1 : public Encryptor {
public:
    virtual std::string encrypt(const std::string& text) override;
    SHA1();

private:
    typedef uint32_t uint32;   // just for convenience
    typedef uint64_t uint64;   // just for convenience

    static const unsigned int BLOCK_INTS = 16;  // number of 32bit integers per SHA1 block
    static const unsigned int BLOCK_BYTES = BLOCK_INTS * 4;

    uint32 digest[5];
    std::string buffer;
    uint64 transforms;
    void update(const std::string &s);
    void update(std::istream &is);
    std::string final();
    void reset();
    void transform(uint32 block[BLOCK_BYTES]);
    static void buffer_to_block(const std::string &buffer, uint32 block[BLOCK_BYTES]);
    static void read(std::istream &is, std::string &s, std::size_t max);
};



#endif /* Encryptor_hpp */
