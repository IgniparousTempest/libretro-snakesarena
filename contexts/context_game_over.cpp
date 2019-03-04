#include <iterator>
#include <utility>
#include "context_game_over.hpp"
#include "context_main.hpp"

ContextGameOver::ContextGameOver(GameManager *game, AudioMixer *mixer, SaveData *save_data, Assets *assets,
        unsigned int screen_width, unsigned int screen_height, const Level* level, std::vector<int> scores, std::vector<int> wins) :
        Context(game, assets, mixer, save_data, screen_width, screen_height, "Game Over Context"),
        scores(std::move(scores)), wins(std::move(wins)), level(level) {
    assert(this->scores.size() == this->wins.size());
    rect_winner = {((int)screen_width - assets->ui_winner->w) / 2, 50, assets->ui_winner->w, assets->ui_winner->h};
    rect_btn_play_again = {((int)screen_width - assets->ui_button_play_again->w) / 2, rect_winner.y + rect_winner.h + 20, assets->ui_button_play_again->w, assets->ui_button_play_again->h};
    rect_btn_main_menu = {((int)screen_width - assets->ui_button_main_menu->w) / 2, rect_btn_play_again.y + rect_btn_play_again.h + 20, assets->ui_button_main_menu->w, assets->ui_button_main_menu->h};
    rect_cursor_left = {0, 0, assets->ui_arrow_left->w, assets->ui_arrow_left->h};
    rect_cursor_right = {0, 0, assets->ui_arrow_right->w, assets->ui_arrow_right->h};
    UpdateCursor();

    long long winner_index = std::distance(this->scores.begin(), std::max_element(this->scores.begin(), this->scores.end()));
    SetWinningSnake(winner_index);
}

void ContextGameOver::Update(double delta_time, std::vector<Input> controller_inputs) {
    for (auto &input : controller_inputs) {
        if (input.up_pressed) {
            if (--menu_index < 0)
                menu_index = 0;
            UpdateCursor();
        }
        else if (input.down_pressed) {
            if (++menu_index >= 2)
                menu_index = 1;
            UpdateCursor();
        }
        else if (input.accept_pressed) {
            switch (menu_index) {
                case 0:
                    game_manager->PushNewContext(new ContextMain(game_manager, mixer, save_data, screen_width, screen_height, assets, level, (int) scores.size(), &wins));
                    game_manager->EndContext(this);
                    return;
                default:
                    game_manager->EndContext(this);
                    return;
            }
        }
    }
}

void ContextGameOver::Render(Renderer *renderer) {
    if (old_frame_buffer != nullptr)
        std::copy(old_frame_buffer, old_frame_buffer + renderer->width * renderer->height, renderer->framebuffer);
    else {
        old_frame_buffer = new uint32_t[renderer->width * renderer->height];
        std::copy(renderer->framebuffer, renderer->framebuffer + renderer->width * renderer->height, old_frame_buffer);
    }
    renderer->Render(assets->ui_winner, &rect_winner);
    renderer->Render(assets->ui_button_play_again, &rect_btn_play_again);
    renderer->Render(assets->ui_button_main_menu, &rect_btn_main_menu);
    renderer->Render(assets->ui_arrow_left, &rect_cursor_left);
    renderer->Render(assets->ui_arrow_right, &rect_cursor_right);

    for (auto &segment : winning_snake) {
        renderer->Render(segment.first, &segment.second);
    }
}

void ContextGameOver::UpdateCursor() {
    Rect *button_rect;
    switch (menu_index) {
        case 0:
            button_rect = &rect_btn_play_again;
            break;
        default:
            button_rect = &rect_btn_main_menu;
            break;
    }

    rect_cursor_right.x = button_rect->x - rect_cursor_right.w - 4;
    rect_cursor_right.y = button_rect->y + button_rect->h / 2 - rect_cursor_right.h / 2;
    rect_cursor_left.x = button_rect->x + button_rect->w + 4;
    rect_cursor_left.y = rect_cursor_right.y;
}

void ContextGameOver::SetWinningSnake(long long winner_index) {
    winning_snake = {};

    auto snake = assets->snakes[winner_index];
    int x = rect_winner.x;
    int y = rect_winner.y;
    int w = rect_winner.w;
    int h = rect_winner.h;
    Rect top_left = {x + 3, y + 3, snake->body_turn->w, snake->body_turn->h};
    Rect top_right = {x + w - snake->body_turn->w - 3, y + 3, snake->body_turn->w, snake->body_turn->h};
    Rect bottom_left = {x + 3, y + h - snake->body_turn->h - 5, snake->body_turn->w, snake->body_turn->h};
    Rect bottom_right = {x + w - snake->body_turn->w - 3, y + h - snake->body_turn->h - 5, snake->body_turn->w, snake->body_turn->h};

    winning_snake.emplace_back(snake->body_turn_right_bottom, top_left);
    winning_snake.emplace_back(snake->body_turn_bottom_left, top_right);
    winning_snake.emplace_back(snake->body_turn_top_right, bottom_left);
    winning_snake.emplace_back(snake->body_turn, bottom_right);
    Texture *top_tex;
    for (int i = 0; i < 9; ++i) {
        // Top
        if (i == 4)
            top_tex = snake->head;
        else if (i == 5)
            top_tex = snake->tail_straight;
        else
            top_tex = snake->body_straight;
        winning_snake.push_back({top_tex, {top_left.x + top_left.w + top_tex->w * i, top_left.y, top_tex->w, top_tex->h}});
        // Bottom
        winning_snake.push_back({snake->body_straight, {bottom_left.x + bottom_left.w + snake->body_straight->w * i, bottom_left.y, snake->body_straight->w, snake->body_straight->h}});
    }
    for (int i = 1; i <= 2; ++i) {
        // Left
        winning_snake.push_back({snake->body_straight_vertical, {top_left.x, top_left.y + snake->body_straight_vertical->h * i, snake->body_straight_vertical->w, snake->body_straight_vertical->h}});
        // Right
        winning_snake.push_back({snake->body_straight_vertical, {top_right.x, top_right.y + snake->body_straight_vertical->h * i, snake->body_straight_vertical->w, snake->body_straight_vertical->h}});
    }
}
