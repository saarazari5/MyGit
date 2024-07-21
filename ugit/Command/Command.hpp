//
//  Command.hpp
//  ugit
//
//  Created by Saar Azari on 07/06/2024.
//

#ifndef Command_hpp
#define Command_hpp

#include <stdio.h>
#include <vector>
#include <string>
#include "utils.h"

class CommandBase {
public:
    virtual ~CommandBase() = default;
};

template <typename T>
class Command {
public:
    virtual ~Command() = default;
    virtual T execute(const std::vector<std::string>& args) = 0;
};

class Init : public Command<void> {
public:
    void execute(const std::vector<std::string>& args) override;
};

class HashObject : public Command<void> {
public:
    void execute(const std::vector<std::string>& args ) override;
};

class CatFile : public Command<void> {
public:
    void execute(const std::vector<std::string>& args) override;
};

class WriteTree : public Command<std::string> {
private:
    bool isIgnored(const std::string& path );
public:
    std::string execute(const std::vector<std::string>& args) override;
};


class ReadTree: public Command<void> {
private:
    utils::StringMap get_tree(const std::string& oid, const std::string& base_path = "");
    utils::Entries iter_tree_entries(const std::string& oid);
public:
    void execute(const std::vector<std::string>& args) override;
};
#endif /* Command_hpp */
