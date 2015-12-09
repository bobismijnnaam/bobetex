## Dependencies:
	* [Conan](http://conan.io)
	* [CMake](http://cmake.org)

## Compiling steps
1. Have the project extracted and all in a directory (`git pull` or unzip the project archive)
2. Open a command prompt with all the environment variables set properly
	* This means opening a visual studio dev prompt if you're on windows
3. `conan install`, possibly including various `-s` or `--build` flags if the libraries need to be built for your compiler
	* This will generate various dll's in the bin folder in the root of the project. Distribute these with your game; they're needed to run the btx engine.
4. `mkdir build`
5. `cd build`
6. `cmake .. -G "YOUR GENERATOR"`
7. Now build the project with the generated files
	* This probably means typing `make` if you're on Unix, or opening the solution file if you're on windows.
8. After compilation the btx binary should end up in the bin folder in the project root.

You should now have a working btx executable in the bin folder, together with the binaries needed to run it. Go play a game!
