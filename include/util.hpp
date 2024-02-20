//!
//! wingcc CLI made by Ian Betz
//! date: 2/19/24
//! dependencies: Windows OS, gcc in system path
//!

#ifndef _WINGCC_UTL
#define _WINGCC_UTL

#include <iostream>
#include <string>
#include <vector>

using namespace std;

/*
    get a build directory
*/
pair<string, string> get_build_dir();

/*
    parses a cache parameter
*/
pair<string, string> parse_cache_param(string param);

/*
    this will store a script in cache
*/
string push_cache_param(ofstream& file, string param_name);

/*
    format chunk with its args "--command input -arg"
*/
string format_command(vector<string> files);

/*
    pull args from a command "--command -arg"
*/
vector<string> fetch_command_args(vector<string> command);
/*
    pulls params from a command "--command param -arg"
*/
vector<string> fetch_command_params(vector<string> command);

/*
    flatten a vector into a string
*/
string format_vector_string(vector<string> input);

/*
    get list of files in a directory and its sub-directories
*/
vector<string> get_file_list(string dir, string ext);

/*
    verify a list of directories
*/
vector<string> verify_dir_list(vector<string> dirs);

/*
    get user defined include directories
*/
string get_include_dirs();

/*
    get user defined linker args
*/
string get_linker_args();

/*
    get user defined c++ standard
*/
string get_standard_args();

/*
    get user defined gcc args
*/
string get_gcc_args();

#endif