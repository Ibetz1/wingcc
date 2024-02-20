/*
    wingcc CLI made by Ian Betz
    date: 2/19/24
    dependencies: Windows OS, gcc in system path
*/

#include "cli.hpp"
#include "util.hpp"
#include <stdio.h>
#include <iostream>
#include <vector>
#include <filesystem>
#include <map>
#include "string.h"
#include <algorithm>
#include <regex>
#include <sstream>
#include <fstream>


using namespace std;

/*
    handles different tools
*/
static int handle_command(string command) {
    // gen build command
    if (command == GEN_BUILD) {
        build();

        // cache commands
        if (!args.count(NO_SAVE)) {
            make_cache();
        }

        // clean objs
        if (!args.count(NO_CLEAN)) {
            clean();
        }

        // run the script
        if (args.count(RUN_BUILD)) {
            run();
        }

        return 0;
    }

    // clean build command
    if (command == CLEAN_BUILD) {
        clean();
        return 0;
    }

    // run build command
    if (command == RUN_BUILD) {
        if (args.count(GEN_BUILD)) {
            build();

            // clean objs
            if (!args.count(NO_CLEAN)) {
                clean();
            }
        }

        run();
        return 0;
    }

    // clear cache command
    if (command == CLEAN_CACHE) {
        clear_cache();
        return 0;
    }

    // prints help commands
    if (command == HELP) {
        print_help();
        return 0;
    }

    // clears updates cache
    if (command == MOD_CACHE) {
        clear_cache(); // deletes old stuff (old commands are in memory)
        make_cache();  // saves new stuff
        cout << "updated cache" << endl;
        return 0;
    }

    cout << "invalid command, use comand \"help\" for a list of commands" << endl;
    return 0;
}

/*
    will parse command args
*/
int main(int argc, char* argv[]) {
    if (argc == 1) {
        cout << "invalid command, use comand \"help\" for a list of commands" << endl;
        return -1;
    }

    // read input params
    string cur_key = "";
    for (int i = 2; i < argc; i++) {
        if (argv[i][0] == '-' && argv[i][1] == '-') {
            string key = string(argv[i]).substr(2, strlen(argv[i]));
            args.insert({key, {}});
            cur_key = key;
        }
        else if (args.count(cur_key)) {
            args.at(cur_key).push_back(string(argv[i]));
        }
    }
    
    if (!args.count(NO_CACHE)) 
        read_cache();

    return handle_command(string(argv[1]));
}