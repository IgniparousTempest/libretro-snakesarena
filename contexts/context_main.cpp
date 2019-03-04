#include "context_main.hpp"
#include "context_game_over.hpp"

ContextMain::ContextMain(GameManager *game, AudioMixer *mixer, SaveData *save_data, unsigned int screen_width,
        unsigned int screen_height, Assets *assets, const Level* level, unsigned int players, std::vector<int> *wins) :
        Context(game, assets, mixer, save_data, screen_width, screen_height, "Main Game Context") {
    int square_size = 16;
    int header_height = 14;
    int field_width = screen_width / square_size;
    int field_height = (screen_height - header_height) / square_size;
    field = new Field(square_size, field_width, field_height, header_height, assets, mixer, level, players);
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
        std::vector<int> scores, wins;
        for (auto &player : field->players) {
            scores.push_back(player.score);
            wins.push_back(player.wins);
        }
        auto highest_score = std::max_element(scores.begin(), scores.end());
        for (int i = 0; i < scores.size(); ++i)
            if (scores[i] == *highest_score)
                wins[i]++;

        game_manager->PushNewContext(new ContextGameOver(game_manager, mixer, save_data, assets, screen_width, screen_height, field->level, scores, wins));
        game_manager->EndContext(this);
        return;
    }
}

void ContextMain::Render(Renderer *renderer) {
    field->PartialRender(renderer);
    field_header->Render(renderer);
}
