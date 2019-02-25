#include <fstream>
#include <iostream>
#include "save_data.hpp"

SaveData::SaveData(std::string config_folder_path) {
    file_path = config_folder_path + file_path;
}

void SaveData::Serialise() {
    std::cout << "Writing save file." << std::endl;
    std::ofstream output_file(file_path, std::ios::binary);
    output_file.write((char*)&data, sizeof(data));
}

void SaveData::Deserialise() {
    std::cout << "Reading save file." << std::endl;
    std::ifstream input_file(file_path, std::ios::binary);
    if (!input_file.fail())
        input_file.read((char*)&data, sizeof(data));
}
