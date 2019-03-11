#include "context_main.hpp"
#include "context_game_over.hpp"
#include "context_high_score_entry.hpp"

ContextMain::ContextMain(ParentContext *game, AudioMixer *mixer, SaveData *save_data, unsigned int screen_width,
        unsigned int screen_height, Assets *assets, const Level* level, unsigned int players, std::vector<int> *wins) :
        Context(game, assets, mixer, save_data, screen_width, screen_height, "Main Game Context"), level(level) {
    int square_size = 16;
    int header_height = 14;
    int field_width = screen_width / square_size;
    int field_height = (screen_height - header_height) / square_size;
    field = new Field(square_size, field_width, field_height, header_height, assets, mixer, level, players, parent->GetRenderer());
    field_header = new FieldHeader(assets, screen_width, header_height, players, wins);
    if (wins != nullptr)
        for (int i = 0; i < wins->size(); ++i) {
            field->players[i].wins = (*wins)[i];
        }

    std::mt19937 rng;
    rng.seed(std::random_device()());
    std::uniform_int_distribution<std::mt19937::result_type> dist(0, static_cast<unsigned int>(assets->snd_background.size() - 1));
    mixer->Clear();
    mixer->Play(assets->snd_background[dist(rng)]);
}

void ContextMain::Update(double delta_time, std::vector<Input> controller_inputs) {
    // Update Field
    field->Update(delta_time, controller_inputs, field_header);
    field_header->Update(delta_time, controller_inputs);

    // Game is over
    if (field->AllSnakesDead()) {
        typedef std::tuple<int, int, int> score_wins_t;
        std::vector<score_wins_t> score_wins_pair;
        for (int i = 0; i < field->players.size(); ++i)
            score_wins_pair.emplace_back(field->players[i].score, field->players[i].wins, i);

        sort(score_wins_pair.begin(), score_wins_pair.end(),
             [](const score_wins_t & a, const score_wins_t & b) -> bool
             {
                 return std::get<0>(a) > std::get<0>(b);
             });

        auto highest_score = std::get<0>(score_wins_pair.front());
        for (auto &pair : score_wins_pair)
            if (std::get<0>(pair) == highest_score)
                std::get<1>(pair)++;

        std::vector<int> scores, wins, indices;
        for (auto &pair : score_wins_pair) {
            scores.push_back(std::get<0>(pair));
            wins.push_back(std::get<1>(pair));
            indices.push_back(std::get<2>(pair));
        }
        parent->PushNewContext(new ContextGameOver(parent, mixer, save_data, assets, screen_width, screen_height, field->level, scores, wins, parent->GetRenderer()->framebuffer));
        int length = save_data->CanAddScores(level, scores);
        if (length > 0) {
            indices.erase(indices.begin() + length, indices.end());
            scores.erase(scores.begin() + length, scores.end());
            parent->PushNewContext(new ContextHighScoreEntry(parent, assets, mixer, save_data, screen_width, screen_height, field->level, indices, scores));
        }
        parent->EndContext(this);
        return;
    }
}

void ContextMain::Render(Renderer *renderer) {
    field->PartialRender(renderer);
    field_header->Render(renderer);
}
