
#include "IO/IO.hpp"
#include "Command/Command.hpp"
#include <map>
#include <filesystem>

using namespace std;

 map<string, CommandBase*> mp;

 vector<string> parseArguments(int argc, const char *argv[]) {
    vector<string> arguments;
    for (int i = 1; i < argc; ++i) {
        arguments.push_back(string(argv[i]));
    }
    return arguments;
}

int main(int argc, const char * argv[]) {
    
    std::string new_dir = "/Users/saarazari/Desktop/ugit_tests";

    try {
        std::filesystem::current_path(new_dir);
        std::cout << "Successfully changed working directory to " << std::filesystem::current_path() << std::endl;
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error changing directory: " << e.what() << std::endl;
        return 1;
    }
    
    std::vector<std::string> args;
    args.push_back("e5e7cfd47bded1c2d2340dd11e27e997b96873a8");
    ReadTree* c = new ReadTree();
    
    c->execute(args);
//    Command* c = new Init();
//    c->execute(args);
//    c = new HashObject();
//    args.push_back("test.txt");
//    c->execute(args);
//    args.clear();
//    c = new CatFile();
//    args.push_back("39dd3fc16d8f3b1d0bdb6c4fbd855a4affc7008f");
//    c->execute(args);
    
//    IO* io = new CLI();
//    //fetch all available commands
//    
//    //parse arguments
//    vector<string> parsed = parseArguments(argc, argv);
//    
//    if(parsed.empty() || parsed.at(0) != "ugit") {
//        std::cerr << "Error" << std::endl;
//        return 1;
//    }
//    
//    if(parsed.size() == 1) {
//        io -> write("Welcome to ugit version 1.0 enjoy!");
//        return 0;
//    }
//    
//    vector<string>args;
//    copy(parsed.begin() + 2, parsed.end(), std::back_inserter(args));
//
//    auto it = mp.find(parsed[1]);
//    if (it == mp.end()) {
//        std::cerr << "Command not found" << std::endl;
//        return 1;
//    }
//    
//    it -> second -> execute(args);
//        
//    
//    
//    std::cout << "Hello, World!\n";
    return 0;
}
