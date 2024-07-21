//
//  IO.hpp
//  ugit
//
//  Created by Saar Azari on 07/06/2024.
//

#ifndef IO_hpp
#define IO_hpp

#include <iostream>
#include <string>


// Abstract base class
class IO {
public:
    virtual ~IO() = default;
    virtual std::string read() = 0;
    virtual void write(const std::string &message) = 0;
};

// CLI class for standard input/output
class CLI : public IO {
public:
    std::string read() override;
    void write(const std::string &message) override;
};

// FileIO class for file input/output
class FileIO : public IO {
private:
    std::string filename;
public:
    FileIO(const std::string &filename);
    std::string read() override;
    void write(const std::string &message) override;
};

#endif /* IO_hpp */
