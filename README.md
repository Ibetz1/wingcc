# WINGCC: Windows Command Line Interface

This is a command-line interface (CLI) tool written in C++ specifically designed to run on the Windows operating system. It offers various functionalities for users to interact with their system and perform specific tasks efficiently.

## Features

- **Windows Compatibility**: This CLI tool is built exclusively for Windows systems and leverages specific Windows APIs for functionality.
- **User-Friendly Commands**: The CLI provides intuitive commands and options for users to perform tasks effortlessly.
- **Dependency on GCC G++**: The tool relies on the GNU Compiler Collection's C++ compiler (GCC G++) to build and run. Ensure GCC G++ is installed and accessible in your system's PATH environment variable.

## Installation

To install and use this CLI tool, follow these steps:

1. **Install GCC G++**: If not already installed, download and install GCC G++ for Windows. Ensure it's added to your system's PATH environment variable.

2. **Clone the Repository**: Clone this repository to your local machine using the following command:
   
```bash
git clone https://github.com/Ibetz1/wingcc.git
```

3. **Build the CLI Tool**: Navigate to the directory where you cloned the repository and build the CLI tool using GCC G++. Run the following command:

```bash
g++ src/*.cpp -I include -o bin/wingcc.exe
```

## Usage

wingcc list of available commands:

**list of params**
```
--src   >> source dirs -> Ex: (src/)
--lib   >> linked library dirs -> Ex: (lib/)
--inc   >> include dirs -> Ex: (include/)
--out   >> output folder & executable name -> Ex: (build/) or (build/build.exe)
--std   >> version of c++ Ex: (c++17)
--arg   >> args for g++ Ex: (-Wall)
--run   >> will run after building
--no-clean   >> will not clean obj files after building
--del-exe   >> will delete executible after cleaning
--no-save   >> will not cache build command
--no-cache   >> will not read from command cache
```

**build** build your project
```
wingcc.exe build --src src --lib lib -linker-arg --inc include -include-arg--out build/my_app.exe --std c++17 --arg -gcc-arg --run --no-clean --no-save
```

**run** run your built project
```
wingcc.exe run --out build/ --no-clean
```

**clean** clean your build folder
```
wingcc.exe clean --out build/ --del-exe
```

**del-cache** delete the cache
```
wingcc.exe del-cache
```

**mod-cache** update the params in your cache:
```
wingcc.exe mod-cache --src src --lib lib --inc include --out build/my_app.exe --std c++17 --arg -Wall -g --run --no-clean
```
## Contributing

Contributions are welcome! If you'd like to enhance this CLI tool or fix any issues, feel free to fork this repository, make your changes, and submit a pull request. Please ensure your code follows the project's coding conventions and includes appropriate documentation.

License

This project is licensed under the MIT License, allowing for both personal and commercial use.

## Disclaimer

This CLI tool is provided as-is without any warranty. Use it at your own risk. We disclaim any liability for damages resulting from its use.
