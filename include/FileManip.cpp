#include "include/FileManip.hpp"
#include <fstream>
#include <sstream>
#include <iterator>
#include <vector>

std::vector<std::vector<float>> LoadFile(std::string fileName, int rows, int columns) {
	
	std::vector<std::vector<float>> returnVec(rows, std::vector<float>(columns));
	const int bufferSize = 100;
	
	std::ifstream fileStream;
	fileStream.open(fileName, std::ios_base::in);
	
	for (int i = 0; fileStream.good(); i++) {
		// Limit na sztywno, nie chce mi się tego uodparniać
		char buffer[bufferSize];
		fileStream.getline(buffer, bufferSize);
		
		// Skopiowane z neta. Tyle.
		std::istringstream buf(buffer);
		std::istream_iterator<std::string> beg(buf), end;
		
		std::vector<std::string> tokens(beg, end);
		
		std::vector<std::string>::iterator stringIt = tokens.begin();
		std::vector<std::string>::iterator stringEnd = tokens.end();
		std::vector<float>::iterator returnIt = returnVec[i].begin();
		std::vector<float>::iterator returnEnd = returnVec[i].end();
		
		for (; returnIt != returnEnd && stringIt != stringEnd; returnIt++, stringIt++) {
			*returnIt = atof(stringIt->c_str());
		}
	}
	
	return returnVec;
}