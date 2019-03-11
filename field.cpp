#include <utility>

#include "field.hpp"

Field::Field(int square_size, int field_width, int field_height, int y_offset, Assets *assets, AudioMixer *mixer, const Level* level, int num_players, Renderer *renderer) :
        square_size(square_size), field_width(field_width), field_height(field_height), y_offset(y_offset),
        level(level), raspberry(assets->raspberry), snd_chomp(assets->snd_chomp), snd_crash(assets->snd_crash), mixer(mixer) {
    std::cout << "Field will have square of " << square_size << "x" << square_size << " in a matrix of " << field_width << "x" << field_height << std::endl;
    colour_light = Renderer::rgb(170, 215, 81);
    colour_dark = Renderer::rgb(162, 209, 73);
    for (int i = 0; i < num_players; ++i)
        players.emplace_back(square_size, field_width, field_height, level->spawns[i], assets->snakes[i]);
    rng.seed(std::random_device()());

    RenderAll(renderer);
}

void Field::Update(double delta_time, std::vector<Input> controller_inputs, FieldHeader* header) {
    // Update player
    for (int i = 0; i < players.size(); ++i) {
        if (controller_inputs[i].up_pressed)
            players[i].MoveUp();
        if (controller_inputs[i].down_pressed)
            players[i].MoveDown();
        if (controller_inputs[i].left_pressed)
            players[i].MoveLeft();
        if (controller_inputs[i].right_pressed)
            players[i].MoveRight();
        if (controller_inputs[i].accept_pressed)
            players[i].GrowBody();
        players[i].Update(delta_time);
    }

    // Handle Player movement
    Pos pos;
    int i = -1;
    for (auto &player : players) {
        i++;
        if (player.CanMakeMove(&pos)) {
            if (!player.IsAlive())
                continue;
            else if (CollectFood(pos)) {
                // Collide with food
                player.score++;
                player.GrowBody();
                header->SetScore(i, player.score);
                mixer->Play(snd_chomp);
            }
            else if (CollideWithSnake(&player, pos)) {
                // Collide with self
                mixer->Play(snd_crash);
                player.Kill();
                continue;
            }
            else if (CollideWithOtherSnakes(&player, pos)) {
                // Collide with other player
                mixer->Play(snd_crash);
                player.Kill();
                continue;
            }
            else if (CollideWithWall(pos)) {
                // Collide with a wall
                mixer->Play(snd_crash);
                player.Kill();
                continue;
            }
            player.MakeMove();
            for (auto &pos : player.PopChangedPositions())
                redraws_required.push_back(pos);
        }
    }

    // Replenish food
    if (food.empty()) {
        std::vector<Pos> valid;
        int x, y;
        for (auto fruit_spawn : level->fruit_spawn) {
            x = fruit_spawn.x;
            y = fruit_spawn.y;
            bool free = true;
            // Can't spawn on player
            for (auto &player : players) {
                auto body = player.Body();
                auto ptr = find(body->begin(), body->end(), Pos(x, y));
                if (ptr != body->end()) {
                    free = false;
                    break;
                }
            }
            // Can't spawn on wall
            if (free)
                for (auto &wall : level->walls) {
                    if (wall.first == Pos(x, y)) {
                        free = false;
                        break;
                    }
                }
            if (free)
                valid.emplace_back(x, y);
        }
        std::uniform_int_distribution<std::mt19937::result_type> dist(0, valid.size() - 1);
        auto i = dist(rng);
        food.emplace_back(valid[i].x, valid[i].y);
        new_food++;
    }
}

void Field::PartialRender(Renderer *renderer) {
    // Redraw select fields
    if (!redraws_required.empty()) {
        Rect dest = {0, 0, square_size, square_size};
        for (auto &draw : redraws_required) {
            dest.x = square_size * draw.x;
            dest.y = square_size * draw.y + y_offset;
            renderer->RenderSquare((draw.x+draw.y) % 2 == 0 ? colour_light : colour_dark, &dest);
        }
        redraws_required.clear();
    }

    // Render new food
    if (new_food > 0) {
        if (new_food <= food.size())
            for (int i = 0; i < new_food; ++i) {
                auto pos = food[food.size() - i - 1];
                Rect dest = {ScreenX(pos.x), ScreenY(pos.y), square_size, square_size};
                renderer->Render(raspberry, &dest);
            }
        new_food = 0;
    }

    // Redraws players
    for (auto &player : players)
        if (player.PopRedrawBody())
            player.Render(renderer, this);
}

void Field::RenderAll(Renderer *renderer) {
    // Draw Field
    Rect dest = {0, 0, square_size, square_size};
    for (int y = 0; y < field_height; ++y) {
        for (int x = 0; x < field_width; ++x) {
            dest.x = square_size * x;
            dest.y = square_size * y + y_offset;
            renderer->RenderSquare((x+y) % 2 == 0 ? colour_light : colour_dark, &dest);
        }
    }

    // Draw Walls
    for (auto &wall : level->walls) {
        dest.x = square_size * wall.first.x;
        dest.y = square_size * wall.first.y + y_offset;
        renderer->Render(wall.second, &dest);
    }
}

bool Field::CollectFood(Pos pos) {
    auto ptr = find(food.begin(), food.end(), pos);
    if (ptr != food.end()) {
        redraws_required.push_back(*ptr);
        food.erase(ptr);
        return true;
    }
    return false;
}

bool Field::CollideWithSnake(Snake *player, const Pos pos) {
    for (auto segment : *player->Body()) {
        if (segment == pos)
            return true;
    }
    return false;
}

bool Field::CollideWithOtherSnakes(Snake *player, Pos pos) {
    for (auto &snake : players) {
        if (&snake == player)
            continue;
        if (CollideWithSnake(&snake, pos))
            return true;
    }
    return false;
}

bool Field::CollideWithWall(Pos pos) {
    for (auto &wall : level->walls) {
        if (wall.first == pos)
            return true;
    }
    return false;
}

bool Field::AllSnakesDead() {
    for (auto &player : players)
        if (player.IsAlive())
            return false;
    return true;
}
