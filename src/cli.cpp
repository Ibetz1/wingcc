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
#include "cli.hpp"
#include "util.hpp"

using namespace std;

map<string, vector<string>> args = {};

/*
    prints command information
*/
void print_help() {
    cout << "wingcc list of available commands: " << endl << endl;;

    // available params
    cout << "----------------------------" << endl;
    cout << "list of params" << endl;
    cout << "--" << SRC_DIR << "   >> source dirs -> Ex: (src/)" << endl;
    cout << "--" << LNK_DIR << "   >> linked library dirs -> Ex: (lib/)" << endl;
    cout << "--" << INC_DIR << "   >> include dirs -> Ex: (include/)" << endl;;
    cout << "--" << OUT_DIR << "   >> output folder & executable name -> Ex: (build/) or (build/build.exe)" << endl;;
    cout << "--" << STD_ARG << "   >> version of c++ Ex: (c++17)" << endl;
    cout << "--" << GCC_ARG << "   >> args for g++ Ex: (-Wall)" << endl;
    cout << "--" << RUN_BUILD << "   >> will run after building" << endl;
    cout << "--" << NO_CLEAN << "   >> will not clean obj files after building" << endl;
    cout << "--" << DEL_EXE << "   >> will delete executible after cleaning" << endl;
    cout << "--" << NO_SAVE << "   >> will not cache build command" << endl;
    cout << "--" << NO_CACHE << "   >> will not read from command cache" << endl;
    cout << "--" << REC_LNK << "   >> will expand linked dirs to target each unit" << endl;
    cout << "----------------------------" << endl << endl;

    // example build
    cout << "----------------------------" << endl;
    cout << GEN_BUILD << " this will build your project example: " << endl; 
    cout << "wingcc.exe " << GEN_BUILD << " ";
    cout << "--" << SRC_DIR << " src ";
    cout << "--" << LNK_DIR << " lib -linker-arg ";
    cout << "--" << INC_DIR << " include -include-arg ";
    cout << "--" << OUT_DIR << " build/my_app.exe ";
    cout << "--" << STD_ARG << " c++17 ";
    cout << "--" << GCC_ARG << " -gcc-arg ";
    cout << "--" << LNK_DIR << " ";
    cout << "--" << RUN_BUILD << " ";
    cout << "--" << NO_CLEAN << " ";
    cout << "--" << NO_SAVE << endl;
    cout << "----------------------------" << endl << endl;


    // example run
    cout << "----------------------------" << endl;
    cout << RUN_BUILD << " this will run your built project, example: " << endl; 
    cout << "wingcc.exe " << RUN_BUILD << " ";
    cout << "--" << OUT_DIR << " build/ ";
    cout << "--" << NO_CLEAN << endl;
    cout << "----------------------------" << endl << endl;


    // example clean
    cout << "----------------------------" << endl;
    cout << CLEAN_BUILD << " this will clean your build folder, example: " << endl; 
    cout << "wingcc.exe " << CLEAN_BUILD << " ";
    cout << "--" << OUT_DIR << " build/ ";
    cout << "--" << DEL_EXE << endl;
    cout << "----------------------------" << endl << endl;

    // example del cache
    cout << "----------------------------" << endl;
    cout << CLEAN_CACHE << " this will delete the cache, example: " << endl; 
    cout << "wingcc.exe " << CLEAN_CACHE << endl;
    cout << "----------------------------" << endl << endl;

    // example mod cache
    cout << "----------------------------" << endl;
    cout << MOD_CACHE << " this will update params in your cache, example: " << endl; 
    cout << "wingcc.exe " << MOD_CACHE << " ";
    cout << "--" << SRC_DIR << " src ";
    cout << "--" << LNK_DIR << " lib ";
    cout << "--" << INC_DIR << " include ";
    cout << "--" << OUT_DIR << " build/my_app.exe ";
    cout << "--" << STD_ARG << " c++17 ";
    cout << "--" << GCC_ARG << " -Wall -g ";
    cout << "--" << RUN_BUILD << " ";
    cout << "--" << NO_CLEAN << endl;
    cout << "----------------------------" << endl << endl;
}

