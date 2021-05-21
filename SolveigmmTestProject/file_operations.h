#pragma once
#include <Windows.h>
#include <atlstr.h>

#include <string>
#include <fstream>

#include "bitmap_informations.h"

constexpr int kBmpCode = 19778;

std::string GetFileName() {
	// Open file
	wchar_t file_size[1024];
	OPENFILENAME open_file_name;
	ZeroMemory(&open_file_name, sizeof(open_file_name));
	open_file_name.lStructSize = sizeof(open_file_name);
	open_file_name.hwndOwner = NULL;
	open_file_name.lpstrFile = file_size;
	open_file_name.lpstrFile[0] = '\0';
	open_file_name.nMaxFile = sizeof(file_size);
	open_file_name.lpstrFilter = L"All\0*.*\0Image\0*.BMP\0";
	open_file_name.nFilterIndex = 1;
	open_file_name.lpstrFileTitle = NULL;
	open_file_name.nMaxFileTitle = 0;
	open_file_name.lpstrInitialDir = NULL;
	open_file_name.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	GetOpenFileName(&open_file_name);
	std::wstring w_file_name = open_file_name.lpstrFile;
	return std::string(begin(w_file_name), end(w_file_name));
}


BMPFormat ReadFromFile(const std::string& file_name) {
	std::ifstream input_file_stream(file_name, std::ios_base::binary);
	BMPFormat file_information;

	input_file_stream.read(reinterpret_cast<char*>(&file_information.bitmap.bfType), sizeof(file_information.bitmap.bfType));

	// It's not bmp file
	if (file_information.bitmap.bfType != kBmpCode) {
		return BMPFormat();
	}

	input_file_stream.read(reinterpret_cast<char*>(&file_information.bitmap.bfSize), sizeof(file_information.bitmap.bfSize));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.bitmap.bfReserved1), sizeof(file_information.bitmap.bfReserved1));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.bitmap.bfReserved2), sizeof(file_information.bitmap.bfReserved2));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.bitmap.bfOffBits), sizeof(file_information.bitmap.bfOffBits));

	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5Size), sizeof(file_information.info.bV5Size));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5Width), sizeof(file_information.info.bV5Width));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5Height), sizeof(file_information.info.bV5Height));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5Planes), sizeof(file_information.info.bV5Planes));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5BitCount), sizeof(file_information.info.bV5BitCount));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5Compression), sizeof(file_information.info.bV5Compression));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5SizeImage), sizeof(file_information.info.bV5SizeImage));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5XPelsPerMeter), sizeof(file_information.info.bV5XPelsPerMeter));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5YPelsPerMeter), sizeof(file_information.info.bV5YPelsPerMeter));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5ClrUsed), sizeof(file_information.info.bV5ClrUsed));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5ClrImportant), sizeof(file_information.info.bV5ClrImportant));

	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5RedMask), sizeof(file_information.info.bV5RedMask));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5GreenMask), sizeof(file_information.info.bV5GreenMask));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5BlueMask), sizeof(file_information.info.bV5BlueMask));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5AlphaMask), sizeof(file_information.info.bV5AlphaMask));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5CSType), sizeof(file_information.info.bV5CSType));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5Endpoints), sizeof(file_information.info.bV5Endpoints));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5GammaRed), sizeof(file_information.info.bV5GammaRed));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5GammaGreen), sizeof(file_information.info.bV5GammaGreen));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5GammaBlue), sizeof(file_information.info.bV5GammaBlue));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5Intent), sizeof(file_information.info.bV5Intent));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5ProfileData), sizeof(file_information.info.bV5ProfileData));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5ProfileSize), sizeof(file_information.info.bV5ProfileSize));
	input_file_stream.read(reinterpret_cast<char*>(&file_information.info.bV5Reserved), sizeof(file_information.info.bV5Reserved));

	unsigned int bufer;

	for (int i = 0; i < file_information.info.bV5Height; i++) {
		file_information.rgbInfo.push_back(std::vector<RGBFormat>());
		for (int j = 0; j < file_information.info.bV5Width; ++j) {
			file_information.rgbInfo[i].push_back(RGBFormat());
		}
	}

	unsigned char* byte;
	for (int i = 0; i < file_information.info.bV5Height; i++) {
		for (int j = 0; j < file_information.info.bV5Width; j++) {
			input_file_stream.read(reinterpret_cast<char*>(&bufer), file_information.info.bV5BitCount / 8);

			byte = (unsigned char*)&bufer;
			file_information.rgbInfo[i][j].rgbRed = *byte;
			++byte;
			file_information.rgbInfo[i][j].rgbGreen = *byte;
			++byte;
			file_information.rgbInfo[i][j].rgbBlue = *byte;
		}
	}

	return file_information;
}

