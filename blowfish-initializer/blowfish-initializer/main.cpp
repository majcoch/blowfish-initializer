#include <fstream>
#include <iostream>
#include <stdio.h>
#include <string>
#include <time.h>

#include "blowfish.h"
#include "blowfish_data_init.h"

const std::string currentDateTime() {
	time_t now = time(0);
	struct tm tstruct;
	localtime_s(&tstruct, &now);

	char buf[80];
	strftime(buf, sizeof(buf), "%Y.%m.%d %X", &tstruct);

	return buf;
}

int main(int argc, char* argv[]) {

	if (argc == 2) {
		uint64_t complete_key = strtoull(argv[1], NULL, 16);
		if (0 != complete_key) {
			uint32_t blowfish[] = { (uint32_t)(complete_key >> 32), (uint32_t)(complete_key) };
			blowfish_init(blowfish, 2);

			// Generate arrays file
			std::ofstream data_file("blowfish_data.c");
			if (data_file.good()) {
				std::cout << "blowfish_data.c file created. Initializaing arrays..." << std::endl;

				// Print file header
				data_file << "/*\n * blowfish_data.c\n *\n * Created: " << currentDateTime() << "\n * Author : Micha³ Granda\n */ " << std::endl;

				// Print include
				data_file << "#include \"blowfish_data.h\"" << std::endl << std::endl;

				// print first array
				data_file << "MODIFY_ARR uint32_t blowfish_p_box[18] FLASH_MEM = {" << std::endl;
				for (size_t i = 0; i < 3; i++) {
					data_file << "\t";
					for (size_t j = 0; j < 6; j++) {
						data_file << "0x" << std::hex << blowfish_p_box[(i * 6) + j] << ",";
					}
					data_file << std::endl;
				}
				data_file << "};" << std::endl << std::endl;

				// print second array
				data_file << "MODIFY_ARR uint32_t blowfish_s_box[4][256] FLASH_MEM = {" << std::endl;
				for (size_t i = 0; i < 4; i++) {
					data_file << "\t//S" << i << std::endl;
					data_file << "\t{" << std::endl << "\t\t";
					for (size_t j = 0; j < 256; j++) {
						if (j % 6 == 0 && j != 0) data_file << ::std::endl << "\t\t";
						data_file << "0x" << std::hex << blowfish_s_box[i][j] << ",";
					}

					data_file << std::endl << "\t}," << std::endl << std::endl;
				}
				data_file << "};" << std::endl;

				data_file.close();
				std::cout << "Initialization completed. Exiting now!" << std::endl;
			}
			else {
				std::cout << "Unable to create blowfish_data.c file. Exiting now!" << std::endl;
			}
		}
		else {
			std::cout << "Invalid cryptographic key provided. Please check key and try again!" << std::endl;
		}
	}
	else {
		std::cout << "No cryptographic key provided. Please provide key and try again" << std::endl;
	}
	return 0;
}
