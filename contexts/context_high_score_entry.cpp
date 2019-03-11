#include "context_high_score_entry.hpp"

ContextHighScoreEntry::ContextHighScoreEntry(ParentContext *game, Assets *assets, AudioMixer *mixer,
        SaveData *save_data, unsigned int screen_width, unsigned int screen_height, const Level *level,
        std::vector<int> player_indices, std::vector<int> scores)
        : Context(game, assets, mixer, save_data, screen_width, screen_height, "High Score Entry"), level(level), scores(scores) {
    int x, y;
    for (int i = 0; i < player_indices.size(); ++i) {
        player_names.emplace_back("");
        x = (i % 2 == 0) ? 0 : screen_width / 2;
        y = (screen_height / 2) * (i / 2);
        if (player_indices.size() == 1) {
            x = screen_width / 4;
            y = screen_height / 4;
        }
        contexts.push_back(new ContextHighScoreSubWindow(this, assets, mixer, save_data, screen_width, screen_height, level, x, y, player_indices[i], scores[i]));
    }
}

void ContextHighScoreEntry::Update(double delta_time, std::vector<Input> controller_inputs) {
    int length = std::min((int)contexts.size(), 4);
    for (int i = 0; i < length; ++i)
        contexts[i]->Update(delta_time, controller_inputs);

    for (auto &context : contexts) {
        if (!context->IsComplete())
            return;
    }
    //All windows complete
    for (int i = 0; i < player_names.size(); ++i)
        if (!player_names[i].empty())
            save_data->AddHighScore(level, player_names[i], scores[i]);
    save_data->Serialise();
    parent->EndContext(this);
}

void ContextHighScoreEntry::Render(Renderer *renderer) {
    renderer->Clear(Renderer::rgb(0, 0, 0));
    int length = std::min((int)contexts.size(), 4);
    for (int i = 0; i < length; ++i)
        contexts[i]->Render(renderer);
}

void ContextHighScoreEntry::PushNewContext(Context *context) {

}

void ContextHighScoreEntry::EndContext(Context *context) {
    auto window = dynamic_cast<ContextHighScoreSubWindow*>(context);
    for (int i = 0; i < contexts.size(); ++i) {
        if (contexts[i] == context) {
            std::cout << "Ending high score sub-window " << i << "." << std::endl;
            player_names[i] = window->PlayerName();
            if (contexts.size() > 4) {
                contexts.back()->x = window->x;
                contexts.back()->y = window->y;
                delete context;
                contexts[i] = contexts.back();
                contexts.erase(contexts.begin() + contexts.size() - 1);
            }
        }
    }
}

Renderer *ContextHighScoreEntry::GetRenderer() {
    return nullptr;
}
