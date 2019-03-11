#include "context_high_score_subwindow.hpp"
#include "../auxiliary.hpp"

ContextHighScoreSubWindow::ContextHighScoreSubWindow(ParentContext *game, Assets *assets, AudioMixer *mixer, SaveData *save_data,
        unsigned int screen_width, unsigned int screen_height, const Level* level, int x, int y, int player_index, int score) :
        Context(game, assets, mixer, save_data, screen_width, screen_height, "High Score SubWindow Context"), x(x), y(y),
        player_index(player_index) {
    characters = "ABCDEFGHIJKLMNOPQRSTUVWXYZ_\b\n";
    SetBorderSprites(player_index);
    SetCharacterPositions();

    auto player_number_src_rects = assets->GetFontSrcRect("PLAYER " + std::to_string(player_index + 1));
    player_number_text = Auxiliary::getFontRects(player_number_src_rects, this->x + 16, this->y + 96, 1, 1);
    auto score_src_rects = assets->GetFontSrcRect("SCORE: " + std::to_string(score));
    score_text = Auxiliary::getFontRects(score_src_rects, this->x + 16, this->y + 107, 1, 1);

    SetCursor(0);
    SetPlayerName();
}

void ContextHighScoreSubWindow::Update(double delta_time, std::vector<Input> controller_inputs) {
    if (!running)
        return;
    Input *input = &controller_inputs[player_index];
    if (input->up_pressed)
        SetCursor(keyboard_index - 10);
    else if (input->down_pressed)
        SetCursor(keyboard_index + 10);
    else if (input->left_pressed)
        SetCursor(keyboard_index - 1);
    else if (input->right_pressed)
        SetCursor(keyboard_index + 1);
    else if (input->accept_pressed) {
        char c = characters[keyboard_index];
        if (c == '_')
            c = ' ';
        if (c == '\n') {
            running = false;
            parent->EndContext(this);
            return;
        }
        else if (c == '\b') {
            if (!name.empty())
                name.pop_back();
        }
        else if (name.size() < 8)
            name += c;
        SetPlayerName();
    }
}

void ContextHighScoreSubWindow::Render(Renderer *renderer) {
    for (auto &segment : snake_border)
        renderer->Render(segment.first, &segment.second);

    for (auto &letter : name_text)
        renderer->Render(assets->font, &letter.first, &letter.second);
    for (auto &letter : player_number_text)
        renderer->Render(assets->font, &letter.first, &letter.second);
    for (auto &letter : score_text)
        renderer->Render(assets->font, &letter.first, &letter.second);

    for (auto &letter : keyboard)
        renderer->Render(assets->font, &letter.first, &letter.second);

    if (running)
        renderer->Render(assets->ui_keyboard_cursor, &cursor_rect);
}

void ContextHighScoreSubWindow::SetCharacterPositions() {
    keyboard = {};
    int x = this->x + 16 + 2;
    int y = this->y + 16;
    int scale;
    char c;
    Rect *src_rect;
    auto keyboard_src_rects = assets->GetFontSrcRect(characters);
    for (int i = 0; i < characters.size(); ++i) {
        c = characters[i];
        src_rect = &keyboard_src_rects[i];
        if (c == '\b' || c == '\n')
            scale = 1;
        else
            scale = 2;
        Rect dest;
        dest.x = x + (i % 10) * 22;
        dest.y = y + (i / 10) * 25;
        dest.w = src_rect->w * scale;
        dest.h = src_rect->h * scale;
        keyboard.emplace_back(*src_rect, dest);
    }
}

void ContextHighScoreSubWindow::SetBorderSprites(int winner_index) {
    snake_border = {};

    auto snake = assets->snakes[winner_index];
    int width = 14;
    int height_top = 5;
    int height_bottom = 2;
    int x = this->x;
    int y = this->y;
    int w = screen_width;
    int h = screen_height;
    int sprite_w = snake->body_turn->w;
    int sprite_h = snake->body_turn->h;
    Rect top_left = {x, y, sprite_w, sprite_h};
    Rect top_right = {x + sprite_w * width, y, sprite_w, sprite_h};
    Rect centre_left = {x, y + height_top * sprite_h, sprite_w, sprite_h};
    Rect centre_right = {x + sprite_w * width, y + height_top * sprite_h, sprite_w, sprite_h};
    Rect bottom_left = {x, y + (height_top + height_bottom) * sprite_h, sprite_w, sprite_h};
    Rect bottom_right = {x + sprite_w * width, y + (height_top + height_bottom) * sprite_h, sprite_w, sprite_h};

    snake_border.emplace_back(snake->body_turn_right_bottom, top_left);
    snake_border.emplace_back(snake->body_turn_bottom_left, top_right);
    snake_border.emplace_back(snake->body_turn_top_right, centre_left);
    snake_border.emplace_back(snake->body_turn_bottom_left, centre_right);
    snake_border.emplace_back(snake->body_turn_top_right, bottom_left);
    snake_border.emplace_back(snake->body_turn, bottom_right);
    for (int i = 0; i < width - 1; ++i) {
        // Top
        snake_border.push_back({snake->body_straight, {top_left.x + top_left.w + sprite_w * i, top_left.y, sprite_w, sprite_h}});
        // Centre
        snake_border.push_back({snake->body_straight, {centre_left.x + centre_left.w + sprite_w * i, centre_left.y, sprite_w, sprite_h}});
        // Bottom
        snake_border.push_back({snake->body_straight, {bottom_left.x + bottom_left.w + sprite_w * i, bottom_left.y, sprite_w, sprite_h}});
    }
    Texture *right_tex;
    for (int i = 1; i <= height_top - 1; ++i) {
        // Left
        snake_border.push_back({snake->body_straight_vertical, {top_left.x, top_left.y + sprite_h * i, sprite_w, sprite_h}});
        // Right
        if (i == 4)
            right_tex = snake->head_down;
        else
            right_tex = snake->body_straight_vertical;
        snake_border.push_back({right_tex, {top_right.x, top_right.y + sprite_h * i, sprite_w, sprite_h}});
    }
    Texture *left_tex;
    for (int i = 1; i <= height_bottom - 1; ++i) {
        // Left
        if (i == 1)
            left_tex = snake->tail_straight_down;
        else
            left_tex = snake->body_straight_vertical;
        snake_border.push_back({left_tex, {centre_left.x, centre_left.y + sprite_h * i, sprite_w, sprite_h}});
        // Right
        snake_border.push_back({snake->body_straight_vertical, {centre_right.x, centre_right.y + sprite_h * i, sprite_w, sprite_h}});
    }
}

void ContextHighScoreSubWindow::SetCursor(int new_keyboard_index) {
    keyboard_index = std::min((int)characters.size() - 1, std::max(0, new_keyboard_index));
    auto current_letter = &keyboard[keyboard_index].second;
    cursor_rect.x = current_letter->x + current_letter->w / 2 - assets->ui_keyboard_cursor->w / 2;
    cursor_rect.y = current_letter->y - 5;
    cursor_rect.w = assets->ui_keyboard_cursor->w;
    cursor_rect.h = assets->ui_keyboard_cursor->h;
}

void ContextHighScoreSubWindow::SetPlayerName() {
    auto player_name_src_rects = assets->GetFontSrcRect("Name: " + name);
    name_text = Auxiliary::getFontRects(player_name_src_rects, this->x + 75, this->y + 99, 2, 2);
}

bool ContextHighScoreSubWindow::IsComplete() {
    return !running;
}

std::string ContextHighScoreSubWindow::PlayerName() {
    return name;
}