/*
    run the build script
*/
void build() {
    // user defined arguments
    string include_arg  = get_include_dirs();
    string linker_arg   = get_linker_args();
    string standard_arg = get_standard_args();
    string gcc_args     = get_gcc_args();

    // get build path
    pair<string, string> path_info = get_build_dir();
    string build_path = path_info.first;
    string build_name = path_info.second;

    /*
        generate objects
    */
    string source_dir = (args.count(SRC_DIR)) ? args.at(SRC_DIR).at(0) + "/" : ".";
    vector<string> sources = get_file_list(source_dir, ".cpp");
   
    string objects = "";
    vector<string> obj_list = {};
    for (string source : sources) {
        string obj = regex_replace(source, regex(".cpp"), ".o");
        string basename = obj.substr(obj.find_last_of("/\\") + 1);
        string path = build_path + basename;
        string cmd = "g++ -Wall -g -c " + source + " -o " + path;
        objects.append(path + " ");
        obj_list.push_back(path);
        cmd.append(" " + standard_arg);
        cmd.append(" " + include_arg);
        cmd.append(" " + linker_arg);
        cmd = regex_replace(cmd, regex("\\s{2,}"), " ");
        cout << "executing: " << cmd << endl;
        system(cmd.c_str());
    }
    
    /*
        build executable
    */
    string cmd = "g++ ";
    cmd.append(" " + gcc_args);
    cmd.append(" " + objects);
    cmd.append(" " + standard_arg);
    cmd.append(" " + include_arg);
    cmd.append(" " + linker_arg);
    cmd.append(" -o " + build_path + build_name);
    cmd = regex_replace(cmd, regex("\\s{2,}"), " ");
    cout << "executing: " << cmd << endl;
    system(cmd.c_str());
}

/*
    clean obj files
*/
void clean() {
    pair<string, string> path_info = get_build_dir();
    string build_path = path_info.first;
    string build_name = path_info.second;
    vector<string> obj_list = get_file_list(build_path, ".o");
    
    /*
        clean obj files
    */
    for (string obj : obj_list) {
        string cmd = "del " + regex_replace(obj, regex("/"), "\\");
        cout << "executing: " << cmd << endl;
        system(cmd.c_str());
    }

    /*
        clean executable
    */
    if (args.count(DEL_EXE)) {
        string cmd = "del " + regex_replace(build_path + build_name, regex("/"), "\\");
        cout << "executing: " << cmd << endl;
        system(cmd.c_str());
    }
}

/*
    generates parameter cache for furure builds
*/
void make_cache() {
    if (!filesystem::exists(SCRIPT_CACHE)) {
        filesystem::create_directory(SCRIPT_CACHE);
    }

    string cache_path = string(SCRIPT_CACHE) + "/";

    /*
        generate build script
    */
    cout << "caching build script" << endl;
    ofstream build_script (cache_path + string(SCRIPT_NAME) + string(SCRIPT_EXT));
    build_script.clear();

    // write to build file
    push_cache_param(build_script, SRC_DIR);
    push_cache_param(build_script, LNK_DIR);
    push_cache_param(build_script, INC_DIR);
    push_cache_param(build_script, OUT_DIR);
    push_cache_param(build_script, GCC_ARG);
    push_cache_param(build_script, STD_ARG);
    push_cache_param(build_script, REC_LNK);
    push_cache_param(build_script, NO_CLEAN);

    build_script.close();
}

/*
    clears out script cache
*/
void clear_cache() {
    vector<string> caches = get_file_list("./", ".buildscript");

    for (string file : caches) {
        string command = "del " + regex_replace(file, regex("/"), "\\");
        cout << "executing: " << command << endl;
        system(command.c_str());
    }
}

/*
    reads cached commands
*/
void read_cache() {
    string cachepath = string(SCRIPT_CACHE) + "/" + string(SCRIPT_NAME) + string(SCRIPT_EXT);
    if (filesystem::exists(cachepath)) {
        args.insert({string(NO_SAVE), {}});
        string line;
        ifstream file(cachepath);
        if (file.is_open()) {
            while (getline(file, line)) {
                pair<string, string> iparam = parse_cache_param(line);
                string pkey = regex_replace(iparam.first, regex(" "), "");
                string pval = iparam.second;

                // parse the key of each cached param
                if (!args.count(pkey)) {
                    args.insert({pkey, {}});
                    istringstream iss(pval);
                    string segment;
                    while (std::getline(iss, segment, ' ')) {
                        if (!segment.empty())
                            args.at(pkey).push_back(segment);
                    }
                }
            }
            file.close();
        }

    } else {
        cout << "could not locate a cache" << endl;
    }
}

/*
    search for a build executible
*/
void run() {
    pair<string, string> path_info = get_build_dir();
    string build_path = path_info.first;
    string build_name = path_info.second;

    string cmd = "";
    cmd.append("start ");
    cmd.append(build_path);
    cmd.append(build_name);

    cout << "executing: " << cmd << endl;
    system(cmd.c_str());
}
