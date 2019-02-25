#include "field_header.hpp"
#include "auxillary.hpp"

FieldHeader::FieldHeader(Assets *assets, int width, int height, unsigned int num_players) : assets(assets) {
    rect = {0, 0, width, height};
    text = {};

    snake_portraits = {};
    std::vector<int> x_pos_8 = {2, 61, 121, 182, 243, 304, 365, 427};
    assert(num_players <= assets->snakes.size());
    for (int i = 0; i < num_players; ++i)
        snake_portraits.push_back({assets->snakes[i]->portrait, {x_pos_8[i], 1, assets->snakes[i]->portrait->w, assets->snakes[i]->portrait->h}});
    text.resize(num_players);
    for (int i = 0; i < num_players; ++i)
        SetScore(i, 0);
}

void FieldHeader::Update(double delta_time, std::vector<Input> controller_inputs) {

}

void FieldHeader::Render(Renderer *renderer) {
    renderer->RenderSquare(Renderer::rgb(0, 0, 0), &rect);

    for (auto &portrait : snake_portraits)
        renderer->Render(portrait.first, &portrait.second);

    for (auto &t : text)
        for (auto &rect : t)
            renderer->Render(assets->font, &rect.first, &rect.second);
}

void FieldHeader::SetScore(int player_number, int score) {
    int x = snake_portraits[player_number].second.x + snake_portraits[player_number].second.w + 2;

    auto score_src_rects = assets->GetFontSrcRect("Score: " + std::to_string(score));
    auto score_rects = Auxiliary::getFontRects(score_src_rects, x, 0, 1, 1);

    auto wins_src_rects = assets->GetFontSrcRect("Wins: 0");
    auto wins_rects = Auxiliary::getFontRects(wins_src_rects, x, 8, 1, 1);

    text_t new_text;
    new_text.reserve(score_rects.size() + wins_rects.size());
    new_text.insert(new_text.end(), score_rects.begin(), score_rects.end());
    new_text.insert(new_text.end(), wins_rects.begin(), wins_rects.end());
    text[player_number] = new_text;
}
