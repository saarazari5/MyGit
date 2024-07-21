//
//  utils.cpp
//  ugit
//
//  Created by Saar Azari on 14/06/2024.
//

#include "utils.h"

std::vector<std::string> split_lines(const std::string& str) {
   std::vector<std::string> lines;
   std::istringstream f(str);
   std::string line;
   while (std::getline(f, line)) {
       lines.push_back(line);
   }
   return lines;
}
