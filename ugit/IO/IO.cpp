//
//  IO.cpp
//  ugit
//
//  Created by Saar Azari on 07/06/2024.
//

#include "IO.hpp"
#include <fstream>

// Implementation of CLI class
std::string CLI::read() {
    std::string input;
    std::cout << "Enter input: ";
    std::getline(std::cin, input);
    return input;
}

void CLI::write(const std::string &message) {
    std::cout << message << std::endl;
}

// Implementation of FileIO class
FileIO::FileIO(const std::string &filename) : filename(filename) {}

std::string FileIO::read() {
    std::ifstream file(filename);
    std::string input;
    if (file.is_open()) {
        std::getline(file, input, '\0'); // Read entire file content
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
    return input;
}

void FileIO::write(const std::string &message) {
    std::ofstream file(filename, std::ios::binary | std::ios::app); // Append mode
    if (file.is_open()) {
        file.write(message.c_str(), message.size());
        file.close();
    } else {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}
