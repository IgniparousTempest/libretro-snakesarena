#include "context_menu.hpp"
#include "context_main.hpp"

ContextMenu::ContextMenu(ParentContext *game, AudioMixer *mixer, SaveData *save_data, unsigned int screen_width,
        unsigned int screen_height, Assets *assets, std::vector<Level> *levels, unsigned int max_players) :
        Context(game, assets, mixer, save_data, screen_width, screen_height, "Main Menu"), levels(levels),
        max_players(max_players) {
    selected_level = save_data->data.previous_arena_index;
    num_players = save_data->data.previous_number_players;
    title_dest = {7, 2, assets->ui_title->w, assets->ui_title->h};
    start_button_dest = {219, 211, assets->ui_start_button->w, assets->ui_start_button->h};
    arrow_left_dest = new Rect(0, 0, assets->ui_arrow_left->w, assets->ui_arrow_left->h);
    arrow_right_dest = new Rect(0, 0, assets->ui_arrow_right->w, assets->ui_arrow_right->h);
    UpdatePlayersText();
    GenerateCredits();
    UpdateArrows();
    UpdateLevelPreview();
}

void ContextMenu::Update(double delta_time, std::vector<Input> controller_inputs) {
    for (auto &controller_input : controller_inputs) {
        if (controller_input.up_pressed) {
            if (--controls_index < 0)
                controls_index = 0;
            UpdateArrows();
        }
        else if (controller_input.down_pressed) {
            if (++controls_index >= 3)
                controls_index = 2;
            UpdateArrows();
        }
        else if (controls_index == 0 && controller_input.left_pressed) {
            if (--num_players <= 0)
                num_players = 1;
            UpdatePlayersText();
        }
        else if (controls_index == 0 && controller_input.right_pressed) {
            if (++num_players > max_players)
                num_players = max_players;
            UpdatePlayersText();
        }
        else if (controls_index == 1 && controller_input.left_pressed) {
            if (--selected_level < 0)
                selected_level = static_cast<int>(levels->size() - 1);
            UpdateLevelPreview();
        }
        else if (controls_index == 1 && controller_input.right_pressed) {
            if (++selected_level >= levels->size())
                selected_level = 0;
            UpdateLevelPreview();
        }
        else if (controls_index == 2 && controller_input.accept_pressed) {
            save_data->data.previous_arena_index = selected_level;
            save_data->data.previous_number_players = num_players;
            save_data->Serialise();
            parent->PushNewContext(new ContextMain(parent, mixer, save_data, screen_width, screen_height, assets, &(*levels)[selected_level], num_players));
        }
    }
}

void ContextMenu::Render(Renderer *renderer) {
    renderer->Clear(Renderer::rgb(0, 0, 0));
    renderer->Render(assets->ui_title, &title_dest);

    renderer->Render(preview_image, &preview_dest);
    for (auto &rect : level_text)
        renderer->Render(assets->font, &rect.first, &rect.second);
    for (auto &t : high_score_text)
        for (auto &rect : t)
            renderer->Render(assets->font, &rect.first, &rect.second);

    for (auto &rect : players_text)
        renderer->Render(assets->font, &rect.first, &rect.second);

    for (auto &t : credits_text)
        for (auto &rect : t)
            renderer->Render(assets->font, &rect.first, &rect.second);

    renderer->Render(assets->ui_start_button, &start_button_dest);

    renderer->Render(assets->ui_arrow_left, arrow_left_dest);
    renderer->Render(assets->ui_arrow_right, arrow_right_dest);
}

void ContextMenu::OnResume() {
    UpdateLevelPreview();
}

void ContextMenu::UpdatePlayersText() {
    auto players_src_rects = assets->GetFontSrcRect("Players: " + std::to_string(num_players));
    players_text = Auxiliary::getFontRects(players_src_rects, 103, 109, 2, 2);
}

