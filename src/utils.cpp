#include <fstream>

#include "utils.hpp"

std::string readFile(std::string filename) {
	std::ifstream file(filename);
	std::string line;
	std::string contents;

	while (std::getline(file, line)) {
		contents += line + "\n";
	}

	return contents;
}

