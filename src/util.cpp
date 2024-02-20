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

/*
    format chunk with its args "--command input -arg"
*/
string format_command(vector<string> files) {
    string dir_out  = "";
    string dir_args = "";

    /*
        get files in directory
    */
    for (string dir : files) {
        if (dir.at(0) == '-') {
            dir_args.append(dir + " ");
        }
        else if (filesystem::exists(dir)) {
            dir_out.append(dir + " ");
        } else {
            cout << "failed to find directory: " << dir << endl;
        }
    }

    return dir_out + dir_args;
}

/*
    pull args from a command "--command -arg"
*/
vector<string> fetch_command_args(vector<string> command) {
    vector<string> args = {};

    for (string param : command) {
        if (param.at(0) == '-') args.push_back(param + " ");
    }

    return args;
}

/*
    pulls params from a command "--command param -arg"
*/
vector<string> fetch_command_params(vector<string> command) {
    vector<string> params = {};

    for (string param : command) {
        if (param.at(0) != '-') params.push_back(param);
    }

    return params;
}

/*
    flatten a vector into a string
*/
string format_vector_string(vector<string> input) {
    string out = "";
    for (string v : input) {
        out.append(v + " ");
    }
    return out;
}

/*
    get list of files in a directory and its sub-directories
*/
vector<string> get_file_list(string dir, string ext) {
    vector<string> files = {};

    if (filesystem::exists(dir)) {
        for (auto &p : filesystem::recursive_directory_iterator(dir))
        {
            if (p.path().extension() == ext) {
                string cur = p.path().string();
                std::replace(cur.begin(), cur.end(), '\\', '/');
                files.push_back(cur);
            }
        }
    } else {
        cout << "directory does not exist: " << dir << endl;
    }

    return files;
}

/*
    verify a list of directories
*/
vector<string> verify_dir_list(vector<string> dirs) {
    vector<string> ret = {};

    for (string dir : dirs) {
        if (filesystem::exists(dir)) {
            ret.push_back(dir);
        } else {
            cout << "file does not exist: " << dir << endl;
        }
    }

    return ret;
} 

/*
    get user defined include directories
*/
string get_include_dirs() {
    string include_arg = "";
    if (args.count(INC_DIR)) {
        vector<string> folders = verify_dir_list(fetch_command_params(args.at(INC_DIR)));
        vector<string> params  = fetch_command_args(args.at(INC_DIR));

        include_arg.append("-I ");
        if (folders.size()) {
            include_arg.append(format_vector_string(folders));
        }
        include_arg.append(format_vector_string(params));
    }

    return include_arg;
}

/*
    get user defined linker args
*/
string get_linker_args() {

    string linker_arg = "";


    if (args.count(LNK_DIR)) {
        vector<string> folders = verify_dir_list(fetch_command_params(args.at(LNK_DIR)));
        vector<string> params  = fetch_command_args(args.at(LNK_DIR));

        linker_arg.append("-L ");
        if (folders.size()) {
            if (args.count(REC_LNK)) {
                for (string folder : folders) {
                    vector<string> files = get_file_list(folder, ".a");
                    linker_arg.append(format_vector_string(files));
                }
            } else {
                linker_arg.append(format_vector_string(folders));
            }
        }

        linker_arg.append(format_vector_string(params));
    }

    return linker_arg;
}

/*
    get user defined c++ standard
*/
string get_standard_args() {
    string standard = (args.count(STD_ARG)) ? args.at(STD_ARG).at(0) : "c++11";
    string standard_arg = "-std=" + standard;
    return standard_arg;
}

/*
    get user defined gcc args
*/
string get_gcc_args() {
    if (args.count(GCC_ARG)) {
        return format_vector_string(fetch_command_args(args.at(GCC_ARG)));
    }
    return "";
}

/*
    get a build directory
*/
pair<string, string> get_build_dir() {
    string basepath = "./";
    string basename = "build.exe";

    if (args.count(OUT_DIR)) {
        string arg = args.at(OUT_DIR).at(0);
        
        // string basepath = arg.substr(0, arg.find_last_of("\\/"));
        string tempname = filesystem::path(arg).filename().string();
        if (!tempname.empty()) basename = tempname;
        basepath += filesystem::path(arg).parent_path().string();

        // generate base path
        stringstream ss(basepath);
        string segment;
        string cwd;

        // build file path
        while (std::getline(ss, segment, '/')) {
            cwd += segment;
            if (!filesystem::exists(cwd)) 
                filesystem::create_directory(cwd);
            cwd += "/";
        }

        basepath += '/';
    }

    return {basepath, basename};
}

/*
    this will store a script in cache
*/
string push_cache_param(ofstream& file, string param_name) {
    if (!args.count(param_name)) return "";

    string param = "[" + string(param_name) + "]";
    for (string arg : args.at(param_name)) {
        param.append(" " + arg);
    }
    file << param << endl;
    return param;
}

/*
    parses a cache parameter
*/
pair<string, string> parse_cache_param(string param) {
    size_t lindex = param.find('[');
    size_t rindex = param.find(']');

    if (lindex != string::npos && rindex != string::npos) {
        string key = param.substr(lindex + 1, rindex - 1);
        string val = param.substr(rindex + 1, param.length());
        return {key, val};
    }

    return {"", ""};
}