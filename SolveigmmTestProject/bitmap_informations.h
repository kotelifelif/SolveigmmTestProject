#pragma once
#include <Windows.h>

#include <vector>

struct RGBFormat {
	int rgbRed;
	int rgbGreen;
	int rgbBlue;
};

struct BMPFormat {
	BITMAPFILEHEADER bitmap;
	BITMAPV5HEADER info;
	std::vector<std::vector<RGBFormat>> rgbInfo;
};

int ChangeChannel(int value, int brightness) {
	int result_value;
	if (value + brightness < 0) result_value = 0;
	else if (value + brightness > 255) result_value = 255;
	else result_value = value + brightness;
	return result_value;
}

void ApplyBrightness(BMPFormat& file_information, const int brightness) {
	for (unsigned int i = 0; i < file_information.info.bV5Height; i++) {
		for (unsigned int j = 0; j < file_information.info.bV5Width; j++) {
			file_information.rgbInfo[i][j].rgbRed = ChangeChannel(file_information.rgbInfo[i][j].rgbRed, brightness);
			file_information.rgbInfo[i][j].rgbGreen = ChangeChannel(file_information.rgbInfo[i][j].rgbGreen, brightness);
			file_information.rgbInfo[i][j].rgbBlue = ChangeChannel(file_information.rgbInfo[i][j].rgbBlue, brightness);
		}
	}
}