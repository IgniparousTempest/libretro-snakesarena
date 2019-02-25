#ifndef SNAKESARENA_LIBRETRO_SAVE_DATA_HPP
#define SNAKESARENA_LIBRETRO_SAVE_DATA_HPP


#include <string>

struct SaveFile {
    int32_t previous_arena_index = 0;
    uint32_t previous_number_players = 1;
};

class SaveData {
public:
    SaveFile data;

    explicit SaveData(std::string config_folder_path);

    void Serialise();

    void Deserialise();

private:
    std::string file_path = "snakes_arena_save_data.dat";
};


#endif //SNAKESARENA_LIBRETRO_SAVE_DATA_HPP
