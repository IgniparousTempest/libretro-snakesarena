#ifndef SNAKESARENA_LIBRETRO_SAVE_DATA_HPP
#define SNAKESARENA_LIBRETRO_SAVE_DATA_HPP


#include <string>
#include <vector>
#include "level.hpp"

class SerialisationHelper {
public:
    static void Deserialise(std::ifstream &input_file, std::string &str) {
        char byte;
        for (;;) {
            input_file.read((char*)&byte, sizeof(byte));
            if (byte == '\0')
                break;

            str += byte;
        }
    }

    static void Deserialise(std::ifstream &input_file, int &num) {
        input_file.read((char*)&num, sizeof(num));
    }

    template <class T>
    static void Deserialise(std::ifstream &input_file, std::vector<T> &vec) {
        int32_t length;
        SerialisationHelper::Deserialise(input_file, length);

        for (int i = 0; i < length; ++i) {
            T data;
            SerialisationHelper::Deserialise(input_file, data);
            vec.push_back(data);
        }
    }

    static void Serialise(std::ofstream &output_file, std::string &str) {
        output_file.write((char*)str.c_str(), str.size() + 1);
    }

    static void Serialise(std::ofstream &output_file, int32_t &num) {
        output_file.write((char*)&num, sizeof(num));
    }

    template <class T>
    static void Serialise(std::ofstream &output_file, std::vector<T> &vec) {
        int32_t length = vec.size();
        SerialisationHelper::Serialise(output_file, length);

        for (auto &item : vec)
            Serialise(output_file, item);
    }
};

struct HighScore {
    std::string level_name;
    std::vector<std::string> high_score_names;
    std::vector<int> high_scores;

    void Deserialise(std::ifstream &input_file) {
        level_name = "";
        high_score_names = {};
        high_scores = {};

        SerialisationHelper::Deserialise(input_file, level_name);
        SerialisationHelper::Deserialise(input_file, high_score_names);
        SerialisationHelper::Deserialise(input_file, high_scores);
    }

    void Serialise(std::ofstream &output_file) {
        output_file.write((char*)level_name.c_str(), level_name.size() + 1);
        SerialisationHelper::Serialise(output_file, high_score_names);
        SerialisationHelper::Serialise(output_file, high_scores);
    }
};

struct SaveFile {
    int32_t previous_arena_index = 0;
    uint32_t previous_number_players = 1;
    std::vector<HighScore> high_scores;

    void Deserialise(std::ifstream &input_file) {
        high_scores = {};

        input_file.read((char*)&previous_arena_index, sizeof(previous_arena_index));
        input_file.read((char*)&previous_number_players, sizeof(previous_number_players));
        char byte;
        for (;;) {
            input_file.read((char*)&byte, sizeof(byte));
            if (byte == '\0')
                break;

            input_file.seekg(-1, std::ios_base::cur);
            HighScore score{};
            score.Deserialise(input_file);
            high_scores.push_back(score);
        }
    }

    void Serialise(std::ofstream &output_file) {
        char terminator = '\0';
        output_file.write((char*)&previous_arena_index, sizeof(previous_arena_index));
        output_file.write((char*)&previous_number_players, sizeof(previous_number_players));
        for (auto &high_score : high_scores)
            high_score.Serialise(output_file);
        output_file.write((char*)&terminator, sizeof(terminator));
    }
};

class SaveData {
public:
    /// The maximum number of scores on each level.
    const int MAX_SCORES = 3;
    SaveFile data;

    explicit SaveData(std::string config_folder_path);

    void Serialise();

    void Deserialise();

    void AddHighScore(const Level *level, std::basic_string<char> player_name, int score);

    /// Gets the high score table for a particular level.
    /// \param level The level to get scores for.
    /// \return The high score table.
    HighScore *GetHighScore(const Level *level);

    /// Counts how many of the specified scores can be added to the high score table.
    /// \param level The level the scores belong to.
    /// \param scores The scores to try and add.
    /// \return The number of scores that can be added in order.
    int CanAddScores(const Level *level, std::vector<int> scores);

private:
    std::string file_path = "snakes_arena_save_data.dat";

    void AddHighScore(HighScore *high_score, std::basic_string<char> player_name, int score);
};


#endif //SNAKESARENA_LIBRETRO_SAVE_DATA_HPP
