#include <fstream>
#include <iostream>
#include "save_data.hpp"

SaveData::SaveData(std::string config_folder_path) {
    file_path = config_folder_path + file_path;
}

void SaveData::Serialise() {
    std::cout << "Writing save file." << std::endl;
    std::ofstream output_file(file_path, std::ios::binary);
    data.Serialise(output_file);
}

void SaveData::Deserialise() {
    std::cout << "Reading save file." << std::endl;
    std::ifstream input_file(file_path, std::ios::binary);
    if (!input_file.fail())
        data.Deserialise(input_file);
}

void SaveData::AddHighScore(const Level *level, std::basic_string<char> player_name, int score) {
    // Update existing entry
    auto high_score = GetHighScore(level);
    if (high_score != nullptr) {
        AddHighScore(high_score, player_name, score);
        return;
    }
    // Create new entry
    HighScore high_score_new;
    high_score_new.level_name = level->level_name;
    high_score_new.high_scores = {score};
    high_score_new.high_score_names = {player_name};
    data.high_scores.push_back(high_score_new);
}

void SaveData::AddHighScore(HighScore *high_score, std::basic_string<char> player_name, int score) {
    for (int i = 0; i < high_score->high_scores.size(); ++i) {
        if (score > high_score->high_scores[i]) {
            high_score->high_scores.insert(high_score->high_scores.begin() + i, score);
            high_score->high_score_names.insert(high_score->high_score_names.begin() + i, player_name);
            if (high_score->high_scores.size() > MAX_SCORES) {
                high_score->high_scores.erase(high_score->high_scores.begin() + MAX_SCORES);
                high_score->high_score_names.erase(high_score->high_score_names.begin() + MAX_SCORES);
            }
            return;
        }
    }
    // Add to bottom of list
    if (high_score->high_scores.size() < MAX_SCORES) {
        high_score->high_scores.push_back(score);
        high_score->high_score_names.push_back(player_name);
    }
}

HighScore *SaveData::GetHighScore(const Level *level) {
    for (auto &high_score : data.high_scores) {
        if (high_score.level_name == level->level_name)
            return &high_score;
    }
    return nullptr;
}

int SaveData::CanAddScores(const Level *level, std::vector<int> scores) {
    std::vector<std::pair<int, bool>> scores_new;
    auto h = GetHighScore(level);
    auto high_scores = h == nullptr ? std::vector<int>() : h->high_scores;
    scores_new.reserve(high_scores.size() + scores.size());
    for (int &high_score : high_scores)
        scores_new.emplace_back(high_score, false);
    for (int &score : scores)
        scores_new.emplace_back(score, true);

    sort(scores_new.begin(), scores_new.end(),
         [](const std::pair<int, bool> & a, const std::pair<int, bool> & b) -> bool
         {
            if (a.first == b.first)
                // Secondary sort on whether it was already in the high score table.
                return a.second < b.second;
            else
                // Primary sort on score.
                return a.first > b.first;
         });

    int count = 0;
    for (int i = 0; i < std::min(MAX_SCORES, (int)scores_new.size()); ++i)
        if (scores_new[i].second)
            count++;
    return count;
}
