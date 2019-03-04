#ifndef SUPERSNAKES_LIBRETRO_SNAKE_HPP
#define SUPERSNAKES_LIBRETRO_SNAKE_HPP


#include <deque>
#include <vector>
#include "engine/renderer.hpp"
#include "field.hpp"

class Field;

class Snake {
public:
    int score = 0;
    int wins = 0;

    Snake(int square_size, int field_width, int field_height, std::pair<Pos, Direction> spawn, SnakeAssets *assets);

    void Update(double delta_time);

    void Render(Renderer* renderer, Field *field);

    void MoveUp();

    void MoveDown();

    void MoveLeft();

    void MoveRight();

    /// Checks if the snake is ready to move to the next tile.
    /// \param position The position the snakes head will move to.
    /// \return True if the snake is ready to move, false otherwise.
    bool CanMakeMove(Pos *position);

    void MakeMove();

    const std::deque<Pos> *Body() {
        return &body;
    }

    /// Grows or shrinks the snakes body by a specified amount.
    /// \param delta the number of body segments to append or remove.
    void GrowBody(size_t delta = 1);

    Pos HeadLocation() {
        return body.front();
    }

    bool PopRedrawBody() {
        auto temp = redraw_body;
        redraw_body = false;
        return temp;
    }

    std::vector<Pos> PopChangedPositions() {
        auto temp = need_to_be_updated;
        need_to_be_updated = {};
        return temp;
    }

    /// Determines the next valid move in the player's move queue.
    /// \return The next valid move.
    Direction PeekNextMove();

    bool IsAlive() {
        return alive;
    }

    void Kill() {
        alive = false;
    }

private:
    /// tiles per second
    int speed = 4;
    double delay = 0;
    size_t body_desired_size;
    std::deque<Pos> body;
    std::deque<Direction> body_orientations;
    std::vector<Pos> need_to_be_updated;
    bool redraw_body;
    Texture *head;
    Texture *body_straight;
    Texture *body_turn;
    Texture *tail;
    Texture *tail_turn_left;
    Texture *tail_turn_right;
    Rect screen_rect;
    static const int MOVES_QUEUE_MAX_SIZE = 3;
    std::deque<Direction> moves_queue;
    bool can_make_move = false;
    int field_width;
    int field_height;
    bool alive = true;

    Direction PopNextMove();

    Pos ApplyDirection(Pos pos, Direction move);
};


#endif //SUPERSNAKES_LIBRETRO_SNAKE_HPP
