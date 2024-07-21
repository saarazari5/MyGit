//
//  utils.h
//  ugit
//
//  Created by Saar Azari on 14/06/2024.
//

#ifndef utils_h
#define utils_h

#include <set>
#include <string>
#include <sstream>
#include <filesystem>


#define GIT_DIR ".ugit"

namespace fs = std::filesystem;

namespace utils {
    using Entries = std::vector<std::tuple<std::string, std::string, std::string>>;
    using StringMap = std::unordered_map<std::string, std::string>;
}

const std::set<std::string> ignored_files = {
    GIT_DIR,
    ".DS_Store",
    "Thumbs.db"
};


std::vector<std::string> split_lines(const std::string& str);

#endif /* utils_h */
