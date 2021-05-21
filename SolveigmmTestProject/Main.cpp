#include <stdlib.h>

#include <string>
#include <iostream>

#include "bitmap_informations.h"
#include "file_operations.h"

// https://google.github.io/styleguide/cppguide.html
int main(int argc, char* argv[])
{

	if (argc == 1) {
		std::cout << "There are no arguments" << std::endl;
		return 0;
	}

	std::string input_file_name("Empty");
	std::string output_file_name("Empty");
	
	int brightness = 0;
	
	for (int i = 1; i < argc; i += 2) {
		if (strcmp(argv[i], "-input") == 0) {
			input_file_name = argv[i + 1];
		}
		else if (strcmp(argv[i], "-brightness") == 0) {
			brightness = std::stoi(argv[i + 1]);
		}
		else if (strcmp(argv[i], "-output") == 0) {
			output_file_name = argv[i + 1];
		}
	}
	
	int input_value = 0;
	BMPFormat file_information;
	
	file_information = ReadFromFile(input_file_name);

	if (brightness < 0) brightness = 0;
	else if (brightness > 255) brightness = 255;
	
	ApplyBrightness(file_information, brightness);
	
	WriteToFile(output_file_name, file_information);
	
	return 0;
}