void ContextMenu::UpdateLevelPreview() {
    Level *level = &(*levels)[selected_level];

    // Get level preview
    preview_image = level->PreviewImage();
    preview_dest = {86, 132, preview_image->w, preview_image->h};

    // Get level name
    auto src_rects = assets->GetFontSrcRect(level->level_name + ":");
    level_text = Auxiliary::getFontRects(src_rects, screen_width / 2 + 20, preview_dest.y, 2, 2);

    // Get high scores
    high_score_text = {};
    int y_offset = preview_dest.y + level_text[0].second.h + 10;
    text_t number_text, name_text, score_text;
    auto high_score = save_data->GetHighScore(level);
    std::string name, score;
    for (int i = 0; i < 3; ++i) {
        if (high_score != nullptr && i < high_score->high_score_names.size()) {
            name = high_score->high_score_names[i];
            score = std::to_string(high_score->high_scores[i]);
        } else {
            name = "...";
            score = "0";
        }
        src_rects = assets->GetFontSrcRect("No " + std::to_string(i + 1) + ".");
        number_text = Auxiliary::getFontRects(src_rects, screen_width / 2 + 20, y_offset + (level_text[0].second.h + 4) * i, 2, 2);
        src_rects = assets->GetFontSrcRect(name);
        name_text = Auxiliary::getFontRects(src_rects, screen_width / 2 + 80, y_offset + (level_text[0].second.h + 4) * i, 2, 2);
        src_rects = assets->GetFontSrcRect(score);
        score_text = Auxiliary::getFontRects(src_rects, screen_width / 2 + 180, y_offset + (level_text[0].second.h + 4) * i, 2, 2);
        text_t high_score_line;
        high_score_line.reserve(number_text.size() + name_text.size() + score_text.size());
        high_score_line.insert(high_score_line.end(), number_text.begin(), number_text.end());
        high_score_line.insert(high_score_line.end(), name_text.begin(), name_text.end());
        high_score_line.insert(high_score_line.end(), score_text.begin(), score_text.end());
        high_score_text.push_back(high_score_line);
    }
}

void ContextMenu::GenerateCredits() {
    credits_text = {};
    int y, x;

    auto developer_src_rects = assets->GetFontSrcRect("Game by Courtney Pitcher (github.com/IgniparousTempest)");
    auto developer_text = Auxiliary::getFontRects(developer_src_rects, 0, screen_height - 21, 1, 1);
    x = (screen_width - developer_text.back().second.x + developer_text.back().second.w) / 2;
    for (auto &character : developer_text)
        character.second.x +=x;
    credits_text.push_back(developer_text);

    y = developer_text.front().second.y + developer_text.front().second.h + 1;
    auto concept_src_rects = assets->GetFontSrcRect("Concept by Robyn Wimberley");
    auto concept_text = Auxiliary::getFontRects(concept_src_rects, 0, y, 1, 1);
    x = (screen_width - concept_text.back().second.x + concept_text.back().second.w) / 2;
    for (auto &character : concept_text)
        character.second.x +=x;
    credits_text.push_back(concept_text);

    y = concept_text.front().second.y + concept_text.front().second.h + 1;
    auto music_src_rects = assets->GetFontSrcRect("Music by Eric Matyas (www.soundimage.org)");
    auto music_text = Auxiliary::getFontRects(music_src_rects, 0, y, 1, 1);
    x = (screen_width - music_text.back().second.x + music_text.back().second.w) / 2;
    for (auto &character : music_text)
        character.second.x +=x;
    credits_text.push_back(music_text);
}

void ContextMenu::UpdateArrows() {
    switch (controls_index) {
        case 0:
            arrow_left_dest->x = players_text.front().second.x - arrow_left_dest->w - 4;
            arrow_left_dest->y = players_text.front().second.y;
            arrow_right_dest->x = players_text.back().second.x + players_text.back().second.w + 4;
            arrow_right_dest->y = arrow_left_dest->y;
            break;
        case 1:
            arrow_left_dest->x = preview_dest.x - arrow_left_dest->w - 4;
            arrow_left_dest->y = preview_dest.y + preview_dest.h / 2 - arrow_left_dest->h / 2;
            arrow_right_dest->x = preview_dest.x + preview_dest.w + 4;
            arrow_right_dest->y = arrow_left_dest->y;
            break;
        case 2:
            arrow_left_dest->x = start_button_dest.x + start_button_dest.w + 4;
            arrow_left_dest->y = start_button_dest.y + start_button_dest.h / 2 - arrow_left_dest->h / 2;
            arrow_right_dest->x = start_button_dest.x - arrow_right_dest->w - 4;
            arrow_right_dest->y = arrow_left_dest->y;
            break;
        default:
            return;
    }
}
