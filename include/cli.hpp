//!
//! wingcc CLI made by Ian Betz
//! date: 2/19/24
//! dependencies: Windows OS, gcc in system path
//!

#ifndef _WINGCC_CLI
#define _WINGCC_CLI

#include <map>
#include <vector>
#include <string>

/*
    inputs
*/

// gcc options
#define SRC_DIR "src" // set the source director for your code
#define LNK_DIR "lib" // set files to link to on build
#define INC_DIR "inc" // set the include directory
#define OUT_DIR "out" // set the output folder
#define GCC_ARG "arg" // set g++ args
#define STD_ARG "std" // set the gcc standard

// build options
#define NO_CLEAN "no-clean" // does not clean obj files after building
#define NO_SAVE  "no-save"  // cancels saving of commands during build
#define NO_CACHE "no-cache" // does not attempt to read from cache
#define DEL_EXE  "del-exe"  // deletes the built executible during clean
#define REC_LNK  "exp-lnk"  // expands linkage to target each file

// tools
#define GEN_BUILD   "build"     // runs a build script
#define RUN_BUILD   "run"       // runs a built executible
#define CLEAN_BUILD "clean"     // cleans out a build directory
#define CLEAN_CACHE "del-cache" // deletes the script cache
#define MOD_CACHE   "mod-cache" // add an arg to the cache
#define HELP        "help"      // prints help information

// cache name
#define SCRIPT_CACHE ".build-cache"
#define SCRIPT_NAME  "params"
#define SCRIPT_EXT   ".buildscript"

using namespace std;

/*
    args passed to build command
*/
extern map<string, vector<string>> args;


/*
    prints command information
*/
void print_help();

/*
    generates parameter cache for furure builds
*/
void make_cache();

/*
    clears out script cache
*/
void clear_cache();

/*
    reads cached commands
*/
void read_cache();

/*
    search for a build executible
*/
void run();

/*
    run the build script
*/
void build();

/*
    clean obj files
*/
void clean();

#endif