//
//  Command.cpp
//  ugit
//
//  Created by Saar Azari on 07/06/2024.
//

#include "Command.hpp"
#include "../IO/IO.hpp"
#include "../Encryption/Encryptor.hpp"
#include "data.h"
#include "utils.h"
#include <cassert>

//init command

void Init::execute(const std::vector<std::string>& args) {
    if(!fs::exists(GIT_DIR)) {
        fs::create_directory(GIT_DIR);
    }
    
    fs::create_directory(fs::path(GIT_DIR) / "objects");
}

void HashObject::execute(const std::vector<std::string>& args) {
    FileIO fileIO = FileIO(args[0]);
    std::string data = fileIO.read();
    
    // Construct the object
    std::string type = args.size() > 1  ? args[1] : "blob";
    hash_object(data, type);
}


void CatFile::execute(const std::vector<std::string>& args) {
    std::string expected = args.size() > 1  ? args[1] : "blob";
    std::cout.flush();
    std::vector<char> content = get_object(args[0], expected);
    std::fwrite(content.data(), sizeof(char), content.size(), stdout);
}


bool WriteTree::isIgnored(const std::string &path) {
    std::istringstream stream(path);
    std::string segment;
    while (std::getline(stream, segment, '/')) {
        if (ignored_files.find(segment) != ignored_files.end()) {
            return true;
        }
    }
    return false;
}

std::string WriteTree::execute(const std::vector<std::string>& args) {
    utils::Entries entries;
    const std::string& directory = args[0];
    try {
        if (!fs::exists(directory)) {
            std::cerr << "Directory does not exist: " << directory << std::endl;
            return "";
        }
        std::string oid;
        std::string type;
        for (const auto& entry : fs::directory_iterator(directory)) {
            auto full = directory + "/" + entry.path().filename().string();
            if (this->isIgnored(full)) { continue; }
            if (fs::is_regular_file(entry.path())) {
                type = "blob";
                std::string data = FileIO(full).read();
                oid = hash_object(data, type);
            } else if (fs::is_directory(entry.path())) {
                type = "tree";
                oid = this->execute({full});
            }
            entries.emplace_back(entry.path().filename().string(), oid, type);
        }
        
        // Sort entries and construct the tree object
        std::sort(entries.begin(), entries.end());
        std::ostringstream tree;
        for (const auto& [name, oid, type] : entries) {
            tree << type << " " << oid << " " << name << "\n";
        }
        std::string tree_content = tree.str();
        return hash_object(tree_content, "tree");
        
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    
    return "";
}

utils::Entries ReadTree::iter_tree_entries(const std::string &oid) {
   utils::Entries entries;
    if (oid.empty()) return entries;
    
    std::vector<char> tree_vec = get_object(oid, "tree");
    std::string tree(tree_vec.begin(), tree_vec.end());
    for (const std::string& entry : split_lines(tree)) {
        std::istringstream iss(entry);
        std::string type, oid, name;
        iss >> type >> oid >> name;
        entries.emplace_back(type, oid, name);
    }
    return entries;
}

utils::StringMap ReadTree::get_tree(const std::string &oid, const std::string& base_path) {
    utils::StringMap result;
    for (const auto& [type, oid, name] : iter_tree_entries(oid)) {
        assert(name.find('/') == std::string::npos);
        assert(name != ".." && name != ".");
        std::string path = base_path + name;
        if (type == "blob") {
            result[path] = oid;
        } else if (type == "tree") {
            auto sub_result = get_tree(oid, path + "/");
            result.insert(sub_result.begin(), sub_result.end());
        } else {
            assert(false && "Unknown tree entry type");
        }
    }
    return result;
}

void ReadTree::execute(const std::vector<std::string>& args) {
    const std::string tree_oid = args[0];
    auto tree = get_tree(tree_oid, "./");
    for (const auto& [path, oid] : tree) {
        std::filesystem::create_directories(std::filesystem::path(path).parent_path());
        std::ofstream out(path, std::ios::binary);
        if (!out) {
            std::cerr << "Failed to open file for writing: " << path << std::endl;
            continue;
        }
        
        std::vector<char> content_vec = get_object(oid);
        std::string content(content_vec.begin(), content_vec.end());
        out.write(content.c_str(), content.size());
    }
}