void WriteToFile(const std::string& file_name, BMPFormat& file_information) {
	std::ofstream output_file_stream(file_name, std::ios_base::binary);

	if (file_information.bitmap.bfType != kBmpCode) {
		return;
	}

	output_file_stream.write(reinterpret_cast<char*>(&file_information.bitmap.bfType), sizeof(file_information.bitmap.bfType));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.bitmap.bfSize), sizeof(file_information.bitmap.bfSize));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.bitmap.bfReserved1), sizeof(file_information.bitmap.bfReserved1));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.bitmap.bfReserved2), sizeof(file_information.bitmap.bfReserved2));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.bitmap.bfOffBits), sizeof(file_information.bitmap.bfOffBits));

	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5Size), sizeof(file_information.info.bV5Size));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5Width), sizeof(file_information.info.bV5Width));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5Height), sizeof(file_information.info.bV5Height));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5Planes), sizeof(file_information.info.bV5Planes));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5BitCount), sizeof(file_information.info.bV5BitCount));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5Compression), sizeof(file_information.info.bV5Compression));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5SizeImage), sizeof(file_information.info.bV5SizeImage));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5XPelsPerMeter), sizeof(file_information.info.bV5XPelsPerMeter));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5YPelsPerMeter), sizeof(file_information.info.bV5YPelsPerMeter));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5ClrUsed), sizeof(file_information.info.bV5ClrUsed));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5ClrImportant), sizeof(file_information.info.bV5ClrImportant));

	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5RedMask), sizeof(file_information.info.bV5RedMask));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5GreenMask), sizeof(file_information.info.bV5GreenMask));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5BlueMask), sizeof(file_information.info.bV5BlueMask));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5AlphaMask), sizeof(file_information.info.bV5AlphaMask));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5CSType), sizeof(file_information.info.bV5CSType));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5Endpoints), sizeof(file_information.info.bV5Endpoints));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5GammaRed), sizeof(file_information.info.bV5GammaRed));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5GammaGreen), sizeof(file_information.info.bV5GammaGreen));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5GammaBlue), sizeof(file_information.info.bV5GammaBlue));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5Intent), sizeof(file_information.info.bV5Intent));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5ProfileData), sizeof(file_information.info.bV5ProfileData));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5ProfileSize), sizeof(file_information.info.bV5ProfileSize));
	output_file_stream.write(reinterpret_cast<char*>(&file_information.info.bV5Reserved), sizeof(file_information.info.bV5Reserved));

	for (int i = 0; i < file_information.info.bV5Height; i++) {
		for (int j = 0; j < file_information.info.bV5Width; j++) {
			output_file_stream.write(reinterpret_cast<char*>(&file_information.rgbInfo[i][j].rgbRed), sizeof(BYTE));
			output_file_stream.write(reinterpret_cast<char*>(&file_information.rgbInfo[i][j].rgbGreen), sizeof(BYTE));
			output_file_stream.write(reinterpret_cast<char*>(&file_information.rgbInfo[i][j].rgbBlue), sizeof(BYTE));
		}
	}
}