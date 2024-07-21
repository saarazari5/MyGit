//
//  data.h
//  ugit
//
//  Created by Saar Azari on 14/06/2024.
//

#ifndef data_h
#define data_h

#include <string>
#include "../Encryption/Encryptor.hpp"
#include <fstream>

#include "utils.h"


using string = std::string;
string hash_object(string data, string type = "blob") {
    std::vector<char> obj(type.begin(), type.end());
    obj.push_back('\n');
    obj.insert(obj.end(), data.begin(), data.end());
    
    string oid = SHA1().encrypt(string(obj.begin(), obj.end()));
    std::filesystem::path object_dir = std::filesystem::path(GIT_DIR) / "objects";
    std::filesystem::path object_path = object_dir / oid;
    
    // Create directories if they don't exist
    std::filesystem::create_directories(object_dir);
    
    std::ofstream out(object_path, std::ios::binary);
    if (!out) {
        std::cerr << "Failed to open file for writing: " << object_path << std::endl;
        return "";
    }
    
    out.write(obj.data(), obj.size());
    std::cout << "Object ID: " << oid << std::endl;
    
    return oid;
}

std::vector<char> get_object(string oid, string expected = "blob") {
    fs::path filePath = std::filesystem::path(GIT_DIR) / "objects" / oid;
    std::ifstream file(filePath, std::ios::binary);
    
    if (!file) {
        throw std::runtime_error("Unable to open file: " + filePath.string());
    }
    
    // Read the file contents into a vector
    std::vector<char> buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    
    auto nullPos = std::find(buffer.begin(), buffer.end(), '\n');
    std::string type(buffer.begin(), nullPos);
    
    if (type != expected) {
        throw std::runtime_error("Expected" + expected + ", got " + type);
    }
    
    std::vector<char> content(nullPos + 1, buffer.end());
    return content;
}

#endif /* data_h */
