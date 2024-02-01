#include "pch.h"
#include "Utils.h"

void empti_the_file(const std::string& file_name)
{
	std::ofstream MyFile(file_name);
	MyFile << "";
	MyFile.close();
}

std::string read_from_file(const std::string& file_name)
{
	std::string buffer;
	std::ifstream MyFile;
	MyFile.open(file_name);
	if (!MyFile.is_open()) return "READING FILE ISSUE";
	MyFile.seekg(0, std::ios::end);
	buffer.resize(MyFile.tellg());
	MyFile.seekg(0, std::ios::beg);
	MyFile.read(&buffer[0], buffer.size());
	MyFile.close();
	empti_the_file(file_name);//empti the file
	return buffer;
}
