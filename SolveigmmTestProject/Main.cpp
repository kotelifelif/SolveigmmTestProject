#include <stdlib.h>

#include <string>
#include <iostream>

#include "bitmap_informations.h"
#include "file_operations.h"

// https://google.github.io/styleguide/cppguide.html
int main()
{
	std::string input_file_name("Empty");
	std::string output_file_name("Empty");
	int brightness = 0;
	int input_value = 0;
	BMPFormat file_information;
	while (true)
	{
		std::cout << "1. Choose input file name" << std::endl << input_file_name << std::endl;
		std::cout << "2. Choose output file name" << std::endl << output_file_name << std::endl;
		std::cout << "3. Set the brightness value" << std::endl << brightness << std::endl;
		std::cout << "4. Save changes" << std::endl;
		std::cout << "5. Exit" << std::endl;
		std::cout << "Your input ";
		std::cin >> input_value;
		switch (input_value) {
		case 1:
			input_file_name = GetFileName();
			file_information = ReadFromFile(input_file_name);
			break;
		case 2:
			output_file_name = GetFileName();
			break;
		case 3:
			std::cout << "Your input of brightness value ";
			std::cin >> brightness;
			if (brightness < 0) brightness = 0;
			else if (brightness > 255) brightness = 255;
			ApplyBrightness(file_information, brightness);
			break;
		case 4:
			WriteToFile(output_file_name, file_information);
			break;
		case 5:
			return 0;
		}
		system("cls");
	}
	return 0;
}
